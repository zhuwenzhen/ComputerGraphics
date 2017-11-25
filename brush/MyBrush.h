#ifndef _MY_BRUSH_H_
#define _MY_BRUSH_H_

#include "cse452.h"
#include "ScreenPoint.h"
#include "UIInterface.h"
#include <vector>

using namespace std;

class BrushInterface;
class MyBrush : public UIInterface {
public:
    enum ToolType {
        TOOL_BRUSH = 0,
        TOOL_LINE,
        TOOL_CIRCLE,
        TOOL_POLYGON,
        TOOL_FILTER
    };

    enum FilterType {
        FILTER_BLUR = 0,
        FILTER_SHARPEN,
        FILTER_EDGE
    };
    
    enum BrushType {
        BRUSH_CONSTANT = 0,
        BRUSH_LINEAR,
        BRUSH_QUADRATIC,
        BRUSH_GAUSSIAN,
        BRUSH_SPECIAL,
		BRUSH_RAYLEIGH
    };
	struct Point2D{
		int x_;
		int y_;
	};

	struct FloatPt2D{
		float x_;
		float y_;
	};
	struct pixel{
		ScreenPoint p;
		float pixelFlow;
	};

    MyBrush();
    ~MyBrush();

    // Link to the brush widget
    void setUI( const BrushInterface *in_ui ) { brushUI = in_ui; }

    // Inherited from UIInterface
    // Implemented in MyBrush_UI.cpp
    virtual void resize( int width, int height );
    virtual void draw();
    virtual int  handle( int event );

    void loadImage(Fl_Image* image);
    
    // Called by the UI when the brush type or radius changes
    void changedBrush();

private:
    // read a pixel from the canvas, returns (r, g, b) values in the [0,1] range
    Color getPixel(int x, int y );
    // put a pixel on the canvas, (r, g, b) are to be given in the [0,1] range
    void putPixel(int x, int y, const Color &in_col);

    // Pixel and screen data
    std::vector<unsigned char> pixelData;
    int screenWidth, screenHeight;
    int imageWidth, imageHeight;

    // For interactive drawing, set by handle
    // You do not need to set these; the interface for that is
    // already set up
    bool isMouseDown;
    ScreenPoint mouseDown, mouseDrag;
    std::vector< ScreenPoint > polygon;
    static void draw_callback( void *in_data );

    // All of the widget data can be accessed through
    // methods on this class
    const BrushInterface *brushUI;

    // You need to write these
    // Stubs are in MyBrush.cpp
    void drawBrush( );
    void drawLine( );

    void drawCircle( );
    void drawPolygon( );
    void filterRegion( );

    // declare your extra variables/methods here:

	// draw brush:
	void drawBrush(int x, int y, Color colBrush);
	vector<vector<float>> buffer;
	
	float constantDistribution(float distance, int radius);
	float linearDistribution(float distance, int radius);
	float quadraticDistribution(float distance, int radius);
	float gaussianDistribution(float distance, int radius);
	float specialDistribution(float distance, int radius);
	float rayleighDistribution(float distance, int radius);

	float distance(int x, int y);
	void makeBuffer(int r, int brush_type);

	// draw circle
	float p2distance(Point2D p1, Point2D p2);
	void midpointEigthCircle_1(Point2D center, float R);
	void midpointEigthCircle(Point2D center, float R, int xSign, int ySign, int xyMix);

	// draw line
	void midpointLine(int x0, int y0, int x1, int y1, Color colBrush);
	void drawLine2(ScreenPoint mouseDown, ScreenPoint mouseDrag, Color colBrush, const int radius);
	float norm(float x, float y);
	float findLineIntersect(Point2D p, float slope);

	// draw Polygon


};

inline
Color MyBrush::getPixel( int x, int y ) {
    Color col;
    const int i = (y * imageWidth + x) * 3;
    col[0] = pixelData[i    ] / 255.0f;
    col[1] = pixelData[i + 1] / 255.0f;
    col[2] = pixelData[i + 2] / 255.0f;

    return col;
}

inline
void MyBrush::putPixel(int x, int y, const Color & col) {
	if (x < 0 || y < 0 || x >= imageWidth || y >= imageHeight){
		
	}
	else{
		const int i = (y * imageWidth + x) * 3;
		pixelData[i] = (unsigned char)(col[0] * 255.0f);
		pixelData[i + 1] = (unsigned char)(col[1] * 255.0f);
		pixelData[i + 2] = (unsigned char)(col[2] * 255.0f);
	}
}


#endif /* _MY_BRUSH_H_ */
