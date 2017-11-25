#include "../cse452.h"
#include "../Color.h"
#include <iostream>
#include <FL/FL.h>
#include <FL/gl.h>
#include <FL/glut.h>
#include "MyShape.h"
#include <algorithm>

using namespace std;

void Cube::computeTriangle(int n){
	tesselation.clear();
	for (int i = 1; i <= n; i++){
		for (int j = 1; j <= n; j++){
			for (int ki = -1; ki <= 1; ki += 2){
				for (int m = 0; m <= 2; m++){
					for (int h = -1; h <= 1; h += 2){
						double k = ki / 2.;
						struct triangle result;
						result.p1 = Point3(double(h)* ((i - 1.) / n - 0.5), double(h)* ((j - 1.) / n - 0.5), double(h)*k);
						result.p2 = Point3(double(h)* (i * 1. / n - 0.5), double(h)* ((j - 1.) / n - 0.5), double(h)*k);
						result.p3 = Point3(double(h)* ((i - 1.) / n - 0.5), double(h)* (double(j) / n - 0.5), double(h)*k);

						result.p1 = result.p1.RotateLeft(m);
						result.p2 = result.p2.RotateLeft(m);
						result.p3 = result.p3.RotateLeft(m);

						if (h * ki == -1) {
							Point3 ptemp = result.p2;
							result.p2 = result.p1;
							result.p1 = ptemp;
						}
		
						computeNormal(result);	//used for flat shading
	
						Vector3 n = result.n;
						result.nVertex.push_back(n);
						result.nVertex.push_back(n);
						result.nVertex.push_back(n);

						tesselation.push_back(result);
					}
				}
			}
		}
	}	
}

void Cube::computeVertexNormal(triangle &tr){
	tr.nVertex[0] = tr.n;
	tr.nVertex[1] = tr.n;
	tr.nVertex[2] = tr.n;
}