#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("bird.jpg",CV_LOAD_IMAGE_COLOR);
	Mat imgh(img.rows, img.cols,CV_32FC3,Scalar(0,0,0));
	//imgh = img + Scalar(120,120,120);
//	img.convertTo(imgh,-1,1,25);				//
//	img.convertTo(imgh,IPL_DEPTH_8U,1.0/5.0);	//scaling of image
	img.convertTo(imgh,-1,3,0);
	imshow("input image",img);
	imshow("output image",imgh);
	waitKey(0);
	destroyAllWindows();

}
