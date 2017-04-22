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
void ReplaceSkinWithColor(Mat image)
{
	Size tempSize;


	Mat gray ;
	int medianBlurSize =11;
	//image.convertTo(image,CV_8UC3);

	cv::cvtColor(image,gray,CV_BGR2GRAY);
	cv::medianBlur(gray,gray,medianBlurSize);
	Mat Xedges,Yedges;
	cv::Scharr(gray,Xedges,CV_8U,1,0);
	cv::Scharr(gray,Yedges,CV_8U,1,0,-1);
	Mat edge = Xedges+Yedges;


	tempSize.height = image.rows;
	tempSize.width  = image.cols;
	Mat yuv =Mat(tempSize,CV_8UC3);
	cvtColor(image,yuv,CV_BGR2YCrCb);

	int sw = tempSize.width;
	int sh = tempSize.height;
	Mat mask, maskPlusBorder;
	maskPlusBorder = Mat::zeros(sh+2, sw+2, CV_8UC1);
	mask = maskPlusBorder(Rect(1,1,sw,sh)); // mask is in maskPlusBorder.
	resize(edge, mask, tempSize);
	 // Put edges in both of them.
	const int EDGES_THRESHOLD = 80;
	threshold(mask, mask, EDGES_THRESHOLD, 255, THRESH_BINARY);
	dilate(mask, mask, Mat());
	erode(mask, mask, Mat());


	int const NUM_SKIN_POINTS = 6;
	Point skinPts[NUM_SKIN_POINTS];
	skinPts[0] = Point(sw/2,sh/2-sh/6);
	skinPts[1] = Point(sw/2 - sw/11,sh/2- sh/6);
	skinPts[2] = Point(sw/2 + sw/11, sh/2-sh/6);
	skinPts[3] = Point(sw/2,sh/2+sh/16);
	skinPts[4] = Point(sw/2 - sw/9,sh/2+sh/16);
	skinPts[5] = Point(sw/2 + sw/9,sh/2+sh/16);


	const int LOWER_Y = 60;
	const int UPPER_Y = 80;
	const int LOWER_Cr = 25;
	const int UPPER_Cr = 15;
	const int LOWER_Cb = 20;
	const int UPPER_Cb = 15;
	Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
	Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);

	const int CONNECTED_COMPONENTS = 4; // To fill diagonally, use 8.
	const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE \
	| FLOODFILL_MASK_ONLY;
	Mat edgeMask = mask.clone();
	 // Keep a copy of the edge mask.
	// "maskPlusBorder" is initialized with edges to block floodFill().
	for (int i=0; i< NUM_SKIN_POINTS; i++) {
	floodFill(yuv, maskPlusBorder, skinPts[i], Scalar(), NULL,
	lowerDiff, upperDiff, flags);
//****
	mask -= edgeMask;
	int Red = 0;
	int Green = 70;
	int Blue = 0;
	//add(image, CV_RGB(Red, Green, Blue), image, mask);


}
Mat getFaceMask(Size size)
{
	// Draw the color face onto a black background.
	Mat faceOutline = Mat::zeros(size, CV_8UC3);
	Scalar color = CV_RGB(255,255,0);
	 // Yellow.
	int thickness = 4;
	// Use 70% of the screen height as the face height.
	int sw = size.width;
	int sh = size.height;
	int faceH = sh/2 * 70/100; // "faceH" is the radius of the ellipse.
	// Scale the width to be the same shape for any screen width. int
	int faceW = faceH * 72/100;
	// Draw the face outline.
	ellipse(faceOutline, Point(sw/2, sh/2), Size(faceW, faceH),
	0, 0, 360, color, thickness, CV_AA);


	// Draw the eye outlines, as 2 arcs per eye.
	int eyeW = faceW * 23/100;
	int eyeH = faceH * 11/100;
	int eyeX = faceW * 48/100;
	int eyeY = faceH * 13/100;
	Size eyeSize = Size(eyeW, eyeH);
	// Set the angle and shift for the eye half ellipses.
	int eyeA = 15; // angle in degrees.
	int eyeYshift = 11;
	// Draw the top of the right eye.
	ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY),
	eyeSize, 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
	// Draw the bottom of the right eye.
	ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY -eyeYshift),
	eyeSize, 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);
	// Draw the top of the left eye.
	ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY),
	eyeSize, 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
	// Draw the bottom of the left eye.
	ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY - eyeYshift),
	eyeSize, 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);

	// Draw the bottom lip of the mouth.
	int mouthY = faceH * 48/100;
	int mouthW = faceW * 45/100;
	int mouthH = faceH * 6/100;
	ellipse(faceOutline, Point(sw/2, sh/2 + mouthY), Size(mouthW,
	mouthH), 0, 0, 180, color, thickness, CV_AA);

	// Draw anti-aliased text.
	int fontFace = FONT_HERSHEY_COMPLEX;
	float fontScale = 1.0f;
	int fontThickness = 2;
	char *szMsg = "Put your face here";
	putText(faceOutline, szMsg, Point(sw * 23/100, sh * 10/100),
	fontFace, fontScale, color, fontThickness, CV_AA);
	imshow("FaceOutline",faceOutline);
	return faceOutline;
}
int main()
{

	VideoCapture cap;
	int camNum=0;
	cap.open(camNum);
	if(!cap.isOpened())
	{
		cout<<"The camera couldn't be started"<<endl;
	}
	Mat frame ;
	cap.read(frame);
	Size size ;
	size.height = frame.rows;
	size.width = frame.cols;
	Mat Facemask = getFaceMask(size);
	waitKey(0);
	while(1)
	{
		cap.read(frame);
		Mat temp ;
		frame+=Facemask;
		frame.copyTo(temp);
		//ReplaceSkinWithColor(temp);
		imshow("Frame",frame);
		char key = waitKey(2);
		if(key==27)
			break;
	//	AlienImage(frame) ;
	}
}
