ReadMe

# Assignment 2: Shapes

### Wenzhen Zhu
### Date: 02/22/2016, Monday

> Late days used this lab: 0

## Structure
### Shape (Base class)

+ triangle : struct
	+ contains 3 points and 1 normal vector to this triangle/face, and a vector of 3 normal vectors corresponding to the 3 vertices

+ computeNormal: compute the normal vector (of face) given a triangle.
+ drawShapeWireFrame(): do the wire frame
+ drawShapeFlat(): do the flat shading
+ drawShapeSmooth(): do the smooth shading
+ ReverseTriangleVertex(): make the triangle counter-clockwise if they are not

+ tessellation is a vector of triangles, I use it to store triangles. 

### Cube (derived class #1)
+ computeTriangle(int n): given 1st parameter n, it generate all correct triangles, and push those triangles to tesselation.
+ computeVertexNormal(triangle &tr): given a triangle with p1, p2, p3 it can calculate the face’s normal. 

### Cylinder(derived class #2)
+ computeTriangle(int n, int m) : functionality is similar as cube


### Cone(derived class #3)
+ computeTriangle(int n, int m): functionality is similar as cube

### Sphere(derived class #4)
I found this python code on line
https://sites.google.com/site/dlampetest/python/triangulating-a-sphere-recursively

However, after I tried it in python notebook, it doesn’t work. I think I understand this recursive midpoint divide process. I did a prototype in Mathematica, which you can find in the appendix.

### Pictures

#### Cube#### Cylinder and Cone#### Sphere