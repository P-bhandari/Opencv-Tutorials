#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

//Awesome looks like you are getting blind
void histogram_equalization();
void smoothingImage();
using namespace cv;
using namespace std;

int main()
{

//	histogram_equalization();
	smoothingImage();

	return 0 ;
}

void smoothingImage()	//blurs with time
{
	Mat image = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	Mat smoothedImage(image.rows, image.cols,IPL_DEPTH_8U,Scalar(0,0,0));
	Mat gaussianSmooth(image.rows, image.cols,IPL_DEPTH_8U,Scalar(0,0,0));
	namedWindow("Smoothed Image",CV_WINDOW_AUTOSIZE);
	namedWindow("Gaussian Smoothed Image",CV_WINDOW_AUTOSIZE);

	string zBuffer;
	int sliderValue1=3 , sliderValue2=3;
	int i,j ;

	//create trackbars
	createTrackbar("KernelSize","Smoothed Image",&sliderValue1,31);
	createTrackbar("KernelSize","Gaussian Smoothed Image",&sliderValue2,31);
	while(1)
	{
		i = sliderValue1 ;
		if( i%2 ==0)
			i++ ;
		j = sliderValue2 ;
		if( j%2 == 0)
				j++ ;
		blur(image,smoothedImage,Size(i,i));
		GaussianBlur(image,gaussianSmooth,Size(j,j),0,0);
		imshow("Smoothed Image",smoothedImage);
		imshow("Gaussian Smoothed Image",gaussianSmooth);
		int c =waitKey(2000);

		if(c==27)
			break;
	}
}

void histogram_equalization()
{
	Mat img = imread("bird.jpg",CV_LOAD_IMAGE_COLOR);
	cvtColor(img,img,CV_BGR2GRAY);
	Mat img_histequalized ;

	img.convertTo(img,CV_8UC1);
	equalizeHist(img,img_histequalized);
	imshow("original image",img);
	imshow("histogram equalized image",img_histequalized);
	waitKey(0);
	destroyAllWindows();
}
