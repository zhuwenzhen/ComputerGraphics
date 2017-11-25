#include "cse452.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>
#include <algorithm>

void MyBrush::changedBrush() {
	// this is called anytime the brush type or brush radius changes
	// it should recompute the brush mask appropriately
	const int radius = brushUI->getRadius();
	const int brush_type = brushUI->getBrushType();
	makeBuffer(radius, brush_type);
}

void MyBrush::drawBrush() {
	// apply the current brush mask to image location (x,y)
	// the mouse location is in mouseDrag
	const int radius = brushUI->getRadius();
	const float pixelFlow = brushUI->getFlow();
	const Color colBrush = brushUI->getColor();
	const int brush_type = brushUI->getBrushType();

	// make the region
	int x0 = max(mouseDrag[0] - radius, 0);
	int xt = min(screenWidth, mouseDrag[0] + radius);
	int y0 = max(mouseDrag[1] - radius, 0);
	int yt = min(screenHeight, mouseDrag[1] + radius);

	// traverse the region and create brush on it
	int a, b;
	float M;
	for (int i = x0; i < xt; i++){
		for (int j = y0; j < yt; j++){
			// use symmetry to draw
			a = abs(i - mouseDrag[0]);
			b = abs(j - mouseDrag[1]);
			M = buffer[a][b];
			Color color_canvas = getPixel(i, j);
			putPixel(i, j, (M  * pixelFlow * colBrush + (1 - pixelFlow * M)  * color_canvas));
		}
	}
}

void MyBrush::drawBrush(int x, int y, Color colBrush) {
	// apply the current brush mask to image location (x,y)
	// the mouse location is in mouseDrag
	const int radius = brushUI->getRadius();
	const float pixelFlow = brushUI->getFlow();

	const int brush_type = brushUI->getBrushType();

	// make the region
	int x0 = max(x - radius, 0);
	int xt = min(screenWidth, x + radius);
	int y0 = max(y - radius, 0);
	int yt = min(screenHeight, y + radius);

	// traverse the region and create brush on it
	int a, b;
	float M;
	for (int i = x0; i < xt; i++){
		for (int j = y0; j < yt; j++){
			// use symmetry to draw
			a = abs(i - x);
			b = abs(j - y);
			M = buffer[a][b];
			Color color_canvas = getPixel(i, j);
			putPixel(i, j, (M  * pixelFlow * colBrush + (1 - pixelFlow * M)  * color_canvas));
		}
	}
}

void MyBrush::drawLine() {
	// draw a thick line from mouseDown to mouseDrag
	// the width of the line is given by the current brush radius
	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();

	int x0 = mouseDown[0];
	int y0 = mouseDown[1];
	int x1 = mouseDrag[0];
	int y1 = mouseDrag[1];
	
	//midpointLine(x0, y0, x1, y1, colBrush);
	if (x0 != x1 && y0 != y1){ // slope exsits 
		drawLine2(mouseDown, mouseDrag, colBrush, radius);
	}
	else if (x0 == x1 && y0 != y1){ // vertical line
		for (int i = min(y0, y1); i < max(y0, y1); i++){
			for (int j = (x0 - round(radius / 2)); j <= (x0 + round(radius / 2)); j++){
				putPixel(j, i, colBrush);
			}		
		}
	}
	else if (x0 != x1 && y0 == y1){// horizontal line
		for (int i = min(x0, x1); i < max(x0, x1); i++){
			for (int j = (y0 - round(radius / 2)); j <= (y0 + round(radius / 2)); j++){
				putPixel(i, j, colBrush);
			}
		}
	}
	else if (x0 == x1 && y0 == y1){// draw a square :)
		for (int i = (x0 - round(radius / 2)); i <= (x0 + round(radius / 2)); i++){
			for (int j = (y0 - round(radius / 2)); j <= (y0 + round(radius / 2)); j++){
				putPixel(i, j, colBrush);
			}
		}
	}
}

void MyBrush::midpointLine(int x0, int y0, int x1, int y1, Color colBrush){
	int dx = x1 - x0, dy = y1 - y0;
	int d = 2 * dy - dx;
	int incrE = 2 * dy;
	int incrNE = 2 * (dy - dx);
	int x = x0, y = y0;
	drawBrush(x, y, colBrush);

	while (x < x1){
		if (d <= 0){
			d = d + incrE;
		}
		else{
			d = d + incrNE;
			++y;
		}
		++x;
		drawBrush(x, y, colBrush);
	}
}

