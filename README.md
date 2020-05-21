# the_globe

Showcases my orbit camera approach based on Ken Shoemake's [ArcBall](https://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.457.6530).
It is controled via mouse input and translates the rotation immediately while holding down the LMB.
At the same time, the momentum is captured and on abruptly releasing the LMB it is translated into an animation, which smoothly exhausts over time.

Dependencies:
- [glfw3](https://www.glfw.org/)
- opengl + glew
- glm

In part based on Hector Peeters's [opengl_premake_boilerplate](https://github.com/HectorPeeters/opengl_premake_boilerplate).
