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

// n: number of slices; m: number of stacks
void Cylinder::computeTriangle(int n, int m){
	tesselation.clear();

	// compute the top and bottom surfaces
	for (int i = 0; i < n; i++){
		for (int ki = -1; ki <= 1; ki += 2){
			double k = ki / 2.;
			struct triangle result;
			result.p1 = Point3(0., k, 0.);
			result.p3 = Point3(0.5 * cos(2. * i * PI / n), k, 0.5 * sin(2. * i * PI / n));
			result.p2 = Point3(0.5 * cos(2. *(i + 1) * PI / n), k, 0.5 * sin((i + 1.) * 2. * PI / n));

			if (k == -0.5){
				ReverseTriangleVertex(result);
				result.nVertex.push_back(Vector3(0., -1., 0.));
				result.nVertex.push_back(Vector3(0., -1., 0.));
				result.nVertex.push_back(Vector3(0., -1., 0.));
			}
			else{
				result.nVertex.push_back(Vector3(0., 1., 0.));
				result.nVertex.push_back(Vector3(0., 1., 0.));
				result.nVertex.push_back(Vector3(0., 1., 0.));
			}		
			computeNormal(result);

			tesselation.push_back(result);
		}
	}


	//// compute side 1
	for (int i = 1; i <= n; i++){
		for (int j = 1; j <= m; j++){
			struct triangle result;
			result.p1 = Point3(0.5 * cos(2. * i * PI / n), -0.5 + (j - 1.) / m, 0.5 * sin(2. *i * PI / n));
			result.p3 = Point3(0.5 * cos(2. * i * PI / n), -0.5 + (j * 1.) / m, 0.5 * sin(2. *i * PI / n));
			result.p2 = Point3(0.5 * cos(2. * (i + 1) * PI / n), -0.5 + (j - 1.) / m, 0.5 * sin(2. *(i + 1) * PI / n));

			// barel's normal is (x, 0, z) -> (x, z, 0)
		
			result.nVertex.push_back( Vector3(result.p1[0], 0., result.p1[2]));
			result.nVertex.push_back(Vector3(result.p2[0], 0., result.p2[2]));
			result.nVertex.push_back( Vector3(result.p3[0], 0., result.p3[2]));
			
			ReverseTriangleVertex(result);
	
			computeNormal(result);
			tesselation.push_back(result);
		}
	}

	//// compute complementary side
	for (int i = 1; i <= n; i++){
		for (int j = 1; j <= m; j++){
			struct triangle result;
			result.p1 = Point3(0.5 * cos(2. * (i + 1) * PI / n), -0.5 + (j * 1.) / m, 0.5 * sin(2. *(i + 1) * PI / n));
			result.p3 = Point3(0.5 * cos(2. * i * PI / n), -0.5 + (j * 1.) / m, 0.5 * sin(2. *i * PI / n));
			result.p2 = Point3(0.5 * cos(2. * (i + 1) * PI / n), -0.5 + (j - 1.) / m, 0.5 * sin(2. *(i + 1) * PI / n));
			
			// barel's normal is (x, 0, z) 
			result.nVertex.push_back(Vector3(result.p1[0], 0., result.p1[2]));
			result.nVertex.push_back(Vector3(result.p2[0], 0., result.p2[2]));
			result.nVertex.push_back(Vector3(result.p3[0], 0., result.p3[2]));

			computeNormal(result);
			tesselation.push_back(result);
		}
	}
}