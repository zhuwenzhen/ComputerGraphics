#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>
#include "math.h"

using namespace std;

void MyScene::setPixel(int width, int height, int w, int h, const Color &col, unsigned char* pixel){
	double r, g, b;
	r = col[0]; g = col[1]; b = col[2];
	int i = ((height - 1 - h) * width + w) * 3;
	pixel[i] = (unsigned char)(r * 255.0f);
	pixel[i + 1] = (unsigned char)(g * 255.0f);
	pixel[i + 2] = (unsigned char)(b * 255.0f);
}

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
	if (!isLoaded) {
		return;
	}

	// Add your rendering code here.
	// Keep track of your progress as a value between 0 and 1
	// so the progress bar can update as the rendering progresses
	progress = 0;

	switch (type) {
	case RenderingUI::RENDER_SCANLINE:
		for (int h = 0; h < height; h++){
			for (int w = 0; w < width; w++){
				setPixel(width, height, w, h, background, pixels);
			}
			progress = (h + 1.) / height;
			Fl::check();
		}

		break;
	case RenderingUI::RENDER_RAY_TRACING:
		flattenedTree.clear();
		subgraph.find("root")->second->getMatrix(Matrix4::identity(), flattenedTree);
		stop = false;
		for (int h = 0; h < height; h++){
			if (!stop){
				for (int w = 0; w < width; w++){
					// for each pixel on the screen
					Point3 eye = camera.getEye();
					// 3D pixel location (on the far plane) after WTC transform
					Point3 qs = Point3((w + 0.5) *2. / width - 1., 1. - (2.*h + 1) / height, -1);
					// 3D pixel location in world coordinates
					Point3 qw = camera.getCameraToWorld() * qs;
					// use parametric equation to represent the ray
					Vector3 ray = unit(qw - eye);
					Color col = resetColor(rayTrace(MAX_RECURSION_DEPTH, eye, ray));// get pixel color
					setPixel(width, height, w, h, col, pixels);
				}
				progress = (h + 1.) / height;
				Fl::check();
			}
		}
		break;
	case RenderingUI::RENDER_PATH_TRACING:  break;
	default: break;
	}
}

void MyScene::stopRender()
{
	// Because this is threaded code, this function
	// can be called in the middle of your rendering code.
	// You should then stop at the next scanline
	stop = true;
	progress = 1;
}

double MyScene::getRenderProgress() {
	// return the current progress as a value between 0 and 1
	return progress;
}

// add extra methods here

// check if the ray hits a object
bool MyScene::rayHitObj(int t1, int t2){
	bool hit = false;
	for (unsigned int i = 0; i < flattenedTree.size(); i++){
		Point3 lightsPos = lights[t2].getPos();
		Point3 objPos = flattenedTree[t1].p;
		Vector3 n = objPos - lightsPos;
		Matrix4 M_inv = flattenedTree[i].inverse;
		Vector3 direction = unit(M_inv * n);
		Point3 obj_p = M_inv * lightsPos;

		if (i == t1){ continue; }
		Shape *objShape = flattenedTree[i].obj->shape;
		// use the intersect function written in intersection assignment
		// recall intersect stores the intersection pts in hr hits (p and d defines the line)
		objShape->intersect(obj_p, direction);
		objShape->hr.sortHits();

		Point3 p;
		Vector3 normal;
		double t, u, v; 
		if (!objShape->hr.getFirstHit(t, u, v, p, normal)){
			continue;
		}
		Point3 newP = flattenedTree[i].trans * p;
		double dist_between_P_and_Light = (newP - lightsPos).length();
		double dist = n.length();
		if (dist_between_P_and_Light < dist){
			hit = true;
		}
	}
	return hit;
}

