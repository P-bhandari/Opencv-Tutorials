#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;



Mat logTransform(Mat image);
Mat powerLawTransform(Mat image);
int main()
{
	Mat image = imread("car.jpg",0);
	imshow("image",image);
	//Mat power_law_output=powerLawTransform(image);
	//imshow("power_law_output",power_law_output);
	Mat logtransform = logTransform(image);
	imshow("log transform",logtransform);
	waitKey(0);
}

Mat logTransform(Mat image)
{
	image.convertTo(image,CV_32F);
	float c = 1;

	log(image+1,image);
	image.convertTo(image,CV_8U);
	image = c*image;
	cv::normalize(image,image,0,255,cv::NORM_MINMAX);
	return image;
}

Mat powerLawTransform(Mat image)
{

	/*The formula for this transform is s=cr^y */

	float gamma = 0.5 ;
	float c = 1;

	image.convertTo(image,CV_32F);
	//get maximum value of the image
	double maxVal ,minVal;
	cv::minMaxLoc(image,&minVal,&maxVal,0,0);
	//the value of r (input pixels should be from 0-1)
	image = image -(float)minVal;
	image= (image/(float)maxVal);

	vector<Mat> planes;
	split(image,planes);
	//imshow("eror",image);
	//cout<<"The values of b at 68,68 "<<(float)planes[0].at<uchar>(68,68)<<endl;
	//convertScaleAbs(image,image,(double)1.0/maxVal,0);
//	image.convertTo(image,CV_32F);
	pow(image,gamma,image);
	image=image*255;
	//	convertScaleAbs(image,image,maxVal,0);
	image.convertTo(image,CV_8U);
	return image;
}
