//BACKGORUND DETECTION/SUBSTRACTION USING MOG METHOD OF OPENCV
//////////////////////////////////////////
//LearningRate	The value between 0 and 1 that indicates how fast the background model is learnt.
//Negative parameter value makes the algorithm to use some automatically chosen learning rate.
//0 means that the background modelis not updated at all, 1 means that the background model is completely
//reinitialized from the last frame.

//////////////////////////////////
//******Mean filter algorithm for background detection
//Mean filter[edit]
//For calculating the image containing only the background, a series of preceding images are averaged.
//For calculating the background image at the instant t,
//B(x,y)={1 \over N} \sum_{i=1}^N V(x,y,t-i)
//where N is the number of preceding images taken for averaging. This averaging refers to averaging
//corresponding pixels in the given images. N would depend on the video speed (number of images per
//		second in the video) and the amount of movement in the video.[5] After calculating the
//		background B(x,y) we can then subtract it from the image V(x,y,t) at time t=t and threshold it.
//		Thus the foreground is
//|V(x,y,t)-B(x,y)|> \mathrm{Th} \,
//where Th is threshold. Similarly we can also use median instead of mean in the above calculation of B(x,y).
//Usage of global and time-independent Thresholds (same Th value for all pixels in the image) may limit
//the accuracy of the above two approaches.[2]

#include <cmath>
#include <cstdio>
#include <vector>
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

int main()
{

	Mat fgMaskMOG2;
	int history=100,varThreshold=8,shadowsDetect=0;
	int learning_rate =5;
	//**********************
	//	Var threshold is to reduce noise in the detected background, The more the value the less
	//	is the noise
	//***********************
	VideoCapture cap(0);
	std::vector<std::vector<cv::Point> > contours; // for contour detection
	Mat frame ;
	Mat backgroundImage;
	if(!cap.isOpened())
	{
		cout<<"The capture could not be initiated"<<endl;
		return 0 ;
	}
	namedWindow("Frame",1);
	while(1)
	{
    createTrackbar("History", "Frame", &history, 1000);
    createTrackbar("Shadows", "Frame", &shadowsDetect, 1);
    createTrackbar("LearningRate", "Frame", &learning_rate, 100);
    createTrackbar("VarThreshold", "Frame", &varThreshold, 64);
	BackgroundSubtractorMOG2* pMog2=new BackgroundSubtractorMOG2(history,varThreshold,shadowsDetect);
	while(true)
	{

		cap.read(frame);
		if(frame.empty())
		{
			cout<<"The frame is empty "<<endl;
			break;
		}

		pMog2->operator()(frame,fgMaskMOG2,(double)learning_rate/100);
		pMog2->getBackgroundImage(backgroundImage);
		findContours(fgMaskMOG2,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);

		//get the frame number and write on it 	on the current frame
		stringstream str ;
		rectangle(frame, cv::Point(10, 2), cv::Point(100,20),cv::Scalar(0,255,0), -1);
		//str << cap.get(CV_CAP_PROP_POS_FRAMES);
		string frameNumberString = str.str();
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
	    //show the current frame and the fg masks
		imshow("Frame", frame);
//		Mat binaryMask;
//		cv::threshold(fgMaskMOG2,binaryMask,2,255,CV_THRESH_BINARY);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		imshow("Background Image",backgroundImage);
		//get the input from the keyboard
		char key = waitKey(2);
		if(key==27)
			break;
	}
	if(waitKey(2)==32)
		break;
	}
	destroyAllWindows();
	return -1;
}
