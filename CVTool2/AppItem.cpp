#include "AppItem.h"
#include "ObjMan.h"

AppItem::AppItem(void)
{
	matchMethod = 0;

	dft_sz   = Size(0,0);
	F_a2     = Mat();
	F_a2T    = Mat();
	norm_aT  = 0;

	//is_unique = 1;
}


AppItem::~AppItem(void)
{
}


vector<Rect> AppItem::match(Mat& tar_img_gray, Mat& F_I, Mat& F_I2)
{
	//-->debug
	bool debuging = false;//true;//
	Mat map_show;
	Mat tpl_show;
	Mat box_show;
	//<--debug

	Mat rst;

	double* rst_i;
	int i,j;
	int a, b, a_opt, b_opt;
	double v, v_opt;
	double ep = 0.002;
	double current_thr = dft_thr;
	bool isPresent = false;

	double minVal, maxVal;
	Point minLoc, maxLoc;

	getAlphaTplMatchingMap(Size(tar_img_gray.cols,tar_img_gray.rows), dft_tpl_sz, Size(msk_rect_tight.width, msk_rect_tight.height), 
							rst, F_a2, F_a2T, norm_aT, F_I, F_I2);
	minMaxLoc( rst, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
	
	vector<Rect> detects;
	if( maxVal>=current_thr )
	{
		if( maxVal-ep>=current_thr )
			current_thr = maxVal-ep;
		isPresent = true;
		for( i=0; i<rst.rows; i++ )
		{
			rst_i = (double*)rst.ptr<double>(i);//double type
			for( j=0; j<rst.cols; j++ )
			{
				if( rst_i[j] >= current_thr )
				{
					v_opt = rst_i[j];
					a_opt = i;
					b_opt = j;
					for( a=i; a<i+msk_rect_tight.height && a<rst.rows; a++ )
					{
						for( b=j; b<j+msk_rect_tight.width && b<rst.cols; b++ )
						{
							v = rst.at<double>(a,b);
							if( v>v_opt)
							{
								rst.at<double>(a_opt, b_opt) = 0;
								a_opt = a;
								b_opt = b;
								v_opt = v;
							}else
							{
								rst.at<double>(a,b) = 0;
							}
						}
					}
					detects.push_back( Rect( b_opt-msk_rect_tight.x, a_opt-msk_rect_tight.y, item.cols, item.rows) );
				
				}
			}
		}
	}
	
	if( !isPresent )
	{
		printf("Alpha Tpl Matching: No match found.\n");
	}

	///reset used vector
	return detects;
}

vector<Rect> AppItem::match(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, Mat tar_edge)
{
	FlannBasedMatcher		 matcher;
	vector<vector<DMatch > > matches;
	vector<DMatch >			 matches_good;
	Mat						 matches_img;

	matcher.knnMatch(item_des, tar_des, matches, 2);
	for(int i = 0; i < min(tar_des.rows-1,(int) matches.size()); i++)
	{
		if((matches[i][0].distance < 0.98*(matches[i][1].distance)) && 
			((int) matches[i].size()<=2 && (int) matches[i].size()>0))
		{
			matches_good.push_back(matches[i][0]);
		}
	}

	return clusterDetects_hough_fixedscale(tar_img_gray,tar_kps,tar_des,tar_edge,matches_good);
}

vector<Rect> AppItem::clusterDetects_hough_fixedscale(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des,Mat tar_edge, vector< DMatch > matches)
{
	//Cluster the detects similar to hough transformation meathod
	//This function deals with the case where template appears in the target image without scaling.
	const double SUPPORTSIGMA = GobalParam::DISTANCETHRESH;
	double revSUPPORTSIGMA2 = 0.5/(SUPPORTSIGMA*SUPPORTSIGMA);

	int i;
	int fr_i, to_i;
	int loc_x, loc_y;
	float cnt;
	Mat houghmap;
	houghmap = Mat::zeros(tar_img_gray.rows+item.rows, tar_img_gray.cols+item.cols, CV_32FC1);

	vector<Rect> detects;
	for( i=0; i<matches.size(); i++)
	{
		fr_i = matches[i].queryIdx;
		to_i = matches[i].trainIdx;

		loc_x = tar_kps[to_i].pt.x - item_kps[fr_i].pt.x + item.cols;
		loc_y = tar_kps[to_i].pt.y - item_kps[fr_i].pt.y + item.rows;

		//add support to the location
		//houghmap.at<float>(loc_y, loc_x) += 1;
		houghmap.at<float>(loc_y, loc_x) += exp( -revSUPPORTSIGMA2*matches[i].distance*matches[i].distance );
	}

	//printf("\nFunction clusterDetects_hough_fixedscale\n");
	for( i=0; i<matches.size(); i++)
	{
		fr_i = matches[i].queryIdx;
		to_i = matches[i].trainIdx;

		loc_x = tar_kps[to_i].pt.x - item_kps[fr_i].pt.x + item.cols;
		loc_y = tar_kps[to_i].pt.y - item_kps[fr_i].pt.y + item.rows;
		
		cnt = houghmap.at<float>(loc_y, loc_x);

		Rect temp( loc_x-item.cols, loc_y-item.rows, item.cols, item.rows) ;
		if(temp.x<0) temp.x = 0;
		if(temp.y<0) temp.y = 0;
		if(temp.x+temp.width>tar_img_gray.cols) temp.width = tar_img_gray.cols - temp.x;
		if(temp.y+temp.height>tar_img_gray.rows) temp.height = tar_img_gray.rows - temp.y;
		Mat sub_tar_edge(tar_edge,temp);

		bool useEdge = GobalParam::DETECTED_USE_CANNY;
		double thre = GobalParam::DETECTED_KP_THRESHOLD;
		if( useEdge && cnt > thre*item_kps.size() && cannyMatch(sub_tar_edge,item_edge)
			|| (!useEdge && cnt> thre*item_kps.size()))
		{
			detects.push_back( temp );

			houghmap.at<float>(loc_y, loc_x) = 0;
		}
		
	}
	return detects;
}

vector<Rect> AppItem::clusterDetects_hough(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, vector<DMatch> matches)
{
	printf("\nFunction clusterDetects_hough\n");
	const double MINSCALE = 0.5;
	const double MAXSCALE = 5;
	const double FINENESS = 0.1;
	const double SUPPORTSIGMA = GobalParam::DISTANCETHRESH;
	double ep = 1e-5;//EPSILON;
	double revFINENESS = 1.0/FINENESS;
	double revSUPPORTSIGMA2 = 0.5/(SUPPORTSIGMA*SUPPORTSIGMA);

	int padrows = MAXSCALE*item.rows;
	int padcols = MAXSCALE*item.cols;
	int sz[] = {tar_img_gray.rows+padrows, tar_img_gray.cols+padcols, (MAXSCALE-MINSCALE)/FINENESS+1  };
	SparseMat_<float> houghmap(3, sz);

	int i,j;
	int fr[2];
	int to[2];
	Point2f fr_pt[2];
	Point2f to_pt[2];
	double fr_dx, fr_dy;
	double to_dx, to_dy;

	vector<Rect> detects;
	int nDetects;
	
	int    loc_x, loc_y, loc_s;
	double org_x, org_y, org_s;

	for( i=0; i<matches.size(); i++ )
	{
		for( j=0; j<matches.size(); j++ )
		{
			if(i==j)
			{
				continue;
			}
			fr[0] = matches[i].queryIdx;
			to[0] = matches[i].trainIdx;

			fr_pt[0] = item_kps[fr[0]].pt;
			to_pt[0] =  tar_kps[to[0]].pt;

			fr[1] = matches[j].queryIdx;
			to[1] = matches[j].trainIdx;

			fr_pt[1] = item_kps[fr[1]].pt;
			to_pt[1] =  tar_kps[to[1]].pt;

			fr_dx = fr_pt[0].x - fr_pt[1].x;
			fr_dy = fr_pt[0].y - fr_pt[1].y;
			to_dx = to_pt[0].x - to_pt[1].x;
			to_dy = to_pt[0].y - to_pt[1].y;

			if( abs(fr_dx+fr_dy)<ep || abs(fr_dx)<ep || abs(fr_dy)<ep )
			{
				continue;
			}

			org_s = (to_dx+to_dy)/(fr_dx+fr_dy);
			if( org_s < MINSCALE || org_s > MAXSCALE )
			{
				continue;
			}
			org_x = 0.5*(to_pt[0].x+to_pt[1].x - org_s*(fr_pt[0].x+fr_pt[1].x));
			org_y = 0.5*(to_pt[0].y+to_pt[1].y - org_s*(fr_pt[0].y+fr_pt[1].y));

			loc_x = (int)floor(org_x+padcols+0.5);
			loc_y = (int)floor(org_y+padrows+0.5);
			loc_s = (int)floor((org_s-MINSCALE)*revFINENESS + 0.5);

			houghmap.ref(loc_y, loc_x, loc_s) += exp( -revSUPPORTSIGMA2*( matches[i].distance*matches[i].distance + 
																		 matches[j].distance*matches[j].distance ) );
		}
	}

	nDetects = 0;
	
	SparseMatIterator_<float> it = houghmap.begin(),
                              it_end = houghmap.end();
    for(; it != it_end; ++it)
	{
		const SparseMat_<float>::Node* nd = it.node();
		//float itnum = (*it);//debug
		//float itnum_mat = houghmap(nd->idx[0],nd->idx[1], nd->idx[2]);//debug
		if( (*it) > 0.01 * item_kps.size()*item_kps.size() )
		{
			loc_y = nd->idx[0] - padrows;
			loc_x = nd->idx[1] - padcols;
			org_s = (nd->idx[2])*FINENESS + MINSCALE;

			printf("Rect_%d (%4d,%4d,%2.1lf) contains %.2f matches.\n", nDetects, loc_x, loc_y, org_s, houghmap(nd->idx));

			Rect temp( loc_x, loc_y, org_s*item.cols, org_s*item.rows);
			if(temp.x<0) temp.x = 0;
			if(temp.y<0) temp.y = 0;
			if(temp.x+temp.width>tar_img_gray.cols) temp.width = tar_img_gray.cols - temp.x;
			if(temp.y+temp.height>tar_img_gray.rows) temp.width = tar_img_gray.rows - temp.y;

			detects.push_back(temp);
			nDetects ++;
		}
	}

	return detects;
}

bool AppItem::cannyMatch(Mat tar_canny, Mat item_canny)
{
	if(tar_canny.rows != item_canny.rows || tar_canny.cols != item_canny.cols) 
		return false;

	int count = 0;
	for(int i=0; i<item_canny.rows; i++)
	{
		for(int j=0; j<item_canny.cols; j++)
		{
			if(item_canny.at<uchar>(i,j) == 255 && tar_canny.at<uchar>(i,j) == 255)
			{
				count++;
			}
		}
	}

	double ratio = count * 1.0 / countNonZero(item_canny);

	return ratio > GobalParam::CANNY_MATACH_THRESHOLD;
}

bool AppItem::tightenMask()
{//The msk must be of UCHAR type.
	if( item_msk.empty() )
		return false;

	msk_rect_tight.x = item_msk.cols-1;
	msk_rect_tight.y = item_msk.rows-1;
	msk_rect_tight.width  = 0;
	msk_rect_tight.height = 0;

	int tmpx = 0; 
	int tmpy = 0;
	for(int i=0; i<item_msk.rows; i++)
	{
		uchar* msk_i = (uchar*)item_msk.ptr<uchar>(i);//uchar type
		for(int j=0; j<item_msk.cols; j++)
		{
			if( msk_i[j] != 0 )
			{
				if( msk_rect_tight.x > j ){ msk_rect_tight.x = j;}
				if( msk_rect_tight.y > i ){ msk_rect_tight.y = i;}
				if( tmpx    < j ){ tmpx    = j;}
				if( tmpy    < i ){ tmpy    = i;}
			}
		}
	}

	msk_rect_tight.width = tmpx - msk_rect_tight.x + 1;
	msk_rect_tight.height= tmpy - msk_rect_tight.y + 1;

	msk_rect_tight.width = msk_rect_tight.width>0 ? msk_rect_tight.width : 0;
	msk_rect_tight.height = msk_rect_tight.height>0 ? msk_rect_tight.height : 0;

	item_tight = msk_rect_tight;
	return true;
}

void AppItem::initDFTParameters(Size dft_sz_in, Size dft_tpl_sz_in, double dft_thr_in)
{
	dft_sz     = dft_sz_in;
	dft_tpl_sz = dft_tpl_sz_in;
	dft_thr    = dft_thr_in;

	Mat img_gray, img_fp;
	Mat msk_gray, msk_fp;
	Mat M_a2;
	Mat M_a2T;
	Mat M_aT;

	img_gray = Mat::zeros(dft_tpl_sz, item.type());
	Mat roiImg_gray( img_gray, Rect(0,0,msk_rect_tight.width, msk_rect_tight.height) );
	item( msk_rect_tight ).copyTo( roiImg_gray);
	
	//cvtColor(img_gray, img_gray, CV_BGR2GRAY);//Currently only deel with gray images

	msk_gray = Mat::zeros(dft_tpl_sz, item_msk.type());
	Mat roiMsk_gray( msk_gray, Rect(0,0,msk_rect_tight.width, msk_rect_tight.height) );
	item_msk( msk_rect_tight ).copyTo( roiMsk_gray);//already gray img

	im2double( img_gray, img_gray );
	im2double( msk_gray, msk_gray );

	flip(img_gray, img_fp, -1);
	flip(msk_gray, msk_fp, -1);
		
	M_a2  = msk_fp.mul(msk_fp);
	M_a2T = M_a2.mul(img_fp);
	M_aT  = msk_fp.mul(img_fp);

	norm_aT = norm(M_aT, NORM_L2);

	F_a2 = Mat::zeros(dft_sz, CV_64FC1);
	F_a2T= Mat::zeros(dft_sz, F_a2.type());
	Mat roiF_a2 (F_a2,  Rect(0,0,msk_fp.cols,msk_fp.rows));
	M_a2.copyTo(roiF_a2);
	Mat roiF_a2T(F_a2T, Rect(0,0,msk_fp.cols,msk_fp.rows));
	M_a2T.copyTo(roiF_a2T);

	// now transform the padded Matrices in-place;
	// use "nonzeroRows" hint for faster processing
	dft(F_a2,  F_a2,  0, msk_fp.rows);
	dft(F_a2T, F_a2T, 0, msk_fp.rows);
}

void AppItem::getAlphaTplMatchingMap(Size tar_img_sz, Size com_tpl_sz, Size own_tpl_sz, Mat& map, Mat& F_a2, Mat& F_a2T, double aT, Mat& F_I, Mat& F_I2)
{
	//Convolution
	Mat tmpMat;
	map.create(abs(tar_img_sz.height - own_tpl_sz.height)+1, abs(tar_img_sz.width - own_tpl_sz.width)+1, F_a2.type());

	mulSpectrums(F_a2T, F_I, tmpMat, 0, false);
	dft(tmpMat, tmpMat, DFT_INVERSE+DFT_SCALE, tar_img_sz.height+com_tpl_sz.height-own_tpl_sz.height);//
	tmpMat(Rect(com_tpl_sz.width-1, com_tpl_sz.height-1, map.cols, map.rows)).copyTo(map);
	

	//Calculate the normalization of the cost
	Mat M_norm_a2I2(map.rows, map.cols, F_a2.type());

	mulSpectrums(F_a2, F_I2, tmpMat, 0, false);
	dft(tmpMat, tmpMat, DFT_INVERSE+DFT_SCALE, tar_img_sz.height+com_tpl_sz.height-own_tpl_sz.height);//tar_img_sz.height
	tmpMat(Rect(com_tpl_sz.width-1, com_tpl_sz.height-1, map.cols, map.rows)).copyTo(M_norm_a2I2);

	sqrt(M_norm_a2I2, M_norm_a2I2);

	map = map/M_norm_a2I2 * (1/aT);
}

bool AppItem::isUsingMask()
{
	return !item_msk.empty();
}