#ifndef QT_H
#define QT_H

#include <vector>

//Representation of a point
struct Point
{
	double x, y; //Coordinates

	Point();
	Point(const double, const double); //Constructor
};

//Axis-aligned bounding box
struct BoundingBox
{
	Point center, halfDim;

	BoundingBox();
	BoundingBox(const Point, const Point); //(center, half dimension)
	bool contains(const Point&); //Function to check if a point is found within this object.
	bool intersects(const BoundingBox&); //Examines if this BB intersects with the argument
};

class QTree
{
public:
	static int nMain;
	static int nChildren;
	QTree(const double, const double, const double); //User constructor
	bool insert(const Point&);
	void querySquare(const double, const double, const double, std::vector<Point*>&);
	void queryCircle(const double, const double, const double, std::vector<Point*>&);


private:
	//How many elements can be stored in one node
	const int QT_NODE_CAPACITY = 4;
	const int QT_MAX_DEPTH = 6;

	//Value keeping track of the depth of this node
	int depth;
	//Axis-aligned bounding box representing the center and boundaries of this quad.
	BoundingBox boundary;
	//Vector to store data (will only be used at deepest level)
	std::vector<Point> points;
	//Children
	QTree* nE;
	QTree* nW;
	QTree* sE;
	QTree* sW;

	//Constructor with parents depth called when splitting a node
	QTree(const BoundingBox, const int);

	void subdivide();
	void queryRange(const BoundingBox, std::vector<Point*>&, const int);
	void getPointsSquare(std::vector<Point>&, std::vector<Point*>&);
	void getPointsCircle(const BoundingBox, std::vector<Point>&, std::vector<Point*>&);
	double euclideanDistance(const Point&, const Point&);
};

#endif //QT_H
int QTree::nMain = 0;
int QTree::nChildren = 0;

//Constructor
Point::Point()
{
}

Point::Point(const double x, const double y) : x(x), y(y)
{
}

//Constructors
BoundingBox::BoundingBox()
{
}
BoundingBox::BoundingBox(const Point center, const Point halfDim) : center(center), halfDim(halfDim)
{
}

//See if this BB contains the point p
bool BoundingBox::contains(const Point& p)
{
	//Check to see if the point is within the boundaries of the BB.
	if ((p.x >= center.x - halfDim.x &&
		p.x <= center.x + halfDim.x &&
		p.y >= center.y - halfDim.y &&
		p.y <= center.y + halfDim.y))
	{
		return true;
	}
	else
		return false;
}

//Checks if this bounding box and the argument BB overlaps.
bool BoundingBox::intersects(const BoundingBox& b)
{
	//Box A is this, B is b. LE/RE/TE/BE = Left/Right/Top/Bottom Edge.
	//The following is true if there is overlap between the two rectangles.
	if (center.x - halfDim.x <= b.center.x + b.halfDim.x && //LE of A is to the left of B's RE
		center.x + halfDim.x >= b.center.x - b.halfDim.x && //RE of A is to the right of B's LE
		center.y - halfDim.y <= b.center.y + b.halfDim.y && //BE of A is below TE of B
		center.y + halfDim.y >= b.center.y - b.halfDim.y)   //TE of A is above BE of B
	{
		return true;
	}
	else
		return false;
}

//Constructor for user
QTree::QTree(const double cx, const double cy, const double range)
{
	boundary = BoundingBox(Point(cx, cy), Point(range, range));
	depth = 1;

	nE = nullptr;
	nW = nullptr;
	sE = nullptr;
	sW = nullptr;
}

//Internal constructor used when splitting a node
QTree::QTree(const BoundingBox boundary, const int in_depth) : boundary(boundary)
{
	depth = in_depth + 1;

	nE = nullptr;
	nW = nullptr;
	sE = nullptr;
	sW = nullptr;
}

bool QTree::insert(const Point& p)
{
	//Point does not belong to this BB
	if (!boundary.contains(p))
		return false;

	//Add point if the lowest level is reached
	if (depth == QT_MAX_DEPTH)
	{
		points.push_back(p);
		return true;
	}

	//QT needs to be subdivided to reach a lower level unless it already is.
	if (nW == nullptr)
	{
		subdivide();
	}

	if (nW->insert(p))
		return true;
	if (nE->insert(p))
		return true;
	if (sW->insert(p))
		return true;
	if (sE->insert(p))
		return true;

	//Otherwise fail
	return false;
}

void QTree::subdivide()
{
	//Create four new bounding boxes which becomes the children:
	double halfDim_half = boundary.halfDim.x * 0.5;

	//North east
	nE = new QTree(BoundingBox(Point(boundary.center.x + halfDim_half,
		boundary.center.y + halfDim_half),
		Point(halfDim_half, halfDim_half)), depth);

	//North west
	nW = new QTree(BoundingBox(Point(boundary.center.x - halfDim_half,
		boundary.center.y + halfDim_half),
		Point(halfDim_half, halfDim_half)), depth);

	//South east
	sE = new QTree(BoundingBox(Point(boundary.center.x + halfDim_half,
		boundary.center.y - halfDim_half),
		Point(halfDim_half, halfDim_half)), depth);

	//South west
	sW = new QTree(BoundingBox(Point(boundary.center.x - halfDim_half,
		boundary.center.y - halfDim_half),
		Point(halfDim_half, halfDim_half)), depth);
}

void QTree::queryRange(const BoundingBox bb, std::vector<Point*>& res, const int method)
{
	if (!boundary.intersects(bb))
		return;

	//Deepest level reached. Collect and add the points from this QTree.
	if (depth == QT_MAX_DEPTH)
	{
		switch (method)
		{
		case 1: getPointsSquare(points, res);
			break;

		case 2: getPointsCircle(bb, points, res);
			break;
		}
	}

	//No points here. Check children, return if there are none.
	if (nW == nullptr)
	{
		return;
	}
	nE->queryRange(bb, res, method);
	nW->queryRange(bb, res, method);
	sE->queryRange(bb, res, method);
	sW->queryRange(bb, res, method);
	return;
}

void QTree::querySquare(const double cx, const double cy, const double range, std::vector<Point*>& res)
{
	queryRange(BoundingBox(Point(cx, cy), Point(range, range)), res, 1);
	return;
}

void QTree::queryCircle(const double cx, const double cy, const double range, std::vector<Point*>& res)
{
	queryRange(BoundingBox(Point(cx, cy), Point(range, range)), res, 2);
	return;
}

//A method for collecting the points within a BB that is a square.
void QTree::getPointsSquare(std::vector<Point>& points, std::vector<Point*>& res)
{
	for (auto it = points.begin(); it != points.end(); it++)
	{
		res.push_back(&(*it));
	}
	return;
}

//A method for collecting the points within a bounding circle.
void QTree::getPointsCircle(const BoundingBox bb, std::vector<Point>& points, std::vector<Point*>& res)
{
	for (auto it = points.begin(); it != points.end(); it++)
	{
		if (euclideanDistance(bb.center, (*it)) <= bb.halfDim.x)
			res.push_back(&(*it));
	}
	return;
}

double QTree::euclideanDistance(const Point& p1, const Point& p2)
{
	double A = p1.x - p2.x;
	double B = p1.y - p2.y;
	return(sqrt(A * A + B * B));
}