void MyBrush::drawLine2(ScreenPoint mouseDown, ScreenPoint mouseDrag, Color colBrush, const int radius){
	int x0 = max(mouseDown[0], 0);
	int xt = min(screenWidth, mouseDrag[0]);
	int y0 = max(mouseDown[1], 0);
	int yt = min(screenHeight, mouseDrag[1]);
	//cout << "x: (" << x0 << ", " << y0 << ")" << endl;
	//cout << "y: (" << xt << ", " << yt << ")" << endl;

	float slope = float((yt - y0) / (xt - x0));
	float slope_inverse = 1 / slope;
	float r = float(radius / 2);
	
	// 1. given 2 points, find the 4 pts to bound the line region
	FloatPt2D dir1, dir2;
	float norm_value = norm(y0 - yt, x0 - xt);
	dir1.x_ = float(y0 - yt) / norm_value; 
	dir1.y_ = float(xt - x0) / norm_value;
	dir2.x_ = float(yt - y0) / norm_value;
	dir2.y_ = float(x0 - xt) / norm_value;


	Point2D p1, p2, p3, p4;
	p1.x_ = round(x0 + r * dir1.x_); p1.y_ = round(y0 + r * dir1.y_);
	p2.x_ = round(x0 + r * dir2.x_); p2.y_ = round(y0 + r * dir2.y_);
	p3.x_ = round(xt + r * dir1.x_); p3.y_ = round(yt + r * dir1.y_);
	p4.x_ = round(xt + r * dir2.x_); p4.y_ = round(yt + r * dir2.y_);

	vector<int> x_pixel, y_pixel;
	x_pixel = { p1.x_, p2.x_, p3.x_, p4.x_ };
	y_pixel = { p1.y_, p2.y_, p3.y_, p4.y_ };
	// Checked 4 pts, looks correct

	/*cout << "p1: (" << p1.x_ << ", " << p1.y_ << ")" << endl;
	cout << "p2: (" << p2.x_ << ", " << p2.y_ << ")" << endl;
	cout << "p3: (" << p3.x_ << ", " << p3.y_ << ")" << endl;
	cout << "p4: (" << p4.x_ << ", " << p4.y_ << ")" << endl;*/

	// 2. scan from xmin to xmax, fill the region(line) between y1 and y2
	//	  y1 = m xi + b1 ; y2 = m xi + b2
	float b1 = findLineIntersect(p1, slope);
	float b2 = findLineIntersect(p2, slope);

	auto result_x = minmax_element(x_pixel.begin(), x_pixel.end());
	int xMin_pos = result_x.first - x_pixel.begin();
	int xMax_pos = result_x.second - x_pixel.begin();
	float xMin = x_pixel[xMin_pos];
	float xMax = x_pixel[xMax_pos];

	for (int i = xMin; i <= xMax; i++){
		int y1 = round(slope * i + b1);
		int y2 = round(slope * i + b2);
		for (int j = min(y1, y2); j <= max(y1, y2); j++){
			putPixel(i, j, colBrush);
		}
	}
	
}

float MyBrush::findLineIntersect(Point2D p, float slope){
	return (p.y_ - p.x_ * slope);
}

float MyBrush::norm(float x, float y){
	return float(sqrt(x*x + y*y)); 
}

void MyBrush::drawCircle() {
	// draw a thick circle at mouseDown with radius r
	// the width of the circle is given by the current brush radius
	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();
	Point2D p1, p2;
	p1.x_ = mouseDown[0]; p1.y_ = mouseDown[1];
	p2.x_ = mouseDrag[0]; p2.y_ = mouseDrag[1];
	float radius_circle = p2distance(p1, p2);
	//cout << "R = " << radius_circle << endl;

	//midpointEigthCircle_1(p1, radius_circle);
	for (int i = -1; i <= 1; i = i + 2){
		for (int j = -1; j <= 1; j = j + 2){
			for (int k = 0; k <= 1; k++){
				//cout << "(" << i << ", " << j << ", " << k << ")" << endl;
				midpointEigthCircle(p1, radius_circle, i, j, k);
			}
		}
	}
}

void MyBrush::midpointEigthCircle_1(Point2D center, float R){
	const Color colBrush = brushUI->getColor();
	const int radius = brushUI->getRadius();
	float x = 0; 	
	float y = R;
	//cout << "x = " << x << ", y = " << y << endl;
	float decision = float((x + 1) * (x + 1) + (y - 0.5) * (y - 0.5) - R * R);
	float deltaE = 2 * x + 3;
	float deltaSE = 2 * (x - y) + 5;
	putPixel(x + center.x_, y + center.y_, colBrush);
	while (y  > x){
		if (decision < 0){
			x ++;
			putPixel(x + center.x_, y + center.y_, colBrush);
			decision += deltaE;
			deltaE += 2;
			deltaSE += 2;
		} 
		else {
			y = y - 1;
			x = x + 1;		
			putPixel(x + center.x_, y + center.y_, colBrush);
			decision += deltaSE;
			deltaE += 2;
			deltaSE += 4;
		}
	}
}

