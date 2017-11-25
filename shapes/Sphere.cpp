#include "../cse452.h"
#include "../Color.h"
#include <iostream>
#include <FL/FL.h>
#include <FL/gl.h>
#include <FL/glut.h>
#include "MyShape.h"
#include <algorithm>

#define PI M_PI
using namespace std;

vector<Point3> octahedronVertices = { Point3(1., 0., 0.), Point3(-1., 0, 0.), Point3(0., 1., 0.),
									  Point3(0., -1., 0.), Point3(0., 0, 1.), Point3(0., 0., -1) };

int octahedronTrianglesVertexIndex[8][3] = { { 0, 4, 2 }, { 2, 4, 1 }, 
											 { 1, 4, 3 }, { 3, 4, 0 }, 
											 { 0, 2, 5 }, { 2, 1, 5 }, 
											 { 1, 3, 5 }, { 3, 0, 5 } };

void Sphere::computeVertex(vector<triangle> &tr_set){
	for (int i = 0; i < 8; i++){
		triangle tri_temp;
		
		Point3 p1 = octahedronVertices[octahedronTrianglesVertexIndex[i][0]];
		Point3 p2 = octahedronVertices[octahedronTrianglesVertexIndex[i][1]];
		Point3 p3 = octahedronVertices[octahedronTrianglesVertexIndex[i][2]];
		
		tri_temp.p1 = p1;
		tri_temp.p2 = p2;
		tri_temp.p3 = p3;

		tr_set.push_back(tri_temp);
	}
}

Point3 Sphere:: Mean(Point3 p1, Point3 p2){
	Point3 result;
	double x = (p1[0] + p2[0]) / 2;
	double y = (p1[1] + p2[1]) / 2;
	double z = (p1[2] + p2[2]) / 2;
	result = Point3(x, y, z);
	return result;
}

vector<Shape:: triangle> Sphere::divideTriangle(triangle t){
	Point3 a, b, c;

	a = Mean(t.p1, t.p3);
	b = Mean(t.p1, t.p2);
	c = Mean(t.p2, t.p3);

	triangle t1;
	t1.p1 = t.p1;	t1.p2 = b;	t1.p3 = a;

	triangle t2;
	t2.p1 = b;	t2.p2 = t.p2;	t2.p3 = c;

	triangle t3;
	t3.p1 = a; t3.p2 = b; t3.p3 = c;

	triangle t4;
	t4.p1 = a; t4.p2 = c; t4.p3 = t.p3;

	vector<triangle> newTr;

	newTr.push_back(t1);
	newTr.push_back(t2);
	newTr.push_back(t3);
	newTr.push_back(t4);

	return newTr;
}

void Sphere:: triangleNormalizer(triangle &t){
	t.p1.normalize();
	t.p2.normalize();
	t.p3.normalize();
}
//
//void Sphere::shrinkSphere(triangle &t){
//	double p1x = t.p1[0];
//	double p1y = t.p1[1];
//	double p1z = t.p1[2];
//
//	double p2x = t.p1[0];
//	double p2y = t.p1[1];
//	double p2z = t.p1[2];
//
//	double p3x = t.p1[0];
//	double p3y = t.p1[1];
//	double p3z = t.p1[2];
//
//	Point3 s = t.p1.operator*=(0.5);
//}

void Sphere::computeTriangle(int n){
	tesselation.clear();
	// 1. initialize vertices set
	vector<triangle> vertex_init;
	computeVertex(vertex_init);

	for (unsigned int i = 0; i < vertex_init.size(); i++){
		triangle t = vertex_init[i];
		Point3 ptemp = t.p2;
		t.p2 = t.p3;
		t.p3 = ptemp;
		vertex_init[i] = t;
	}


	// 2. divide
	for (int i = 1; i < n; i++){
		vector<triangle> triangle_temp;
		triangle_temp = vertex_init;
		vertex_init.clear();
		for (unsigned int j = 0; j < triangle_temp.size(); j++){
			vector<triangle> temp = divideTriangle(triangle_temp[j]);
			for (vector<triangle> ::iterator it = temp.begin(); it != temp.end(); it++){
				triangle t = *it;
				
				triangleNormalizer(t);
				t.p1.operator*=(0.5);
				t.p2.operator*=(0.5);
				t.p3.operator*=(0.5);
				
				computeNormal(t);// flat shading

				t.nVertex.push_back(Vector3(t.p1[0], t.p1[1], t.p1[2]));
				t.nVertex.push_back(Vector3(t.p2[0], t.p2[1], t.p2[2]));
				t.nVertex.push_back(Vector3(t.p3[0], t.p3[1], t.p3[2]));

				vertex_init.push_back(t);
			}	
		}
	}
	tesselation = vertex_init;
}
