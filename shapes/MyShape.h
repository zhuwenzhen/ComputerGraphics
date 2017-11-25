#include "../UIInterface.h"
#include "../cse452.h"
#include "FL/FL_Window.h"
#include "Point3.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "math.h"
#include <vector>

using namespace std;

class Shape{
public:	
	struct triangle{
		Point3 p1;
		Point3 p2;
		Point3 p3;
		Vector3 n;
		vector<Vector3> nVertex; // stores the normals of p1, p2, p3
	};

	Shape(){}; // Constructor
	~Shape(){}; // Destructor
	
	void computeNormal(triangle &tr);
	void drawShapeWireFrame();
	void drawShapeFlat();
	void drawShapeSmooth();
	void ReverseTriangleVertex(triangle & t);

	

protected:
	// store all triangles
	vector<triangle> tesselation;
};


class Cube: public Shape{
public:
	Cube(){};
	~Cube(){};
	void computeTriangle(int n);
	void computeVertexNormal(triangle &tr);
};

class Cylinder :public Shape{
public:
	Cylinder(){};
	~Cylinder(){};
	void computeTriangle(int n, int m);
};

class Cone : public Shape{
public:
	Cone(){};
	~Cone(){};
	void computeTriangle(int n, int m);
};

class Sphere : public Shape{
public:
	Sphere(){};
	~Sphere(){};
	
	void computeTriangle(int n);
	void computeVertex(vector<triangle> &tr_set );
	vector<triangle> divideTriangle(triangle t);
	Point3 Mean(Point3 p1, Point3 p2);
	void triangleNormalizer(triangle &t);
	//void shrinkSphere(triangle &t);
};
