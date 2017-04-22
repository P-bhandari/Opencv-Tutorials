#include <cmath>
#include <cstdio>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include<opencv2/opencv.hpp>
#include<string>

using namespace cv;
using namespace std;
int numberOfPeople= 0 ;
RNG rng(10010);
Mat backgroundSubstraction(Mat frame,Mat background);
void detectPedestrians(Mat gray, Mat frame);
void save_image2(Mat image);
void save_image(Mat image);
void updatePedestrian(Mat frame,Point centroid, Rect rect, int index,MatND hist);
MatND calculateHistogram(Mat image);
float compareHistogram(MatND hist1,MatND hist2);
float EuclideanDistance(Point a, Point b);


class Pedestrian
{
	public:
	 Rect roi;
	 int label;
	 MatND hist;
	 Point centroid;
	 Scalar color;

};
vector<Pedestrian> ped ;
int main()
{

	VideoCapture cap("Pedestrian.avi");
	Mat background = imread("PedestrianBackground.jpg",CV_LOAD_IMAGE_COLOR);
	if(!cap.isOpened())
	{
		cout<< "The vedio file cannot be loaded" <<endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_POS_MSEC,0000); //start from 0sec
	Mat frame ;
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout << "FPS of the vedio is:"<<fps<<endl ;

	cap.read(frame);
	while(!frame.empty())
	{

	    Mat gray = backgroundSubstraction(frame,background);

		imshow("Control", gray);
		detectPedestrians(gray,frame);

		cap.read(frame);
		while(waitKey(30)!=27 )
		{

		}

	}

	destroyAllWindows();

}

int matchPedestrian(Mat frame,Point centroid,Rect rect)
{
	int index = -1;
	if(ped.size()==0)
		{
			MatND hist;
			updatePedestrian(frame,centroid,rect,index, hist);
		}

	vector<int> matchedIndex ;
	matchedIndex.clear();
	for(int i = 0;i<ped.size();i++)
	{
		int distance = EuclideanDistance(centroid,ped.at(i).centroid);
		if(distance<7.0)
			matchedIndex.push_back(i);
	}
	if(matchedIndex.size() == 1)
	{
		Mat image = frame(rect);
		MatND hist= calculateHistogram(image);

		double a =compareHistogram(hist,ped.at(matchedIndex.at(0)).hist);
		if(a>0.7)
		{
			updatePedestrian(frame,centroid,rect,matchedIndex.at(0),hist);
			return matchedIndex.at(0) ;
		}
	}
	if(matchedIndex.size()>=2)
	{
		//either occlusion or a number of people are really close
		Mat image = frame(rect);
		MatND hist= calculateHistogram(image);
		matchedIndex.clear();
		int maxMatch = -1;
		int maxMatchIndex = -1;
		for(int i =0 ;i<ped.size();i++)
		{

			double a =compareHistogram(hist,ped.at(i).hist);
			if(a>0.7)
				matchedIndex.push_back(i);
			if(maxMatch<=a)
			{
				maxMatch = a;
				maxMatchIndex=i;
			}

		}

		if(matchedIndex.size()==0)
			{
			    updatePedestrian(frame,centroid,rect,maxMatchIndex,hist);

				return maxMatchIndex ;	//In case no one matches pick the best possible match or use Kalman to go for
			}
		else if(matchedIndex.size()==1)
			{
				updatePedestrian(frame,centroid,rect,matchedIndex.at(0),hist);
				return matchedIndex.at(0);
			}
		else if(matchedIndex.size()==2)	  // Occlusions(Relying on the fact that the occlusion happens and this condition is satisfied)
				{
					return -1;
				}


	}
	if(matchedIndex.size() == 0)
	{
			Mat image = frame(rect);
			MatND hist= calculateHistogram(image);
			matchedIndex.clear();
			for(int i =0 ;i<ped.size();i++)
			{

				double a =compareHistogram(hist,ped.at(i).hist);
				if(a>0.7)
					matchedIndex.push_back(i);

			}
			if(matchedIndex.size()==0)		//New pedestrian
			{
					updatePedestrian(frame,centroid,rect,-1,hist);
					return (ped.size()-1 );
			}
			else if(matchedIndex.size()==1)  //Old pedestrian after occlusions
			{
				updatePedestrian(frame,centroid,rect,matchedIndex.at(0),hist);
				return matchedIndex.at(0) ;
			}
			else if(matchedIndex.size()==2)	  // Occlusions(Relying on the fact that the occlusion happens and this condition is satisfied)
			{
				return -1;
			}


	}



}

void updatePedestrian(Mat frame,Point centroid, Rect rect, int index,MatND hist)
{


	if(index ==-1)
	{
		Pedestrian x;
		x.centroid = centroid;
		numberOfPeople++;
		x.label=numberOfPeople;
		x.roi = rect;
		Mat image = frame(rect);
		x.hist = calculateHistogram(image);
		x.color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));


		ped.push_back(x);
	}
	else
	{
		ped.at(index).centroid = centroid;
		ped.at(index).hist = hist;
		ped.at(index).roi = rect;
	}
}

