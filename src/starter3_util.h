#ifndef STARTER1_UTIL_H
#define STARTER1_UTIL_H

#include <cstdint>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

float deg2rad(float deg);
float rad2deg(float rad);

struct GLFWwindow;
// creates a window using GLFW and initializes an OpenGL 3.3+ context.
GLFWwindow* createOpenGLWindow(int width, int height, const char* title);

// returns 0 on error
// program must be freed with glDeleteProgram()
uint32_t compileProgram(const char* vertexshader, const char* fragmentshader);

static const char* c_vertexshader = R"RAWSTR(
#version 330
// These are vertex attributes.
// You can define custom attributes,
// like color or curvature.
layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec3 Color;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 N;

// var_ (varying) variables are output in the vertex
// shader and are interpolated by the GPU for each
// pixel of the triangle.
out vec3 var_Position;
out vec3 var_Normal;
out vec4 var_Color;

void main () {
    // Simple pass-through vertex shader
    gl_Position = P * V * M * vec4(Position, 1);
    vec4 position_world = M * vec4(Position, 1);
    var_Position = position_world.xyz / position_world.w;

    vec3 normal_world = (N * vec4(Normal, 1)).xyz;
    var_Normal = normalize(normal_world);
    var_Color = vec4(Color, 1);
}
)RAWSTR";
static const char* c_fragmentshader_color = R"RAWSTR(
#version 330
in vec4 var_Color;
in vec3 var_Normal;
in vec3 var_Position;

layout(location=0) out vec4 out_Color;

void main() {
    out_Color = var_Color;
}
)RAWSTR";

static const char* c_fragmentshader_light = R"RAWSTR(
#version 330
in vec4 var_Color;
in vec3 var_Normal;
in vec3 var_Position;

uniform vec3 camPos;

uniform vec3 diffColor;
uniform vec3 specColor;
uniform vec3 ambientColor = vec3(.05);
uniform float shininess = 20;
uniform float alpha = 0;

uniform vec3 lightPos;
uniform vec3 lightDiff;

layout(location=0) out vec4 out_Color;

uniform float kd = .3;
uniform float ks = .6;

vec4 blinn_phong() {
    // Implement Blinn-Phong Shading Model
    // 1. Convert everything to world space
    //    and normalize directions
    vec4 pos_world = vec4(var_Position, 1);
    vec3 normal_world = normalize(var_Normal);
    pos_world /= pos_world.w;
    vec3 light_dir = lightPos - pos_world.xyz;
    vec3 cam_dir = camPos - pos_world.xyz;
    float distsq = dot(light_dir, light_dir);
    light_dir = normalize(light_dir);
    cam_dir = normalize(cam_dir);

    // 2. Compute Diffuse Contribution
    float ndotl = max(dot(normal_world, light_dir), 0.0);
    vec3 diffContrib = kd * lightDiff
                       * ndotl / distsq;

    // 3. Compute Specular Contribution
    vec3 R = reflect( -light_dir, normal_world );
    float eyedotr = max(dot(cam_dir, R), 0.0);
    vec3 specContrib = ks * pow(eyedotr, shininess) *
                       lightDiff / distsq;

    // 5. Add ambient, specular and diffuse contributions
    return  + vec4(ambientColor + diffContrib + specContrib, alpha);
}

void main () {
    out_Color = blinn_phong();
}
)RAWSTR";

#endif
