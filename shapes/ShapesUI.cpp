#include "../cse452.h"
#include "ShapesUI.h"
#include "ShapesInterface.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "MyShape.h"
#include <iostream>

using namespace std;

// global variables initialization
Cube cube;
Cylinder cylinder;
Cone cone;
Sphere sphere;

ShapesUI::ShapesUI() {
    width = height = 0;
    // ToDo: initialize your variables here
}

ShapesUI::~ShapesUI() {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    // ToDo: delete your variables here
}

void ShapesUI::resize(int w, int h) {
    width = w;
    height = h;
}

void ShapesUI::draw() {
    // Sets up the viewport and background color
    setup3DDrawing( Color( 0,0,0 ), width, height, true );

    // Changes the way triangles are drawn
    switch ( shapesUI->getDisplayType() ) {
        case DISPLAY_WIREFRAME: {
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3f(1.0f, 1.0f, 1.0f);
        } break;
        case DISPLAY_FLAT_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_FLAT);
        } break;
        case DISPLAY_SMOOTH_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_SMOOTH);
        } break;
        default: break;
    }

    // Setup the camera
    gluLookAt( 3.5 * cos( shapesUI->getYRot() ) * cos( shapesUI->getXRot() ), 
               3.5 * sin( shapesUI->getYRot() ), 
               3.5 * cos( shapesUI->getYRot() ) * sin( shapesUI->getXRot() ), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // ToDo: draw your shape here
    // DO NOT put the actual draw OpenGL code here - put it in the shape class and call the draw method
	switch (shapesUI->getShapeType()){
		case SHAPE_CUBE:{
			if (shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
				cube.drawShapeWireFrame();
			}			
			if (shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
				cube.drawShapeFlat();
			}
			if (shapesUI->getDisplayType() == DISPLAY_SMOOTH_SHADING){
				cube.drawShapeSmooth();
			}
		}break;
		case SHAPE_CONE:{
			if (shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
				cone.drawShapeWireFrame();
			}	
			if (shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
				cone.drawShapeFlat();
			}
			if (shapesUI->getDisplayType() == DISPLAY_SMOOTH_SHADING){
				cone.drawShapeSmooth();
			}
		}break;
		case SHAPE_CYLINDER:{
			if (shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
				cylinder.drawShapeWireFrame();
			}
			if (shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
				cylinder.drawShapeFlat();
			}
			if (shapesUI->getDisplayType() == DISPLAY_SMOOTH_SHADING){
				cylinder.drawShapeSmooth();
			}
		}break;
		case SHAPE_SPHERE:{
			if (shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
				sphere.drawShapeWireFrame();
			}
			if (shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
				sphere.drawShapeFlat();
			}
			if (shapesUI->getDisplayType() == DISPLAY_SMOOTH_SHADING){
				sphere.drawShapeSmooth();
			}

		}break;
	}
    endDrawing();
}

int ShapesUI::handle(int event) {
    return 0;
}

void ShapesUI::changedShape()
{
    // ToDo: Switch shapes
	int n, m;
	n = shapesUI->getTessel1();
	m = shapesUI->getTessel2();

	switch (shapesUI->getShapeType()){
	case SHAPE_CUBE:
		cube.computeTriangle(n);
		break;
	case SHAPE_CONE:
		cone.computeTriangle(n, m);
		break;
	case SHAPE_CYLINDER:
		cylinder.computeTriangle(n, m);
		break;
	case SHAPE_SPHERE:
		sphere.computeTriangle(n);
		break;
	}
    RedrawWindow();
}

void ShapesUI::changedTessel( ) {
    // ToDo: tessellate your shape here
	int n, m;
	n = shapesUI->getTessel1();
	m = shapesUI->getTessel2();

	switch (shapesUI->getShapeType()){
	case SHAPE_CUBE:
		cube.computeTriangle(n);
		break;
	case SHAPE_CONE:
		cone.computeTriangle(n, m);
		break;
	case SHAPE_CYLINDER:
		cylinder.computeTriangle(n, m);
		break;
	case SHAPE_SPHERE:
		sphere.computeTriangle(n);
		break;
	}    
    RedrawWindow();
}