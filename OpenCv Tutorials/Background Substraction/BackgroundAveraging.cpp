//BACKGRAOUND SEGMENTATION
/* Normally, background is considered to be any static or periodically
moving parts of a scene that remain static or periodic over the period of interest.*/

//AVERAGING OF IMAGES //SUMMATIONS  //ACCUMULATION

/*To find average use
 * accumulate(InputArray src, InputOutputArray dst,InputArrayMask) ;
 *
 * dst<-dst+src ; for all elements with non zero value in mask
 *
 * If we were to accumulate them with weighted average (With the latest image with the most weight
 *
 * accumulateWeighted(InputArray src, InputOutputArray dst, double alpha, InputArrayMask)
 *
 * dst<- (1-alpha)*dst + alpha*src;
 *
 * alpha(Update speed , how fast the function forgets about the previous inputs
 *
 *
 * the first sum would
give 0.5 × 2 + 0.5 × 3 = 2.5 and then adding the third term would give 0.5 × 2.5 + 0.5 ×
4 = 3.25. The reason the second number is larger is that the most recent contributions
are given more weight than those from farther in the past*/


/*
 *
 * The black dots are the background and the clear image is the foreground or the change
 */

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

void backgroundDiff(Mat I,Mat Imask);
void AllocateImages(Mat I);
void accumulateBackground(Mat I);
void createModelsfromStats();
void setLowThreshold(float scale);
void setHighThreshold(float scale);
Mat IavgF,IdiffF,IprevF,IlowF,IhiF;
Mat Iscratch, Iscratch2;
Mat Igray[3];
Mat Ilow[3];
Mat Ihi[3];
Mat Imaskt;
float Icount ;
int first=1;
int main()
{
	namedWindow("BACKGROUND AVERAGING",CV_WINDOW_AUTOSIZE);
	VideoCapture cap(0);
	Mat frame,mask1,mask3;

	int frame_count =0 ;
	while(1)
	{
		frame_count++ ;
		cap.read(frame);
		if(frame.empty())
			{
				cout<<"The frame is empty"<<endl;
				break;
			}
		int rows = frame.rows;
		int cols = frame.cols;
		mask1.create(rows,cols,CV_8UC1);
		mask3.create(rows,cols,CV_8UC3);
		mask1.setTo(0);
		mask3.setTo(0);

		if(frame_count==1)
			AllocateImages(frame);

		else if(frame_count<30)
				accumulateBackground(frame);
		else if(frame_count==30)
			createModelsfromStats();
		else
		{
			backgroundDiff(frame,mask1);

			cv::cvtColor(mask1,mask3,CV_GRAY2BGR);
			cv::norm(mask3,mask3,CV_C,noArray());

			cv::threshold(mask3,mask3,50,1,CV_THRESH_BINARY);
			cv::multiply(frame,mask3,frame,1);
			imshow("Background averaging",frame);

		}
/*
		if(frame_count==70)
			{
				first=1;
				frame_count=0;
			}*/
		char c = waitKey(2);
		if(c==27)
			break;
	}
}

void backgroundDiff(Mat I,Mat Imask)
{
	Iscratch=I;
	split(Iscratch,Igray);

	//channel1
	cv::inRange(Igray[0],Ilow[0],Ihi[1],Imask);

	//channel2
	cv::inRange(Igray[1],Ilow[1],Ihi[1],Imaskt);
	cv::bitwise_or(Imask,Imaskt,Imask);

	//channel2
	cv::inRange(Igray[2],Ilow[2],Ihi[2],Imaskt);
	cv::bitwise_or(Imask,Imaskt,Imask);

	//InvertTheresults
	Imask =255-Imask;		// because foreground should be the values out of range, not in range.
}


void accumulateBackground(Mat I)
{

	Iscratch =I.clone();
	if(!first)
	{
		cv::accumulate(Iscratch,IavgF);
		cv::absdiff(Iscratch,IprevF,Iscratch2);
		cv::accumulate(Iscratch2,IdiffF);
		Icount+=1.0;
	}
	first=0;
	IprevF=Iscratch;

}

void createModelsfromStats()
{
	cv::convertScaleAbs(IavgF,IavgF,(double)(1.0/Icount));
	cv::convertScaleAbs(IdiffF,IdiffF,(double)(1.0/Icount));
	IdiffF +=1;

	setHighThreshold(7.0);
	setLowThreshold(6.0);


}
void setHighThreshold(float scale)
{

	Iscratch=IdiffF*scale;
	IhiF =Iscratch+IavgF;
	cv::split(IhiF,Ihi);

}
void setLowThreshold(float scale)
{
	Iscratch=IdiffF*scale;
	IlowF =Iscratch+IavgF;
	cv::split(IlowF,Ilow);
}

void AllocateImages(Mat I)
{
	int rows = I.rows;
	int cols = I.cols;

	IavgF.create(rows,cols,CV_32FC3);
	IdiffF.create(rows,cols,CV_32FC3);
	IprevF.create(rows,cols,CV_32FC3);
	IhiF.create(rows,cols,CV_32FC3);
	IlowF.create(rows,cols,CV_32FC3);

	Ilow[0].create(rows,cols,CV_32FC1);
	Ilow[1].create(rows,cols,CV_32FC1);
	Ilow[2].create(rows,cols,CV_32FC1);

	Ihi[0].create(rows,cols,CV_32FC1);
	Ihi[1].create(rows,cols,CV_32FC1);
	Ihi[2].create(rows,cols,CV_32FC1);

	IavgF.setTo(0);
	IdiffF.setTo(0);
	IprevF.setTo(0);
	IhiF.setTo(0);
	IlowF.setTo(0) ;

	Icount = 0.0001 ; //to protect against divide by zero ?


	Iscratch.create(rows,cols,CV_32FC3);
	Iscratch2.create(rows,cols,CV_32FC3);
	Igray[0].create(rows,cols,CV_32FC1);
	Igray[1].create(rows,cols,CV_32FC1);
	Igray[2].create(rows,cols,CV_32FC1);
	Imaskt.create(rows,cols,CV_32FC1);

	Iscratch.setTo(0);
	Iscratch2.setTo(0);
}


