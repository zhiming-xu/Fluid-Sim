# <center>CS 184: Computer Graphics and Imaging</center>

## <center>Final Project: Smoothed Particle Hydrodynamics</center>

#### <center>Created by Zhiming Xu, Mingfang Zhang, [Cheng Chen](https://github.com/Julius-c)</center>

#### <center>SIDs: 3034485754, 3034485741, 3034488887</center>

##### <center>[Click here for project milestone](#milestone)</center>

##### <center>[Click here for project proposal](#proposal)</center>

### Abstract

We read the Smoothed Particle Hydronamics paper by $J. J. Monaghan$ and the Position Based Fluids paper by $Macklin$ and $Müller$, and finally build a fluid simulator based on the simplified algorithm which is basically the SPH method. This technique combines pressure, viscosity and gravity to simulate water. We firstly implement the basic mathematical and physical simulation model. Afterwards, we polish our shader programs to create fantastic lighting effects.

### Technical approach and implementation

We can show either 2D or 3D simulation by commenting out or defining a c++​ macro. In 3D Particle Fluid Simulation, at the very beginning, we set up a ground plane and four virtual surrounding walls with which the particles collide. Our approach then can be divided into two main parts — collision detection and system update, force application and time step integration.

We've learned from class the approach of collision detection and correction and it's time we put it into good use.

Our calculation of total simulation force exerted on a single particle is a combination of three components which comes from pressure, viscosity and gravity.



Afterwards, we implement three different integration methods to update particle position.



**Problems encountered**

Throughout the course of our implementation journey, we encountered a couple of different technical problems.

To begin with, a bug in collision correction part leads to our particles' leaking out of the ground plane and bounding walls. (一个对应视频)

After navigating through according code block, we locate our calculation bug and successfully address this issue.

The next issue is about 

**Lessons learned**

One of the critical lesson we've learned is to walk through several academic research paper and adjust the formulas and algorithm pseudo codes into mathematical and physical simulation model according to our project code structure.

### Results

Below is a demonstration of our results with xxx shading:

### References

