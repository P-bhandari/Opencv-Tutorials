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

void scaryImage(Mat image)
{
	Mat gray ;
	int medianBlurSize =11;
	//image.convertTo(image,CV_8UC3);

	cv::cvtColor(image,gray,CV_BGR2GRAY);
	cv::medianBlur(gray,gray,medianBlurSize);
	Mat Xedges,Yedges;
	cv::Scharr(gray,Xedges,CV_8U,1,0);
	cv::Scharr(gray,Yedges,CV_8U,1,0,-1);
	Mat edges = Xedges+Yedges;
	imshow("Edges",edges);
	int evilThresh=20;
	Mat mask;
	cv::threshold(edges,mask,evilThresh,255,THRESH_BINARY_INV);
	medianBlur(mask,mask,3);
	imshow("mask",mask);

	Mat output;
	output.setTo(0);
	imshow("Image",image);
	image.copyTo(output,mask);
	//output = output/255;
	imshow("Output",output);



}
void cartoonifyImage(Mat image)
{
	Mat temp;
	cvtColor(image,temp,CV_BGR2GRAY);
	cv::medianBlur(temp,temp,5);
	Mat edges;
	cv::Laplacian(temp,edges,CV_8U,7);
	Mat mask ;
	int edgethresh=100;
	cv::threshold(edges,mask,edgethresh,255,THRESH_BINARY_INV);
	imshow("mask",mask);
	Size size ;
	size.height=image.rows;
	size.width = image.cols;
	Size smallSize;
	smallSize.height=size.height/2;
	smallSize.width =size.width/2;
	Mat smallImage(smallSize,CV_8UC3);
	cv::resize(image,smallImage,smallSize,0,0,INTER_LINEAR);


	Mat temp2 = Mat(smallSize,CV_8UC3);
	int repetitions = 7; // Repetitions for strong cartoon effect.
	for (int i=0; i<repetitions; i++) {
	int ksize = 9;
	 // Filter size. Has a large effect on speed.
	double sigmaColor = 9;
	 // Filter color strength.
	double sigmaSpace = 7;
	 // Spatial strength. Affects speed.
	bilateralFilter(smallImage, temp2, ksize, sigmaColor, sigmaSpace);
	bilateralFilter(temp2, smallImage, ksize, sigmaColor, sigmaSpace);
	}

	Mat bigImg;
	resize(smallImage, bigImg, size, 0,0, INTER_LINEAR);
	Mat dst;
	dst.setTo(0);
	bigImg.copyTo(dst, mask);
	imshow("Cartoon",dst);

}

//*************for single image input********//

//
//
//int main()
//{
//	Mat image = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
//	imshow("original Image",image);
//	cartoonifyImage(image);
//	waitKey(0);
//	destroyAllWindows();
//}
//*************for video input********//
int main()
{
	VideoCapture cap;
	int camNum=0;
	cap.open(camNum);
	if(!cap.isOpened())
	{
		cout<<"The camera couldn't be started"<<endl;
	}
	while(1)
	{
		Mat frame;
		cap.read(frame);
		Mat copy ;
		frame.copyTo(copy);
		//cartoonifyImage(copy);
		scaryImage(copy);
		imshow("Original Image",frame);
		char key = waitKey(4);
		if(key==27)
			break;
	}

}
