#ifndef __MYDEFINES_H__
#define __MYDEFINES_H__

#define DEBUGGING 1

#define MAXSLEN 255

class MyConfig
{
public:
	MyConfig()
	{
		//default config
		FAST_CORNER_THRESH = 10;
		FAST_CORNER_THRESH_SUPRESS = false;
		HESSIAN_THRESHOLD = 400;
		OCTAVES = 4;
		OCTAVELAYERS = 4;
		EXTENDED = false;
		UPRIGHT = false;

		KERNELSIZE  = 3;
		LOWTHRESHOLD = 18;
		RATIO        = 3;

		DISTANCETHRESH = 0.2;

		DETECTED_USE_CANNY = true;
	}

	inline void setFastCornerThresh(int value)
	{
		FAST_CORNER_THRESH = value;
	}

	inline void setFastCornerThreshSupress(bool value)
	{
		FAST_CORNER_THRESH_SUPRESS = value;
	}

	inline void setHessianThreshold(int value)
	{
		HESSIAN_THRESHOLD = 400;
	}

	inline void setOctaves(int value)
	{
		OCTAVES = 4;
	}

	inline void setOctveLayers(int value)
	{
		OCTAVELAYERS = 4;
	}

	inline void setExtended(bool value)
	{
		EXTENDED = value;
	}

	inline void setUpRight(bool value)
	{
		UPRIGHT = value;
	}

	inline void setKernelSize(int value)
	{
		KERNELSIZE = value;
	}

	inline void setLowThreshold(int value)
	{
		LOWTHRESHOLD = value;
	}

	inline void setRatio(int value)
	{
		RATIO = value;
	}

	inline void setDistanceThresh(double value)
	{
		DISTANCETHRESH = value;
	}

public:
	int FAST_CORNER_THRESH;
	bool FAST_CORNER_THRESH_SUPRESS;
	int HESSIAN_THRESHOLD;
	int OCTAVES;
	int OCTAVELAYERS;
	bool EXTENDED;
	bool UPRIGHT;

	int KERNELSIZE;
	int LOWTHRESHOLD;
	int RATIO;

	double DISTANCETHRESH;
	bool DETECTED_USE_CANNY;

};

class GobalParam
{
public:
	static bool IS_DEBUG;

	//fast param
	static int FAST_CORNER_THRESH;
	static bool FAST_CORNER_THRESH_SUPRESS;

	//surf param
	static int HESSIAN_THRESHOLD;
	static int OCTAVES;
	static int OCTAVELAYERS;
	static bool EXTENDED;
	static bool UPRIGHT;

	//canny param
	static int KERNELSIZE;
	static int LOWTHRESHOLD;
	static int RATIO;

	static double DISTANCETHRESH;

	static bool DETECTED_USE_CANNY;
	static double CANNY_MATACH_THRESHOLD;
	static double DETECTED_KP_THRESHOLD;

	static int LINE_OP_SIZE;
	static int MERGE_LINE_LIMIT;

	static bool G_USING_MASK;
	
};

#endif 