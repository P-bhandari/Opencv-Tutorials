#include <cmath>
#include <cstdio>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;


////*********Function Skeleton********\\\\\\\\
//void goodFeaturesToTrack(InputArray image, OutputArray corners, int maxCorners,
//						double qualityLevel, double minDistance, InputArray mask=noArray(),
//						int blockSize=3, bool useHarrisDetector=false, double k=0.04 )

/// Global variables
Mat src, src_gray;
Mat dst, dst_gray;
vector<Point2f> cornersA; //it will contain all the points
vector<Point2f> cornersB; //it will contain all the points



int maxCorners = 23;
int maxTrackbar = 100;
int qualityLevel = 10 ;	// indicates the minimal acceptable lower eigenvalue for a point to be included as a corner.
RNG rng(12345);
char* source_window = "Image";

/// Function header
void goodFeaturesToTrack_Demo( int, void* );

/**
 * @function main
 */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray

  src = imread( "Piyush1.jpg", 1 );
  dst = imread("Piyush2.jpg",1);
  cvtColor( src, src_gray, CV_BGR2GRAY );
  cvtColor(dst,dst_gray,CV_BGR2GRAY);
  /// Create Window
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );

  /// Create Trackbar to set the number of corners
  createTrackbar( "Max  corners:", source_window, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo );
  createTrackbar( "Quality Level:", source_window, &qualityLevel, 100, goodFeaturesToTrack_Demo );

  imshow( source_window, src );

  goodFeaturesToTrack_Demo( 0, 0 );



  waitKey(0);
  return(0);
}

/**
 * @function goodFeaturesToTrack_Demo.cpp
 * @brief Apply Shi-Tomasi corner detector
 */
void goodFeaturesToTrack_Demo( int, void* )
{
  if( maxCorners < 1 ) { maxCorners = 1; }
  if( qualityLevel ==0 ) {qualityLevel = 1; }

  /// Parameters for Shi-Tomasi algorithm

  double minDistance = 10;
  int blockSize = 3;
  bool useHarrisDetector = false;
  double k = 0.04;

  /// Copy the source image
  Mat copy;
  copy = src.clone();

  /// Apply corner detection
  goodFeaturesToTrack( src_gray,
               cornersA,
               maxCorners,
              (double)qualityLevel/100.0,
               minDistance,
               Mat(),
               blockSize,
               useHarrisDetector,
               k );


  /// Draw corners detected
  cout<<"** Number of corners detected: "<<cornersA.size()<<endl;
//  int r = 4;
//  for( int i = 0; i < cornersA.size(); i++ )
//     { circle( copy, cornersA[i], r, Scalar(rng.uniform(0,255), rng.uniform(0,255),
//              rng.uniform(0,255)), -1, 8, 0 ); }

  /// Show what you got
//  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
//  imshow( source_window, copy );

  /// Set the neeed parameters to find the refined corners
  Size winSize = Size( 5, 5 );
  Size zeroZone = Size( -1, -1 );
  TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 );

  /// Calculate the refined corner locations
  cornerSubPix( src_gray, cornersA, winSize, zeroZone, criteria ); 	//increases the accuracy of the corners

  /// Write them down
  for( int i = 0; i < cornersA.size(); i++ )
     { cout<<" -- Refined Corner ["<<i<<"]  ("<<cornersA[i].x<<","<<cornersA[i].y<<")"<<endl; }


  //find the flow
  Mat imgC = src_gray.clone();

   vector<uchar> features_found;
   Mat features_errors;
   TermCriteria criteria1=TermCriteria(CV_TERMCRIT_EPS , 30, 0.01);
   int flags=0;
   double minEigThreshold=1e-4;
   Size winsize(21,21);
   Size pyrsize(src.rows +8,src.cols/3);
   cout<<"src type"<<src_gray.type()<<"dst type"<<dst_gray.type()<<endl;
   cv::calcOpticalFlowPyrLK(src_gray,dst_gray,cornersA,cornersB,features_found,features_errors,winSize,3,criteria1, flags, minEigThreshold);


   for(int i=0; i<cornersA.size();i++)
   {
	   	   if(features_found[i]==0)
	   	   {
	   		   cout<<"Error for the index "<<i<<" = "<<endl;
	   		   continue;
	   	   }

	   	   Point p0=Point(std::floor(cornersA[i].x),std::floor(cornersA[i].y));
	   	   Point p1=Point(std::floor(cornersB[i].x),std::floor(cornersB[i].y));
	   	   cv::line(imgC,p0,p1,Scalar(0,255,0),3,1,0);

   }

   imshow("Ouput",imgC);
}


//skeleton of the function
//
//void calcOpticalFlowPyrLK(InputArray prevImg, InputArray nextImg, InputArray prevPts,
//						  InputOutputArray nextPts, OutputArray status, OutputArray err,
//						  Size winSize=Size(21,21), int maxLevel=3,
//						  TermCriteria criteria
//						  =TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01),
//						   int flags=0, double minEigThreshold=1e-4 )