- [Smoothed Particle Hydrodynamics](http://adsabs.harvard.edu/full/1992ARA%26A..30..543M)
- [Position Based Fluids[Macklin, Muller]](http://mmacklin.com/pbf_sig_preprint.pdf)
- 

### Contributions from each team member

- Zhiming Xu 
- Mingfang Zhang
- Cheng Chen

---

## <center><span id="milestone">Milestone Status Report</span></center>

##### <center>[Click here for milestone video](<https://drive.google.com/open?id=1nC8FUH43q1RYpyE485neEkTzxLr1EyyS>)</center>

##### <center>[Click here for milestone slides](<https://drive.google.com/open?id=1hGhFIYccuz43eb13ja3d1o1CwRNDRuiS>)</center>

## Summary

While the Navier-Stokes equations can describe the fluid movement accurately, it was too computationally expensive to implement it to simulate the fluid. 

> "It's a common misconception that visual effects are about simulating reality. They're not. Reality is boring. Visual effects are about simulating something dramatic."
>
> \- Jonathan Cohen, Rhythm & Hue 

As a result, we use Particle-Based simulations. In particular, we use a technique called Smoothed Particle Hydrodynamics (SPH).

In SPH approach, Fluid volume is discretized by particles and each particles represents a certain amount of fluid volume.

$$V_i = m_i/\rho_i$$

Particles store attributes, and to evaluate an attribute, we take weighted average of particle values within a neighborhood. Smoothing kernel $W$ prescribes interpolation weights.

<div align="middle">
  <table style="width=100%">
        <img src="images/1.png" align="middle" width=400px/>
  </table>
</div>

Then we sum up contribution of neighboring particles $j$:

$$A(x) = \sum_jm_j/\rho_j * A_jW(x-x_j,h)$$

$A(x)$ : Quantity $A$ at arbitrary position $x$; 

$\sum$ : Sum over all neighbor particles $j$ within $h$; 

$A_j$ : Quantity $A$ of particle $j$; 

$W$ : Smoothing kernel.

Up to now, we have basically implemented the algorithm described from the paper “Position-Based Fluids” by Macklin and Müller. Some key components are as follows.

**Particle Neighbor Finding**. With the help of a basic spatial hashmap.

<div align="middle">
  <table style="width=100%">
    	  <img src="images/neighbor.png" align="middle" width=400px/>
  </table>
</div>

**Density constraints**. In order to make the fluid resist compression, we calculate position correction $\bold{\Delta p_i}$ that satisfies the constraints $C(p+\Delta p)=0$, where $\begin{equation}C_i(p_1, \cdots, p_n) = \frac{\rho_i}{\rho_0} - 1\end{equation}$ and $\rho_i$ is the density at a particle evaluated by the *poly6 kernel*.

<div align="middle">
  <table style="width=100%">
    	  <img src="images/Density.png" align="middle" width=400px/>
  </table>
</div>

**Surface tension**. To address the particle clustering and clumping problem in *SPH simulations*, an artificial pressure term $s_{corr}$ is added in the particle position update. 

<div align="middle">
  <table style="width=100%">
    	  <img src="images/scorr.png" align="middle" width=400px/>
  </table>
</div>

**Collision handling**. Implement collosions with planes to keep the fluid inside the box.

**Vorticity confinement**. Vorticity confinement is implemented to counteract undesirable additional damping introduced by position based methods. The vorticity at a particle is calculated and afterwards a corrective force is applied.

**XSPH viscosity**. XSPH viscosity is implemented for coherent motion.

<div align="middle">
  <table style="width=100%">
    	  <img src="images/coherent.png" align="middle" width=400px/>
  </table>
</div>

However, the rendering procedure is far too slow and the fluid appears so coarse. We will set out to solve these problems in the remaining days.

## Schedule Update

Week 1: Fine-grained water appearance. Consider adding one or more of these features. Preparing final report as well.

Week 2: Improve code structure and cleanliness. Create a satisfying result video for the final presentation. Update final report as well.

---

## <center><span id="proposal">Final Project Proposal</span></center>

## Overview

In the lectures, we saw several demonstrations of fluids simulation and we all found it excited to discover the theory behind it and to implement it by ourselves. As we know, accurate stimulations of fluids can be applied in a really wide range of areas and the final effects are really cool. That is the reason why we are so driven to pursue this technical challenge.

Problem Description
-------------------

Creating a fluid simulation means that we should simulate the fluid to match its real physical properties, which always leads to a high level of time complexity.  We aim to create an iterative density solver in this project, which will be integrated into the position based dynamics framework. We hope to be able to create a method that allows for similar incompressibility and convergence to modern smoothed particle hydrodynamic solvers, but inherits the stability of the geometric, position based dynamics method, allowing large time steps suitable for real-time applications.

Goals and Deliverables
----------------------

Our baseline goal for this project is being able to create water/fluidic behavior between particles in a simulated environment. We expect to find some difficulties in correctly implementing the physics of the fluids, as well as making the particles have water like properties, and quantify the accuracy of our simulation by comparing its visual similarity to real life examples or real life observations. Ideally, we would be able to run a real time simulation, but that would be a bit more of a reach if we are ahead of schedule. Some other future goals we were thinking of are interactions between multiple bodies of water (dropping a large blob of water into a body of water) and simulating interactions between different Newtonian fluids.

Schedule
--------

Week 1: Read resources listed below to get an idea of what to do. Outline pseudocode if possible.

Week 2: Code - get basic physics working

Week 3: Code - water appearance. Consider adding one or more of these features: interface between fluids, water blob and real-time simulation.

Week 4: Debug - make already included features work together

Finish the write-up as we go over the steps mentioned above.

Resources
---------

*   [Position Based Fluids paper](http://mmacklin.com/pbf_sig_preprint.pdf) in conjunction with [PositionBasedDynamics framework](https://github.com/InteractiveComputerGraphics/PositionBasedDynamics).
*   We are very likely to use [OpenGL](http://www.opengl-tutorial.org/), so we will need to learn it.
*   Potentially import everything into [Mitsuba](http://www.mitsuba-renderer.org/) for easier and better rendering.
*   We may also need to use AWS instance if we need considerable amount of computation power.