bool MyScene::intersection(Point3 point, Vector3 dir, int &ind){
	double min_distance = MAX_DIST;
	ind = NULL_INDEX;
	for (unsigned int i = 0; i < flattenedTree.size(); i++){
		Matrix4 M = flattenedTree[i].trans;
		Matrix4 M_inv = flattenedTree[i].inverse;
		Point3 p_obj = M_inv * point;
		Vector3 direction = unit(M_inv * dir);

		Shape *objShape = flattenedTree[i].obj->shape;
		objShape->intersect(p_obj, direction);
		objShape->hr.sortHits();

		double t_, u_, v_;
		t_ = flattenedTree[i].t; u_ = flattenedTree[i].u; v_ = flattenedTree[i].v;

		if (objShape->hr.getFirstHit(t_, u_, v_, flattenedTree[i].p, flattenedTree[i].n)){
			// p_world = M * p_object
			flattenedTree[i].p = M * flattenedTree[i].p;
			// n_world = (M^-1)^T*n_object
			flattenedTree[i].n = unit(M_inv.transpose()*flattenedTree[i].n);
			// t distance
			flattenedTree[i].t = (flattenedTree[i].p - point).length();

			if (flattenedTree[i].t < min_distance){
				min_distance = flattenedTree[i].t;
				ind = i;
			}
		}
	}
	if (ind != -1){
		dir.normalize();
		flattenedTree[ind].theta = acos((-1.*dir)*flattenedTree[ind].n); 
		return true;
	}
	else{
		return false;
	}

}

// compute the color at each intersection 
Color MyScene::computeIllumination(Point3 p, Vector3 dir, int &index){
	//Color illumination;
	// I_amb = I_A * k_a 
	//Color ambientLight = flattenedTree[index].obj->ambient * ambientLight;
	Color illumination = flattenedTree[index].obj->ambient * ambientLight;
	for (int i = 0; i < lights.size(); i++){
		if (rayHitObj(index, i)){ continue; }
		/*  Light Attenuation */
		// dl: distance to light source = Norm(light position - obj position )
		double dl = (lights[i].getPos() - flattenedTree[index].p).length();
		// attenuation coeefficients c0, c1, c2:
		double c0, c1, c2;
		c0 = lights[i].getFalloff()[0];
		c1 = lights[i].getFalloff()[1];
		c2 = lights[i].getFalloff()[2];
		double fatt = 1 / (c0 + c1*dl + c2*dl*dl);

		// diffuse light = I_L * f_att * (k_d * (N * L))
		//L: direction of light
		Vector3 L = unit(lights[i].getPos() - flattenedTree[index].p);
		Vector3 N = flattenedTree[index].n;
		if (L * N < 0){
			continue;
		}

		Color I_L = lights[i].getColor();
		Color k_d = flattenedTree[index].obj->diffuse;
		Color diffuseLight = I_L * fatt *k_d * (N * L);

		/*  Specular light */
		Color specularLight = Color(0, 0, 0);
		Vector3 V = unit(camera.getEye() - flattenedTree[index].p);
		Vector3 R = Matrix4().rotation(flattenedTree[index].n, PI) * L;
		if (R*V > 0.){
			// specular light = I_L * fatt * K_s * (R * V)^n
			double temp = pow(R * V, flattenedTree[i].obj->shine);
			specularLight = I_L * fatt * flattenedTree[index].obj->specular * temp;
		}
		illumination += diffuseLight + specularLight;
	}
	return illumination;
}

// Recursive RayTracer 
Color MyScene::rayTrace(int level, Point3 p, Vector3 dir){
	int index;
	// find nearest intersection of L with all surfaces
	if (intersection(p, dir, index)){	// if there is intersection between ray and object
		if (level == 1){
			// compute local illumination
			return computeIllumination(p, dir, index);
		}
		else{
			Vector3 rotate_axis = flattenedTree[index].n;
			double angle = flattenedTree[index].theta;
			Vector3 dirR = unit(Matrix4().rotation(rotate_axis, angle)*dir);

			if (flattenedTree[index].obj->reflect[0] == 0 &&
				flattenedTree[index].obj->reflect[1] == 0 &&
				flattenedTree[index].obj->reflect[2] == 0){
				return computeIllumination(p, dir, index);
			}
			else{
				// recusively compute the illumination until the level is 1 (nearest)
				return computeIllumination(p, dir, index) + flattenedTree[index].obj->reflect * rayTrace(--level, flattenedTree[index].p, dirR);
			}
		}
	}
	else{
		// if no intersection found 
		return background;
	}
}
