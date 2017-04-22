#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat inputImage = imread("girl.jpg",0);

	Mat outputImage = inputImage.clone();
	inputImage.convertTo(inputImage,CV_32F,1.0/255.0);	//rescale it to the value 0
	imshow("input image",inputImage);
	outputImage.convertTo(outputImage,CV_32F,1.0/255.0);
	dft(inputImage,outputImage,CV_DXT_FORWARD,0);

	imshow("dft output",outputImage);
	int flag = CV_DXT_INVERSE | CV_DXT_SCALE;
	dft(outputImage,inputImage,flag,0);

	imshow("inverse ",inputImage);
	cvWaitKey(0);
	return 0 ;
}
