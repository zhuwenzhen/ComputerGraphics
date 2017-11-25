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
void Cone::computeTriangle(int n, int m){
	tesselation.clear();

	// compute the bottom surfaces
	for (int i = 0; i < n; i++){		
		double k = -0.5;
		struct triangle result;
		result.p1 = Point3(0., k, 0.);
		result.p3 = Point3(0.5 * cos(2. * i * PI / n), k, 0.5 * sin(2. * i * PI / n));
		result.p2 = Point3(0.5 * cos(2. *(i + 1) * PI / n), k, 0.5 * sin((i + 1.) * 2. * PI / n));

		ReverseTriangleVertex(result);
		computeNormal(result);
		Vector3 n = result.n;
		result.nVertex.push_back(n); // no time to fix here. Smooth shading is wrong
		tesselation.push_back(result);
	}

	// compute side 1
	for (int i = 1; i <= n; i++){
		for (int j = 1; j <= m; j++){
			for (int k = 1; k <= m; k++){
				struct triangle result;
				result.p1 = Point3(0.5 * cos(2. * i * PI / n) * (1 - j*1. / m), -0.5 + (j * 1.) / m, 0.5 * sin(2. *i * PI / n) * (1 - j*1. / m));
				result.p3 = Point3(0.5 * cos(2. * i * PI / n) * (1 - (j - 1.) / m), -0.5 + (j - 1.) / m, 0.5 * sin(2. *i * PI / n)* (1 - (j - 1.) / m));
				result.p2 = Point3(0.5 * cos(2. * (i + 1) * PI / n)* (1 - (j - 1.) / m), -0.5 + (j - 1.) / m, 0.5 * sin(2. *(i + 1) * PI / n) * (1 - (j - 1.) / m));
				// face-normal
				computeNormal(result);
				// vertex-normal
				Point3 p1, p2, p3;
				p1 = result.p1;
				p2 = result.p2;
				p3 = result.p3;
				result.nVertex.push_back(Vector3(p1[0], 0.5 * sqrt(p1[0] * p1[0] + p1[2] * p1[2]), p1[2]));
				result.nVertex.push_back(Vector3(p2[0], 0.5 * sqrt(p2[0] * p2[0] + p2[2] * p2[2]), p2[2]));
				result.nVertex.push_back(Vector3(p3[0], 0.5  *sqrt(p3[0] * p3[0] + p3[2] * p3[2]), p3[2]));

				tesselation.push_back(result);
			}
		}
	}

	//// compute complementary side
	for (int i = 1; i <= n; i++){
		for (int j = 1; j <= m; j++){
			struct triangle result;
			result.p1 = Point3(0.5 * cos(2. * (i + 1) * PI / n) * (1 - j * 1. / m), -0.5 + j * 1. / m, 0.5 * sin(2. *(i + 1) * PI / n)* (1 - j * 1. / m));
			result.p3 = Point3(0.5 * cos(2. * i * PI / n)* (1 - j*1. / m), -0.5 + (j * 1.) / m, 0.5 * sin(2. *i * PI / n)* (1 - j*1. / m));
			result.p2 = Point3(0.5 * cos(2. * (i + 1) * PI / n)* (1 - (j - 1.) / m), -0.5 + (j - 1.) / m, 0.5 * sin(2. *(i + 1) * PI / n)* (1 - (j - 1.) / m));
			// face-normal
			computeNormal(result);
			// vertex-normal
			Point3 p1, p2, p3;
			p1 = result.p1;
			p2 = result.p2;
			p3 = result.p3;
			result.nVertex.push_back(Vector3(p1[0], 0.5*sqrt(p1[0] * p1[0] + p1[2] * p1[2]), p1[2]));
			result.nVertex.push_back(Vector3(p2[0], 0.5*sqrt(p2[0] * p2[0] + p2[2] * p2[2]), p2[2]));
			result.nVertex.push_back(Vector3(p3[0], 0.5*sqrt(p3[0] * p3[0] + p3[2] * p3[2]), p3[2]));

			tesselation.push_back(result);
		}
	}

}