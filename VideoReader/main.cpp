#include <stdio.h>
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"
#include <io.h>
#include <fstream>

using namespace std;
using namespace cv;

inline double corr2(Mat A, Mat B)
{
	int channel = A.channels();
	if(channel > 1)
	{
		//warning
		cout<<"channel is bigger than 1"<<endl;
	}

	Scalar meanA = cv::mean(A);
	Scalar meanB = cv::mean(B);
	//meanStdDev(A, meanA, stdA);
	//meanStdDev(B, meanB, stdB);

	double ma = meanA[0];
	double mb = meanB[0];

	//double sa = stdA[0];
	//double sb = stdB[0];

	Mat doubleA;
	A.convertTo(doubleA,CV_64FC1);
	Mat doubleB;
	B.convertTo(doubleB,CV_64FC1);

	double sum = 0;
	double sum1 = 0;
	double sum2 = 0;
	for(int i=0; i<doubleA.rows; i++)
	{
		for(int j=0; j<doubleA.cols; j++)
		{
			sum += (doubleA.at<double>(i,j) - ma) * (doubleB.at<double>(i,j) - mb);
			sum1 += (doubleA.at<double>(i,j) - ma) * (doubleA.at<double>(i,j) - ma);
			sum2 += (doubleB.at<double>(i,j) - mb) * (doubleB.at<double>(i,j) - mb);
		}
	}


	double result = sum / (sqrt(sum1) * sqrt(sum2));

	return result;
}

void video_stat()
{
	string root = "D:/experimental video/";
	string subfolders[] = {"20121019","20120926","20120911"};
	ofstream fcout("stat.csv");

	for(int i=0; i<3; i++)
	{
		string folderpattern = root + subfolders[i] + "/*";
		_finddata_t fileDir;
		long lfDir;

		if((lfDir=_findfirst(folderpattern.c_str(), &fileDir) )!= -1)
		{
			do
			{
				if(strcmp(fileDir.name,".") == 0 || strcmp(fileDir.name,"..") == 0) continue;

				if(fileDir.attrib == _A_SUBDIR)
				{
					string subjectpattern = root + subfolders[i] + "/" + fileDir.name + "/*"; 
					_finddata_t subjectfile;
					long lfSubDir;

					string subjectname;
					bool flag = false;
					if((lfSubDir = _findfirst(subjectpattern.c_str(), &subjectfile)) != -1)
					{
						do
						{
							string filename(subjectfile.name);
							int index;
							if((index = filename.find(".flv")) >= 0)
							{
								subjectname = filename.substr(0, index);
								flag =  true;
								break;
							}

						}while(_findnext( lfSubDir, &subjectfile ) == 0);
					}

					if(flag)
					{
						string videoname =  root + subfolders[i] + "/" + fileDir.name + "/" + subjectname + ".flv";
						VideoCapture cap(videoname);
						if(cap.isOpened())
						{
							int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
							double fps = cap.get(CV_CAP_PROP_FPS);
							fcout<<subjectname<<", ";
							fcout<<frameCount<<", ";
							fcout<<fps<<", ";

							string diffimagespattern = root + subfolders[i] + "/" + fileDir.name + "/" + subjectname +"/*";
							_finddata_t diffpattern;
							long lfDiffDir;

							if((lfDiffDir = _findfirst(diffimagespattern.c_str(), &diffpattern)) != -1)
							{
								int count = 0;
								do
								{
									string strDiff(diffpattern.name);
									int idx = strDiff.find(".png");
									if(idx >= 0)
									{
										count++;
									}
								}while( _findnext( lfDiffDir, &diffpattern ) == 0);
								fcout<<count<<", ";
							}
							fcout<<endl;
						}
					}
				}
			}while( _findnext( lfDir, &fileDir ) == 0 );
		}
	}
	fcout.close();
}

int main()
{
	video_stat();
	/*
	//string input_filename = "D:\\experimental video\\new\\zhp\\zhp_jedit.avi";
	string input_filename = "D:\\experimental video\\20121019\\chenxiao\\chenxiao.flv";

	VideoCapture cap(input_filename);
	if(!cap.isOpened())
		return -1;

	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	double fps = cap.get(CV_CAP_PROP_FPS);
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
	*/

	/*
	Mat preFrame;
	for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera

		cout<<"reading..."<<endl;

		
		if(!preFrame.empty())
		{
			Mat c1[3];
			Mat c2[3];

			split(frame,c1);
			split(preFrame,c2);

			double B = corr2(c1[0],c2[0]);
			double G = corr2(c1[1],c2[1]);
			double R = corr2(c1[2],c2[2]);

			std::cout<<B<<"/"<<G<<"/"<<R<<std::endl;
		}

		frame.copyTo(preFrame);

        //cvtColor(frame, edges, CV_BGR2GRAY);
       // GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
       // Canny(edges, edges, 0, 30, 3);
       // imshow("frame", frame);
        //waitKey(0);
    }
		*/
	return 0;
}