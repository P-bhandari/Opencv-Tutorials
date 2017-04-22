//BACKGRAOUND SEGMENTATION
/* Normally, background is considered to be any static or periodically
moving parts of a scene that remain static or periodic over the period of interest.*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Point point(120,120); //to select where the rectangle would be made
Point p2(point.x+200,point.y+200);

Mat get_image()
{
	VideoCapture capture(0);
	Mat frame;
	capture.read(frame);
	cout<<"Frame height and width "<<frame.cols<<frame.rows<<endl;

	namedWindow("Select Object",CV_WINDOW_AUTOSIZE);
	while(!frame.empty())
	{
		rectangle(frame,point,p2,Scalar(0,255,0),3,8,0);
		imshow("Select Object",frame);
		int c = waitKey(2);
		if(c==32)
		{
			return frame;
		}
		capture.read(frame);
	}
}

int main()
{
	//Mat image = imread("Piyush1.jpg",CV_LOAD_IMAGE_COLOR);
	//get image
	Mat image = get_image();
	destroyWindow("Select Object");
	cout<<"Image channels "<<image.channels()<<"Depth "<<image.depth();
	Mat input(image.rows,image.cols,IPL_DEPTH_8U,Scalar::all(0));
	//Size size(800,800);

	image.convertTo(image,IPL_DEPTH_8U);

	//namedWindow("victor");
	//resize(image,input,size);

	input = image;

	imshow("image",image);
	imshow("Input ",input);
	cout<<"Image height and width "<<image.cols<<image.rows<<endl;

	//set ROI or template
	Rect imageroi = Rect(point.x,point.y,200,200);	//the last two parameters are the width and height of the rectangle



	//cout<< "Roi x= "<<imageroi.x <<" and height = "<<imageroi.height<<"Roi y="<< imageroi.y <<"roi.width = "<<imageroi.width<<"Roi height ="<< imageroi.height <<"image cols = "<< input.cols<<" image rows="<<input.rows<< endl ;
	Mat roi_temp = input(imageroi);
	waitKey(0);

	imshow("template ",roi_temp);

	//match template
	int iwidth =input.cols-roi_temp.cols+1;
	int iheight=input.rows-roi_temp.rows+1;

	Mat frame;
	VideoCapture cap(0);
	cap.read(frame);
	while(!frame.empty())
	{

		Mat ftmp;
	cv::matchTemplate(frame,roi_temp,ftmp,5);
	double maxVal ;
	Point maxpoint;
	imshow("matching output",ftmp);
	minMaxLoc(ftmp,0,&maxVal,0,&maxpoint,noArray());

	cout<<"Max value = "<<maxVal<<"Location = "<<maxpoint.x<<","<<maxpoint.y<<endl;
//	cout<<"Sup"<<endl;
	namedWindow("output",1);
	imshow("image",ftmp);
	Point p2(maxpoint.x+200,maxpoint.y+200);
	rectangle(frame,maxpoint,p2,Scalar(0,255,0),3,8,0);
	imshow("Input Image",frame);
	cap.read(frame);
	int c = waitKey(2);
	if(c==27)
		break;
	}
	startWindowThread();
	waitKey(0);

}

