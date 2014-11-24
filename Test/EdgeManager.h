#pragma once
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"
#include <vector>

#include "MyDefines.h"

using namespace std;
using namespace cv;

class EdgeManager
{
public:
	EdgeManager(void);
	~EdgeManager(void);
public:
	
	MyImage myImage;

};

