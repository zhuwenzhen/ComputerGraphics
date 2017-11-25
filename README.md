# ComputerGraphics
Computer Graphics Class (Fall 2016)

## Assignments

### 1. Airbrush -- Due: 11:59pm Monday, Feb 8

In this assignment, you will be writing a simple application that will sport a few different types of
airbrushes, geometric drawing tools, and have the capability to load and save the images you alter.

### 2. Tessellation of shapes -- Due: 11:59pm Monday, Feb 22

Graphics cards are best at drawing triangles. In this assignment, you will be handling the process
that pertains to tessellating objects. That is, you will be breaking up the “standard” shapes into
a lot of triangles that, when put together, look as much like actual shapes as possible. Flat-faced
objects will be pretty simple, and will come out looking just like the actual thing. On the other hand,
curved surfaces won’t look exactly like the real thing. It is possible to get a better approximation of
a curved surface using more triangles, but the whole idea behind tessellating objects is to simplify
the process of displaying them.

The demo lets you choose between a few different shapes, choose whether they wish to view the shape
as a solid or a wireframe, and the orientation of the shape. The interface also allows you to modify
how finely tessellated the object is. That is, how small the triangles are. Notice that if you use
really high tessellation values (i.e., using more, smaller triangles), even the shapes that have curved
surfaces look really good. However, the time it takes to draw the shape is roughly proportional
to the number of triangles used in the tessellation; if you try to rotate a finely tessellated shape,
you have very jerky movement. You should also keep in mind that there is another end to the
spectrum; it is sometimes not desirable to allow tessellation values below a certain point, as you lose
determining features of the shape you are tessellating. For an example, the demo’s cylinder uses 3
as a lower bound for the first tessellation value. Given anything less the cylinder would collapse to
a flat surface.

### 3. Camera -- Due: 11:59pm Monday, March 7
For this assignment, you will be filling in Camera, which is a class that provides all the methods for
almost all the adjustments that one could perform on a camera. It will be your job to implement
the functionality behind those methods. Once that has been completed, you will possess all the
tools needed to handle displaying three-dimensional objects oriented in any way, and viewed from
any position.
https://www.cse.wustl.edu/~furukawa/cse452/lab3/camera.pdf

### 4. Shape intersection -- Due: 11:59pm Monday, March 28
https://www.cse.wustl.edu/~furukawa/cse452/lab4/shapeIntersect.pdf
In this assignment you will learn how to intersect shapes with rays. Like the previous shapes assignment,
you will be performing your intersection in object space — i.e., the object will be in the coordinate system
defined in the first shapes assignment. Obviously, it will be useful in the future to be able to put the shape
anywhere in the scene and still be able to use the same intersection code. How to do this? Well, if you apply
a matrix M to an object to move it somewhere, you can always apply the matrix M−1
to get it back to its
initial starting place. Suppose you’re given a ray r and a shape S that has been moved by the matrix M,
and you want to find the intersection using the intersection code you’re about to write. How can you get
your ray into S’s object space? Think about this; we’ll come back to it again in later assignments.
You will be using this shape intersect code later in the ray tracing assignment. In ray tracing, you will
be generating rays from the camera through every pixel on the screen, which you will then intersect with
all of the objects in the scene. These rays will spawn new rays that bounce off of intersected objects and
possibly intersect with other objects.

### 5. Sceneview -- Due: 11:59pm Monday, April 11
When you look at a moderately complicated computer generated image, you realize that no person
(who has a life), would sit down and hand place every single one of the triangles necessary to
create that image. The triangle is simply at too low a level to be much good for large-scale scenes.
Displaying a scene of moderate complexity would require the placement of thousands of triangles,
if not tens or hundreds of thousands. To help bridge the difference between low-level triangles and
the complicated scene, we will interpose a layer which is a medium-level description language. This
language will deal with shapes, not triangles, and will deal with applying transformations to those
shapes, as opposed to specifying the coordinates of those shapes absolutely. Color will also be dealt
with on a per-shape basis, as opposed to a per-triangle basis. As you have probably realized, you
have all the tools necessary to deal with such a scene description. In this assignment, you will be
taking a hierarchical scene description (which you will read from a text file), and transform that
scene description into its visual representation.

### 6. Ray tracking -- Due: 11:59pm Monday, April 25
Throughout this semester you have written code that manipulated shapes and
cameras to prepare a scene for rendering. In this (final) assignment, you will be
writing a raytracer for your scenes.