void MyBrush::midpointEigthCircle(Point2D center, float R, int xSign, int ySign, int xyMix){
	const Color colBrush = brushUI->getColor();
	const int radius = brushUI->getRadius();
	float x = 0;
	float y = R * ySign; // check
	float decision = float((x * xSign + 1) * (x * xSign + 1) + (y * ySign - 0.5) * (y * ySign - 0.5) - R * R);
	float deltaE = 2 * x * xSign + 3;
	float deltaSE = 2 * (x * xSign - y * ySign) + 5;
	putPixel(x + center.x_, y + center.y_, colBrush);
	while (y * ySign > x * xSign){
		if (decision < 0){
			x = x + xSign; // check
			if (xyMix){
				//putPixel(y + center.x_, x + center.y_, colBrush);
				drawBrush(y + center.x_, x + center.y_, colBrush);
			}
			else{
				//putPixel(x + center.x_, y + center.y_, colBrush);
				drawBrush(x + center.x_, y + center.y_, colBrush);
			}
			decision += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else {
			y = y - ySign;
			x = x + xSign;
			if (xyMix){
				//putPixel(y + center.x_, x + center.y_, colBrush);
				drawBrush(y + center.x_, x + center.y_, colBrush);
			}
			else{
				//putPixel(x + center.x_, y + center.y_, colBrush);
				drawBrush(x + center.x_, y + center.y_, colBrush);
			}
			decision += deltaSE;
			deltaE += 2;
			deltaSE += 4;
		}
	}
}

void MyBrush::drawPolygon() {
	// draw a polygon with numVertices whos coordinates are stored in the
	// polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
	const Color colBrush = brushUI->getColor();
	// 1. find out the ymin and ymas from the given polygon
	vector<float> y_vec;
	for (vector<ScreenPoint>::const_iterator it = polygon.begin(); it != polygon.end(); it++){
		ScreenPoint	pt= *it;
		y_vec.push_back(pt[1]);
		//cout << "(" << pt[0] << ", " << pt[1] << ")" << endl;
	}
	auto result = minmax_element(y_vec.begin(), y_vec.end());
	int yMin_pos = result.first - y_vec.begin();
	int yMax_pos = result.second - y_vec.begin();
	float yMin = y_vec[yMin_pos];
	float yMax = y_vec[yMax_pos];// check, works perfectly
	//cout << yMax << " " << yMin << endl;

	// 2. ScanLine intersects with each edge of the polygon from Ymin to Ymax.
	//		Name each intersection point of the polygon


}

void MyBrush::filterRegion() {
	// apply the filter indicated by filterType to the square
	// defined by the two corner points mouseDown and mouseDrag
	// these corners are not guarenteed to be in any order
	// The filter width is given by the brush radius
}

// 1. make Buffer 
float MyBrush::constantDistribution(float distance, int radius){
	return float(1);
}
float MyBrush::linearDistribution(float distance, int radius){
	return float((radius - distance) / radius);
}
float MyBrush::quadraticDistribution(float distance, int radius){
	return float(1 - pow((distance / radius), 2.0));
}
float MyBrush::gaussianDistribution(float distance, int radius){
	float mu = 1, sigma = 0.75;
	return float(exp(-(distance / radius - mu) / (2 * sigma * sigma)) / sqrt(2 * M_PI));
}
float MyBrush::specialDistribution(float distance, int radius){
	float mu = 1, sigma = 0.75;
	return float(exp(-(radius / distance - mu) / (2 * sigma * sigma)) / sqrt(2 * M_PI));
}
float MyBrush::rayleighDistribution(float distance, int radius){
	// RayleighDistribution
	float sigma = 0.75;
	float r = radius / distance;
	return float(exp(-(pow(r, 2.0) / (2 * sigma * sigma)) / (2 * sigma * sigma)));
}
float MyBrush::distance(int x, int y){
	double x_ = pow(x, 2.0);
	double y_ = pow(y, 2.0);
	return float(sqrt(x_ + y_));
}

float MyBrush::p2distance(Point2D p1, Point2D p2){
	float dx = float(pow(p1.x_ - p2.x_, 2.0));
	float dy = float(pow(p1.y_ - p2.y_, 2.0));
	return float(sqrt(dx + dy));
}

void MyBrush::makeBuffer(int radius, int brush_type){
	buffer.clear();
	if (brush_type == BRUSH_CONSTANT){
		for (int x = 0; x <= radius; x++){
			vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(constantDistribution(input, radius));
				}
			}
			buffer.push_back(temp);
		}
		return;
	}
	// 1. linear
	else if (brush_type == BRUSH_LINEAR){
		for (int x = 0; x <= radius; x++){
			vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(linearDistribution(input, radius));
				}
			}
			buffer.push_back(temp);
		}
		return;
	}
	// 2. quadratic
	if (brush_type == BRUSH_QUADRATIC){
		for (int x = 0; x <= radius; x++){
			vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(quadraticDistribution(input, radius));
				}
			}
			buffer.push_back(temp);
		}
		return;
	}
	// 3. guassian
	if (brush_type == BRUSH_GAUSSIAN){
		for (int x = 0; x <= radius; x++){
			vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(gaussianDistribution(input, radius));
				}
			}
			buffer.push_back(temp);
		}
		return;
	}
	// 4. special (dounut)
	if (brush_type == BRUSH_SPECIAL){
		for (int x = 0; x <= radius; x++){
			vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(specialDistribution(input, radius));
				}
			}
			buffer.push_back(temp);
		}
		return;
	}
	// 5. special (Rayleigh)
	if (brush_type == BRUSH_RAYLEIGH){
		for (int x = 0; x <= radius; x++){
			vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(rayleighDistribution(input, radius));
				}
			}
			buffer.push_back(temp);
		}
		return;
	}
}