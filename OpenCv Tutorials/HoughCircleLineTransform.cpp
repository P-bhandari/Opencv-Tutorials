#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//void HoughsLineTransform();
void HoughsCircleTransform();
int main()
{
	//HoughsLineTransform();
	HoughsCircleTransform();
}

void HoughsCircleTransform()
{
		Mat image = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
		Mat clone =image.clone();
		Mat clone2 = clone.clone();
		cvtColor(image,image,CV_BGR2GRAY);

		image.convertTo(image,CV_8UC1,1);


		vector<Vec3f> circles;	//should be floating point brother
		namedWindow("Output Image",CV_WINDOW_AUTOSIZE);

		int i,method =CV_HOUGH_GRADIENT;
		int dp = 8 ; //accumulator resolution
		int    param1 = 120 ;  //higher threshold passed to canny()
		double param2 = 20 ;  //accumulator threshold higher the better
		int    minDist = 2 ;  //minimum distance between the centers of the circles
		int    minRadius = 0 ;
		int    maxRadius = 0;
		createTrackbar("minRadius","Output Image",&minRadius,(int)image.rows/2);
		createTrackbar("maxRadius","Output Image",&maxRadius,(int)image.rows/2);
		createTrackbar("accumulator resolution","Output Image",&dp,10);
		createTrackbar("threshold","Output Image",&param1,255);
		createTrackbar("minimum distance b/w centers","Output Image",&minDist,(int)image.rows/2);
		//houghs transform

		while(1)
		{
			HoughCircles(image,circles,method,(double)dp,(double)minDist,(double)param1,(double)param2,minRadius,maxRadius);
			namedWindow("inout Image",CV_WINDOW_AUTOSIZE);
			cout <<"circles size = "<<circles.size()<<endl;
			for(i=0 ; i<circles.size() ; i++)
			{
				circle(clone2,Point(circles[i][0],circles[i][1]),circles[i][2],Scalar(0,255,0),1,8,0);
			}
			imshow("Output Image",clone2);
			clone2 = clone.clone();
			int c=waitKey(2);
			if(c==27)
				break;
		}
}

/*
void HoughsLineTransform()
{
	Mat image = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	Mat clone =image.clone();
	Mat clone2 = clone.clone();
	cvtColor(image,image,CV_BGR2GRAY);

	image.convertTo(image,CV_8UC1 ,1);


	 vector<Vec4i> lines;
	namedWindow("Output Image",CV_WINDOW_AUTOSIZE);
	int rho=200;
	int theta=2 ;
	createTrackbar("Ro","Output Image",&rho,(int)image.rows/2);
	createTrackbar("Theta","Output Image",&theta,(int)3.14*2);
	int i,method =CV_HOUGH_STANDARD;
	int threshold=2; 	 //minimum number of points in accumulator plane that satisfy the line in order to consider it
	int param1 = 0 ;  //for SHT minimum length of line segment
	double param2 = 0 ;  //sets the seperation between collinear segments
	createTrackbar("threshold","Output Image",&threshold,1000);
	createTrackbar("minimum length of line","Output Image",&param1,1000);
	//houghs transform

	while(1)
	{
		HoughLinesP(image,lines,rho,theta,threshold,param1,param2);
		cout <<"line size = "<<lines.size()<<endl;
		for(i=0 ; i<lines.size() ; i++)
		{
			line(clone2 ,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(0,0,255),1,0);
		}
		imshow("Output Image",clone2);
		clone2 = clone.clone();
		int c=waitKey(2);
		if(c==27)
			break;
	}
	destroyAllWindows();

}*/
