#include "../cse452.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>

# define  PI  M_PI
Camera::Camera()
{
	// initialize your data here
}

Camera::~Camera() {
	// destroy your data here
}

// The following three should be unit length and orthogonal to each other
// u vector
Vector3 Camera::getRight() const
{
	// Change this
	Vector3 n, v;
	n = (-1.) * this->getLook(); //  negative look up * (-1)
	v = this->getUp();
	Vector3 right = v ^ n; // u = v cross n
	right.normalize();
	return right;
}

// v vector
Vector3 Camera::getUp() const
{
	Vector3 n = (-1.) * this->getLook();
	n.normalize();
	Vector3 v = upVector - n * (upVector * n);
	v.normalize();
	return v;
}

// - n vector
Vector3 Camera::getLook() const
{
	return this->lookVector;
}

double Camera::getSkew() const
{
	// Change this to implement the extra credit
	return skew;
}

double Camera::getAspectRatioScale() const
{
	// Change this to implement the extra credit
	return scale;
}

Point3 Camera::getProjectionCenter() const
{
	// Change this to implement the extra credit
	return center;
}

Matrix4 Camera::getProjection() const {
	// return the current projection and scale matrix
	
	double q_w = 2 * atan(tan(PI * zoom / 360.0)) * (1. * width / height);
	double q_h = PI * zoom / 180.;
	
	double k = dn/ df;

	Matrix4 S_xy = Matrix4(
		Vector4(1. / tan(q_w / 2.), 0., 0., 0.),
		Vector4(0., 1. / tan(q_h / 2.), 0., 0.),
		Vector4(0., 0., 1., 0.),
		Vector4(0., 0., 0., 1.));
	Matrix4 S_xyz = Matrix4(
		Vector4(1/df, 0., 0., 0.), 
		Vector4(0., 1/df, 0., 0.), 
		Vector4(0., 0., 1/df, 0.), 
		Vector4(0., 0., 0., 1.));
	Matrix4 D = Matrix4(
		Vector4(1., 0., 0., 0.), 
		Vector4(0., 1., 0., 0.), 
		Vector4(0., 0., 1 / (k - 1), k / (k - 1)), 
		Vector4(0., 0., -1., 0.));
	return D * S_xyz * S_xy;
}

Matrix4 Camera::getWorldToCamera() const {
	// return the current world to camera matrix
	// Rotation and translation
	Matrix4 T = Matrix4(
		Vector4(1., 0., 0., -from[0]), 
		Vector4(0., 1., 0., -from[1]), 
		Vector4(0., 0., 1., -from[2]), 
		Vector4(0., 0., 0., 1.));
	return this->getRotationToXYZ() * T;
}

Matrix4 Camera::getRotationFromXYZ() const
{
	// return just the rotation matrix
	// R = M^-1  since M is orthonormal, R = M^T
	Matrix4 R = this->getRotationToXYZ().transpose();
	return R;
}

// rotate the three axes u, v, n to X, Y, Z
Matrix4 Camera::getRotationToXYZ() const
{
	// return just the rotation matrix u, v, n
	Matrix4 R = Matrix4(this->getRight(), this->getUp(), (-1.)*this->getLook());
	return R;
}

Matrix4 Camera::getCameraToWorld() const {
	// return the current camera to world matrix
	// This is the inverse of the rotation, translation, and scale
	double ratio = width / (double)height;

	// translation matrix
	Matrix4 T = Matrix4(
		Vector4(1., 0., 0., from[0]), 
		Vector4(0., 1., 0., from[1]), 
		Vector4(0., 0., 1., from[2]), 
		Vector4(0., 0., 0., 1.));

	// scale matrix
	Matrix4 S_xy = Matrix4(
		Vector4(ratio * tan(PI * zoom / 360.), 0., 0., 0.),
		Vector4(0., tan(PI * zoom / 360.), 0., 0.),
		Vector4(0., 0., 1., 0.),
		Vector4(0., 0., 0., 1.));

	// push the far plane from z = -df to z = -1
	Matrix4 S_xyz = Matrix4(
		Vector4(df, 0., 0., 0.), 
		Vector4(0., df, 0., 0.),
		Vector4(0., 0., df, 0.), 
		Vector4(0., 0., 0., 1.));

	return T * getRotationFromXYZ() * S_xy * S_xyz;
}

