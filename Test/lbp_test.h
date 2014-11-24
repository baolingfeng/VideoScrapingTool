#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

void elbp(Mat& src, Mat &dst, int radius, int neighbors);

void elbp1(Mat& src, Mat &dst);