void detectPedestrians(Mat gray, Mat frame)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Moments> mu(contours.size());
    vector<Point2f> mc(contours.size());
	vector<Rect> bounding_rect;
	Mat output ;
	frame.copyTo(output);
	findContours(gray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for(int i = 0 ; i<contours.size();i++)
	{
		double area = contourArea(contours[i],false);
		//cout<<"area"<<i<<" = "<<area<<endl;
		if(area > 3000)
		{
			Rect rect = boundingRect(contours[i]);
			Moments mu = moments(contours[i]);
			Point centroid = Point2f(mu.m10/mu.m00,mu.m01/mu.m00);
			int index = matchPedestrian(frame,centroid,rect);
			//cout<<"Centroid"<<i<<"= ("<<centroid.x<<","<<centroid.y<<")"<<endl;
			bounding_rect.push_back(rect);
			if(index != -1)
			{
			rectangle(output, rect, ped.at(index).color, 2, 2, 0);

			stringstream ss;
			//if(index>0)
			ss << ped.at(index).label;
			string str = ss.str();
			putText(output, str , Point(rect.x , rect.y-5 ), FONT_HERSHEY_DUPLEX, 1.0, ped.at(index).color, 2.0);
			}
		}

	}



	imshow("Output",output);
}
Mat backgroundSubstraction(Mat frame,Mat background)
{
	Mat srctmp,gray;
	int erodeValue = 3 ;
    int dilateValue = 7;
	int thresholdValue = 18;
	namedWindow("Control",CV_WINDOW_AUTOSIZE);
//	cvCreateTrackbar("Erode", "Control", &erodeValue, 13); //Saturation (0 - 255)
//	cvCreateTrackbar("threshold", "Control", &thresholdValue, 100); //Saturation (0 - 255)
//	cvCreateTrackbar("Dilate", "Control", &dilateValue, 13); //Saturation (0 - 255)
	absdiff(frame, background, srctmp);
	cvtColor(srctmp, gray, CV_BGR2GRAY);

	dilate(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(dilateValue,dilateValue)) );//3
	erode(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(erodeValue, erodeValue)) );
	threshold(gray, gray, thresholdValue, 255, CV_THRESH_BINARY);//60

	return gray ;
}
float EuclideanDistance(Point a, Point b)
{
	return (std::sqrt(pow(a.x - b.x,2.0) + pow(a.y-b.y,2.0)));

}
float compareHistogram(MatND hist1,MatND hist2)
{
    float p1 = compareHist( hist1, hist2 ,CV_COMP_CORREL );

	return p1;
}
MatND calculateHistogram(Mat image)
{
	    Mat out1;
	    //image.copyTo(out1);
	    // Convert to HSV
	    cvtColor( image, out1, COLOR_BGR2HSV );
	    /// Using 50 bins for hue and 60 for saturation
	    int h_bins = 100; int s_bins = 100;
	    int histSize[] = { h_bins, s_bins };

	    // hue varies from 0 to 179, saturation from 0 to 255
	    float h_ranges[] = { 0, 180 };
	    float s_ranges[] = { 0, 256 };

	    const float* ranges[] = { h_ranges, s_ranges };

	    // Use the o-th and 1-st channels
	    int channels[] = { 0, 1 };


	    /// Histograms
	    MatND hist1;


	    /// Calculate the histograms for the HSV images
	    calcHist( &out1,1, channels, Mat(), hist1, 2, histSize, ranges, true, false );
	    normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat() );
	    return hist1;

}
void save_image(Mat image)
{
	//Mat image(200,300,CV_32FC3 ,Scalar(0,230,200));
	vector<int> compression_param;
	compression_param.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_param.push_back(99);
	bool success = imwrite("pedestrian1pic4.jpg",image,compression_param);
	if(!success)
	{
		cout<< "Failed Loading File" <<endl;
	}
	else
	{
		Mat writtenImage = imread("pedestrian1pic4.jpg",CV_LOAD_IMAGE_COLOR);
		imshow("New Image",writtenImage);
//		waitKey(0);
//		destroyAllWindows();
	}
}

void save_image2(Mat image)
{
	//Mat image(200,300,CV_32FC3 ,Scalar(0,230,200));
	vector<int> compression_param;
	compression_param.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_param.push_back(99);
	bool success = imwrite("pedestrian2pic4.jpg",image,compression_param);
	if(!success)
	{
		cout<< "Failed Loading File" <<endl;
	}
	else
	{
		Mat writtenImage = imread("pedestrian2pic4.jpg",CV_LOAD_IMAGE_COLOR);
		imshow("New Image",writtenImage);
		//waitKey(0);
		//destroyAllWindows();
	}
}
