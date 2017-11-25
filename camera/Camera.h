#ifndef _MY_CAMERA_H_
#define _MY_CAMERA_H_

#include "cse452.h"
#include "Matrix4.h"

// you must implement all of the following methods

class Camera {
public:
    Camera();
    ~Camera();
    
    // Perspective plus scale (x,y, and z)
    Matrix4 getProjection() const;

    // Rotation and translation from world to camera
    Matrix4 getWorldToCamera() const;
    // Rotation, translation and scale from camera to world
    Matrix4 getCameraToWorld() const;
    // Just rotation from x,y,z axes to u,v,n
    Matrix4 getRotationFromXYZ() const;
    // Just rotation from u,v,n to x,y,z axes (canonical)
    Matrix4 getRotationToXYZ() const;

    // screen width/height
    int getWidth() const;
    int getHeight() const;

    // Camera orientation, position _after_ normalization
    Point3  getEye() const;
    // These should be unit length and orthogonal to each other
    // u vector
    Vector3 getRight() const;
    // v vector
    Vector3 getUp() const;
    // -n vector
    Vector3 getLook() const;

    // Perspective data
    Point3 getProjectionCenter() const;
    double getZoom() const;
    double getSkew() const;
    // "Squishing" in x,y (not Width/Heigh)
    double getAspectRatioScale() const;

    // For setting camera
    void setFrom(const Point3& from);
    void setAt(const Point3& at);
    void setLook(const Vector3& l);
    void setUp(const Vector3& up);
    void setWidthHeight(int w, int h);
    void setZoom(double z);
    void setNearFar(double n, double f);
    void setProjectionCenter( const Point3 &in_pt );
    // Extra "squishing" in x,y (not Width/Height)
    void setAspectRatioScale( double );
    void setSkew( double );

    // This is what gets called when a key is pressed
    void moveKeyboard();

    // user interaction methods
    // These rotate the camera around itself
    void moveForward(double dist);
    void moveSideways(double dist);
    void moveVertical(double dist);
    void rotateYaw(double angle);
    void rotatePitch(double angle);

    // IBar: This rotates the camera around a point at a distance focusDist
    // from the eye point of the camera. If axis is 0, it rotates the camera itself
    // around the Right vector, 1 is around the up axis, and 2 is around the look vector
    void rotateAroundAtPoint(int axis, double angle, double focusDist);

private:
    // declare your variables here:

	// Rotate the camera around the right (0), up (1), or look (2) vector
	enum{	
		RIGHT = 0,
		UP,
		LOOK
	}cameraRotate;

	int width;
	int height;

	double zoom;

	double dn;
	double df;

	Point3 from;
	Point3 at;

	Vector3 upVector;
	Vector3 lookVector;

	// extra 
	double skew;
	double scale;
	Point3 center;

	friend class MyScene;
};

#endif /* _MY_CAMERA_H_ */
