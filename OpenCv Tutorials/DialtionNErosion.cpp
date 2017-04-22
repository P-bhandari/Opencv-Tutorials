#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main()
{
	Mat car = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	Mat erodedImage(car.rows,car.cols,CV_8UC3,Scalar(0,0,0));
	Mat dilatedImage(car.rows,car.cols,CV_8UC3,Scalar(0,0,0));
	Mat invertedImage(car.rows,car.cols,CV_8UC3,Scalar(0,0,0));
	Mat mask(car.rows,car.cols,CV_8UC1,Scalar(255));
	//invertedImage = car.clone();


    Mat element = getStructuringElement(MORPH_RECT,Size(5,5),Point(0,0));



    erode(car,erodedImage,element);
	dilate(car,dilatedImage,element);
	bitwise_not(car,invertedImage,mask);
	imshow("original image",car);
	imshow("eroded iamge",erodedImage);
	imshow("dilated image",dilatedImage);
	imshow("inverted image",invertedImage);
	waitKey(0);
	destroyAllWindows();
}
