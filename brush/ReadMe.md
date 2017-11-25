ReadMe

# Assignment 1: AirBrush

### Wenzhen Zhu
### Date: 02/10/2016, Wednesday, Snowy

## Part I: Brush

+ Buffer
	+ In MyBrush.h, I initialized vector<vector<float>> buffer. 
	+ void makeBuffer(int radius, int brush_type)
		+ I check the brush type first, and for each brush type, I push back the corresponding distribution value.
		+ I also made 6 distribution function:
			+ constant distribution
			+ linear distribution
			+ quadratic distribution
			+ gaussian distribution
			+ special distribution (gives the donut-like brush)
			+ rayleigh distribution

+ changeBrush()
	+ every time chnage brush, the buffer is clear and then refilled
+ drawBrush()
	+ I limited the drawing in the region from(0,0) to (screenWidth, screenHeight) to prevent drawing out of the canvas
	+ then I traverse the region and putPixel on each point with the C = MFC_B + (1-F*M)*C_C where M is the distribution stored in buffer

+ drawBrush(int x, int y, Color colBrush)
	+ I created this function very similar to the drawBrush, I use this to draw circle later


## Part II: Line

+ First, I check if the slope exists or not. 
+ When slope exist, I can use the scan-line algorithm to fill the region
+ findLineIntersect(Point2D p1, float slope)
	+ given (x, y) and slope m, we can find b by y = m * x + b, 
	+ returns b, the intersection with y - axis
+ drawLine2(ScreenPoint mouseDown, ScreenPoint mouseDrag, Color ColBrush, int radius) 
	+ First, I use mouseDown and mouseDrag to compute the coordinates of p1, p2, p3, p4
	+ Second, compute b1 and b2 for the two line function
	+ Third, apply scan-line algorithm to fill the pixels between y1 and y2

## Part III: Circle

+ midpointEiththCircle_1(Point2D center, float R)
	+ I tried the algorithm in slides, and it works as expected, which gives 1/8 circle
+ midPointEightCircle(Point 2D center, float R, int xSign, int ySign, int xyMix)
	+ We can use symmetry to complete the full circle by switching the signs of x, y, and xyMix
+ drawCircle()
	I made a nested for loops with 3 levels to make 2^3 1/8 circle

## Part IV: Other struct and methods

+ data structures 
	struct Point2D{ int x_, int y_};
	struct FloatPoint2D { float x_, float y_};

	> I used those to store point

+ functions
	+ float p2distance(Point2D p1, Point2D p2);
		+ returns euclidean distance between point p1 and p2;
	float distance(int x, int y);
		+ used in make buffer to compute d
	float norm(float x, float y);
		+ used in drawLine to normalize the two direction, since the directions have to be unit vectors.

## Part V: Analysis of complexity of drawing routine

+ DrawMask
	+ O(n^2) where n = radius
+ DrawCircle
	+ O(n)
+ DrawLine
	+ O(n^2)

## Part VI: Extra Credits
+ Gaussian distribution 
	+ float mu = 1, sigma = 0.75;
	+ float(exp(-(distance / radius - mu) / (2 * sigma * sigma)) / sqrt(2 * M_PI));

+ Special Distribution
	+ I accidently found if I reverse d/r (make it r/d) the gaussian distribution gives me a donut shaped brush
	+ float mu = 1, sigma = 0.75;
	+ return float(exp(-(radius / distance - mu) / (2 * sigma * sigma)) / sqrt(2 * M_PI));
+ Rayleigh distribution 
	float sigma = 0.75;
	float r = radius / distance;
	float(exp(-(pow(r, 2.0) / (2 * sigma * sigma)) / (2 * sigma * sigma)));

+ in summary, it's really easy. 
	Given a probability density function pdf(x), substitute x by either d/r or r/d
