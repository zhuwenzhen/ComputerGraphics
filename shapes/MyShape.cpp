#include "../cse452.h"
#include "../Color.h"
#include <iostream>
#include <FL/FL.h>
#include <FL/gl.h>
#include <FL/glut.h>
#include "MyShape.h"
#include <algorithm>
#include <Point3.h>

using namespace std;

void Shape::ReverseTriangleVertex(triangle & tr){
	Point3 ptemp = tr.p1;
	tr.p1 = tr.p3;
	tr.p3 = ptemp;
}
void Shape::computeNormal(triangle & tr){
	Vector3 a, b, normal;
	a = tr.p1 - tr.p2;
	b = tr.p1 - tr.p3;
	normal = a^b;
	normal.normalize();
	tr.n = normal;
}

void Shape::drawShapeWireFrame(){
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < tesselation.size(); i++){	
		glVertex3d(tesselation[i].p1[0], tesselation[i].p1[1], tesselation[i].p1[2]);
		glVertex3d(tesselation[i].p2[0], tesselation[i].p2[1], tesselation[i].p2[2]);
		glVertex3d(tesselation[i].p3[0], tesselation[i].p3[1], tesselation[i].p3[2]);
	}
	glEnd();
}

void Shape::drawShapeFlat(){
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < tesselation.size(); i++){
		glNormal3d(tesselation[i].n[0], tesselation[i].n[1], tesselation[i].n[2]);
		glVertex3d(tesselation[i].p1[0], tesselation[i].p1[1], tesselation[i].p1[2]);
		glVertex3d(tesselation[i].p2[0], tesselation[i].p2[1], tesselation[i].p2[2]);
		glVertex3d(tesselation[i].p3[0], tesselation[i].p3[1], tesselation[i].p3[2]);
	}
	glEnd();
}

void Shape::drawShapeSmooth(){
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < tesselation.size(); i++){
		glNormal3d(tesselation[i].nVertex[0][0], tesselation[i].nVertex[0][1], tesselation[i].nVertex[0][2]);
		glVertex3d(tesselation[i].p1[0], tesselation[i].p1[1], tesselation[i].p1[2]);
		glNormal3d(tesselation[i].nVertex[1][0], tesselation[i].nVertex[1][1], tesselation[i].nVertex[1][2]);
		glVertex3d(tesselation[i].p2[0], tesselation[i].p2[1], tesselation[i].p2[2]);
		glNormal3d(tesselation[i].nVertex[2][0], tesselation[i].nVertex[2][1], tesselation[i].nVertex[2][2]);
		glVertex3d(tesselation[i].p3[0], tesselation[i].p3[1], tesselation[i].p3[2]);
	}
	glEnd(); 
}
