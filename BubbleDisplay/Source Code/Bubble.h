#include <opencv2\opencv.hpp>

using namespace cv;

typedef struct {
	unsigned int ID;
	Point3f center;
	float radius;
	float state;
} Bubble;