int Camera::getWidth()  const{
	// return the current image width
	return this->width;
}

int Camera::getHeight()  const{
	// return the current image height
	return this->height;
}

Point3 Camera::getEye()  const{
	// return the current eye location
	return this->from;
}

double Camera::getZoom() const
{
	return this->zoom;
}

void Camera::setFrom(const Point3& from) {
	// set the current viewpoint (eye point)
	this->from = from;
}

void Camera::setAt(const Point3& at) {
	// set the point the camera is looking at
	// calling this requires that the from (or eye) point already be valid
	this->at = at;
	setLook(at - from);
}

void Camera::setLook(const Vector3& l) {
	// set the direction the camera is looking at
	this->lookVector = l;
	lookVector.normalize();
}

void Camera::setUp(const Vector3& up) {
	// set the upwards direction of the camera
	this->upVector = up;
	upVector.normalize();
}

void Camera::setWidthHeight(int w, int h) {
	// set the current width and height of the film plane
	this->width = w;
	this->height = h;
}

void Camera::setZoom(double z) {
	// set field of view (specified in degrees)
	this->zoom = z;
}

void Camera::setNearFar(double n, double f) {
	// set the near and far clipping planes
	this->dn = n;
	this->df = f;
}

void Camera::setSkew(double d)
{
	skew = d;
}

void Camera::setAspectRatioScale(double d)
{
	scale = d;
}

void Camera::setProjectionCenter(const Point3 &p)
{
	center = p;
}

void Camera::moveForward(double dist) {
	// move the camera forward (in the viewing direction)
	// by the amount dist
	Vector3 move = getLook();
	move *= dist;
	setFrom(from + move);
}

void Camera::moveSideways(double dist) {
	// move the camera sideways (orthogonal to the viewing direction)
	// by the amount dist
	Vector3 move = getRight();
	move *= dist;
	setFrom(from + move);
}

void Camera::moveVertical(double dist) {
	// move the camera vertically (along the up vector)
	// by the amount dist
	Vector3 move = getUp();
	move *= dist;
	setFrom(from + move);
}

void Camera::rotateYaw(double angle) {
	// rotate the camera left/right (around the up vector)
	Vector3 up = getUp();
	Matrix4 rotate = Matrix4::rotation(up, angle);
	lookVector = rotate * lookVector;
}

void Camera::rotatePitch(double angle) {
	// rotate the camera up/down (pitch angle)
	Vector3 right = getRight();
	Matrix4 rotate = Matrix4::rotation(right, angle);
	lookVector = rotate * lookVector;
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
	// Rotate the camera around the right (0), up (1), or look (2) vector
	// around the point at eye + look * focusDist
	Matrix4 rotate = Matrix4::identity();
	switch ( axis ){
		case RIGHT: {
			Vector3 axis_now = getRight();
			rotate = Matrix4::rotation(axis_now, angle);
		}break;
		case UP: {
			Vector3 axis_now = getUp();
			rotate = Matrix4::rotation(axis_now, angle);
		}break;
		case LOOK: {
			Vector3 axis_now = getLook();
			rotate = Matrix4::rotation(axis_now, angle);
		}break;
		default: break;
	}
	Point3 eye = getEye() + focusDist* getLook();
	Vector3 look = eye - from;
	setLook(rotate * look);
	setUp(rotate * upVector);
	setFrom(eye + getLook()*(-focusDist));
}

void Camera::moveKeyboard()
{
	// you may change key controls for the interactive
	// camera controls here, make sure you document your changes
	// in your README file

	if (Fl::event_key('w'))
		moveForward(+0.05);
	if (Fl::event_key('s'))
		moveForward(-0.05);
	if (Fl::event_key('a'))
		moveSideways(-0.05);
	if (Fl::event_key('d'))
		moveSideways(+0.05);
	if (Fl::event_key(FL_Up))
		moveVertical(+0.05);
	if (Fl::event_key(FL_Down))
		moveVertical(-0.05);
	if (Fl::event_key(FL_Left))
		rotateYaw(+0.05);
	if (Fl::event_key(FL_Right))
		rotateYaw(-0.05);
	if (Fl::event_key(FL_Page_Up))
		rotatePitch(+0.05);
	if (Fl::event_key(FL_Page_Down))
		rotatePitch(-0.05);
}