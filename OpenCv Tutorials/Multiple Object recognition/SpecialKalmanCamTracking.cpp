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
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>
#include <string>


using namespace cv;
using namespace std;

////////
//change all areas to bounding rect area
/////
Mat backgroundSubstraction(Mat frame,Mat background)
{
	Mat srctmp,gray;



	int erodeValue = 7;
		int dilateValue = 2;
		int thresholdValue = 17;
//	cvCreateTrackbar("Erode", "Control", &erodeValue, 13); //Saturation (0 - 255)
//	cvCreateTrackbar("threshold", "Control", &thresholdValue, 100); //Saturation (0 - 255)
//	cvCreateTrackbar("Dilate", "Control", &dilateValue, 13); //Saturation (0 - 255)
	absdiff(frame, background, srctmp);
	cvtColor(srctmp, gray, CV_BGR2GRAY);
	imshow("New", srctmp);
	imshow("frame",frame);
	threshold(gray, gray, thresholdValue, 255, CV_THRESH_BINARY);//60
	dilate(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(dilateValue,dilateValue)) );//3
	erode(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(erodeValue, erodeValue)) );



	return gray ;

//		Mat Background;				// For Extracted Background
//		Mat Foreground;				// Foreground Mask Thingy
//		Mat foreground;
//		BackgroundSubtractorMOG2 bg;
//	    bg = cv::BackgroundSubtractorMOG2(history,varThreshold,shadowsDetect);
//		bg.operator ()(frame, Foreground);
//		threshold( Foreground, foreground, 1, 255,THRESH_BINARY );
//
//
//		erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(erodeValue, erodeValue)) );
//
//		dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(dilateValue,dilateValue)) );//3
//		bg.getBackgroundImage(Background);
//		imshow("Foreground",Foreground);
//return Foreground;
}

class Pedestrian
{
	public:
	 Rect roi;
	 int label;
	 MatND hist;
	 Point centroid;
	 Scalar color;
	 cv::KalmanFilter KF;
	 float area;
	 int occlusionTimeStamp;
	 int trigger;

	 //area of the starting of occlusion

	 void KalmanInit()
	 {
		 	KF.init(4, 2, 0);

		    KF.statePre.at<float>(0) = centroid.x;
			KF.statePre.at<float>(1) = centroid.y;
			KF.statePre.at<float>(2) = 0;
			KF.statePre.at<float>(3) = 0;

			KF.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);

	        setIdentity(KF.measurementMatrix);
	        setIdentity(KF.processNoiseCov, Scalar::all(0.5));
	        setIdentity(KF.measurementNoiseCov, Scalar::all(0.05));
	        setIdentity(KF.errorCovPost, Scalar::all(.1));
	 }

	 Point kalmanPredict(int occlusion)
	 {
	         Mat prediction = KF.predict();
	         Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
//	         if(occlusion ==1)
//	        	 KF.measurementMatrix.setTo(0);
//	         else
	 	        setIdentity(KF.measurementMatrix);


	         KF.statePre.copyTo(KF.statePost);
	         KF.errorCovPre.copyTo(KF.errorCovPost);

	         return predictPt;
	 }

	 Point kalmanCorrect()
	 {
		     cv::Mat_<float> measurement(2, 1);

		     measurement = Mat_<float>::zeros(2, 1);

	         measurement(0) = centroid.x;
	         measurement(1) = centroid.y;
	         Mat estimated = KF.correct(measurement);
	         Point statePt(estimated.at<float>(0), estimated.at<float>(1));
	         return statePt;
	 }

};
vector<Pedestrian> ped ;
void detectPedestrians(Mat gray, Mat frame);
void labelPedestrians(vector<Pedestrian> newPed, Mat frame, Mat gray);
float EuclideanDistance(Point a, Point b);
void makeBoundingBoxes(Mat frame);
Mat calhis(Mat image, Rect object);


Rect tracko(Mat track, Mat hist, Rect selection);

int indexLabel = 0 ;
int main()
{
	VideoCapture cap("Multi Pedestrian.MP4") ;
	Mat frame;
	if(!cap.isOpened())
	{
		cout<< "The vedio file cannot be loaded" <<endl;
		return -1;
	}
	namedWindow("New",CV_WINDOW_KEEPRATIO);
	namedWindow("frame",CV_WINDOW_KEEPRATIO);

	cap.set(CV_CAP_PROP_POS_MSEC,0000); //start from 0sec
	cap.read(frame);

	Mat background = frame.clone() ;
	//background.resize(Size(640,480));
	//imshow("background",background);
	cap.read(frame);
	int k =0;
	namedWindow("Control",CV_WINDOW_KEEPRATIO);
	while(!frame.empty())
	{
		cap.read(frame);

	    Mat gray = backgroundSubstraction(frame,background);
		imshow("Control", gray);
		detectPedestrians(gray,frame);
		char key = waitKey(2);
		if(key==27 )
		{
				break;
		}
	}
}

void detectPedestrians(Mat gray, Mat frame)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Moments> mu(contours.size());
    vector<Point2f> mc(contours.size());
	vector<Pedestrian> newPed;
	Mat output ;
	frame.copyTo(output);
	findContours(gray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	for(int i = 0 ; i<contours.size();i++)
	{
		double area = contourArea(contours[i],false);
		//cout<<"area"<<i<<" = "<<area<<endl;

		if(area > 10000)			//assumed anything of the given threshold area is a human/pedestrian
		{
				Rect rect = boundingRect(contours[i]);
				Moments mu = moments(contours[i]);
				Point centroid = Point2f(mu.m10/mu.m00,mu.m01/mu.m00);
				Pedestrian x ;
				x.area = area ;
				x.centroid=centroid ;
				x.roi =rect;
				newPed.push_back(x);
		}
	}

	labelPedestrians(newPed,frame,gray);
}


void labelPedestrians(vector<Pedestrian> newPed, Mat frame, Mat gray)
{

	  printf("Nline number %d in file \n", __LINE__);

		//occlusion or banda gayab hai
		vector<int> OcclusionIndexes ;
		float maxArea = 0;
		for(int i = 0 ; i< ped.size();i++)
		{
			if(ped.at(i).area >maxArea)
			{
				maxArea=ped.at(i).area;
			}
		}

		//current occlusion blobs
		if(ped.size()>0)
		{

		for(int i =0;i<newPed.size();i++)
		{
			if(newPed.at(i).area> 1.2*maxArea)
			{
				//gand mar gayi occlusion
				OcclusionIndexes.push_back(i);
			}
		}
		cout<<"line 228"<<endl;
		}
		bool isOcclusion = false ;
		if(OcclusionIndexes.size()>0)
			isOcclusion = true ;
		//find the number of people selected
		if(isOcclusion == true)
		{

			cout<<"line 245"<<endl;
			vector<int> IndexPreviousPedsThatAreNowOccluded ;
			for(int i = 0 ; i< ped.size();i++)
			{
				cout<<"pedsize"<<ped.size()<<endl;
				for(int j=0;j<OcclusionIndexes.size();j++)
				{
					cout<<"EuclideanDistance = "<<EuclideanDistance(ped.at(i).centroid,newPed.at(OcclusionIndexes.at(j)).centroid) << endl;
					cout<<"Height of box = "<<newPed.at(OcclusionIndexes.at(j)).roi.height << endl;
					if(EuclideanDistance(ped.at(i).centroid,newPed.at(OcclusionIndexes.at(j)).centroid) < (newPed.at(OcclusionIndexes.at(j)).roi.height)*4)
					{
						//cout<<"EuclideanDistance = "<<EuclideanDistance(ped.at(i).centroid,newPed.at(OcclusionIndexes.at(j)).centroid) << endl;
						//cout<<"Height of box = "<<newPed.at(OcclusionIndexes.at(j)).roi.height << endl;

						IndexPreviousPedsThatAreNowOccluded.push_back(i);
					}

				}
			}
			cout<<" IndexPreviousPedsThatAreNowOccluded = "<<IndexPreviousPedsThatAreNowOccluded.size()<<endl;

			cout<<" line 258"<<endl;

			vector<int> IndexNewPedThatAreMatched;
			for(int i=0;i<ped.size();i++)
			{
				int useTracko=1;
				for(int j=0;j<IndexPreviousPedsThatAreNowOccluded.size();j++)
				{
					if(i==IndexPreviousPedsThatAreNowOccluded.at(j))
						useTracko = 0 ;
				}
				if(useTracko==1)
				{
					int didnotMatch=1;
					for(int j =0; j<newPed.size();j++)
					{
						int track=1;
						for(int x =0;x<OcclusionIndexes.size();x++)
						{
							if(j == OcclusionIndexes.at(x))
								track=0;
						}
						if(track ==1)
						{
							Mat object2 = Mat::zeros(frame.size(),frame.type());
							Mat roi2;
							roi2 = frame(newPed.at(j).roi);


							roi2.copyTo(object2(newPed.at(j).roi));

							Rect rectan = tracko(object2,ped.at(i).hist,ped.at(i).roi);
							if(rectan.area() >1000)
							{
								IndexNewPedThatAreMatched.push_back(j);
								ped.at(i).kalmanPredict(0);

								ped.at(i).centroid =newPed.at(j).centroid;
								ped.at(i).kalmanCorrect();
								ped.at(i).hist = calhis(frame,rectan);
								ped.at(i).roi = rectan;
								didnotMatch =0;
								break;
							}

						}


					}
					if(didnotMatch==1)
					{
																	///delete pedestrian
						ped.erase(ped.begin()+i);
					}
					}

			}
			cout<<"line 315"<<endl;


			for(int i = 0 ; i<newPed.size();i++)
			{
				int newBanda = 1;
				for(int j  =0 ; j< OcclusionIndexes.size();j++)
				{
					if(i == OcclusionIndexes.at(j))
						newBanda = 0 ;
				}
				if(newBanda==1)
				{
					for(int j =0 ;j<IndexNewPedThatAreMatched.size();j++)
					{
						if(i == IndexNewPedThatAreMatched.at(j))
							newBanda = 0 ;
					}
				}

				if(newBanda ==1)
				{

				    ped.push_back(newPed.at(i));
				    indexLabel++;
				    ped.at(ped.size()-1).label = indexLabel ;
				    ped.at(ped.size()-1).KalmanInit();

				}
			}



			for(int i = 0 ; i<IndexPreviousPedsThatAreNowOccluded.size();i++)
				{

					Point p =ped.at(IndexPreviousPedsThatAreNowOccluded.at(i)).kalmanPredict(1);
					ped.at(IndexPreviousPedsThatAreNowOccluded.at(i)).centroid=p;
					ped.at(IndexPreviousPedsThatAreNowOccluded.at(i)).kalmanCorrect();
					Rect current=ped.at(IndexPreviousPedsThatAreNowOccluded.at(i)).roi;
					Rect update = Rect(p.x - current.width / 2, p.y - current.height / 2, current.width, current.height);

					ped.at(IndexPreviousPedsThatAreNowOccluded.at(i)).roi=update;


				}



//			vector<int> NotoccludedPeoplePreFrame;
//			for(int i=0; i<newPed.size();i++)
//			{
//				int yes=0;
//				for(int j=0; j<OcclusionIndexes.size();j++)
//				{
//					if(i==OcclusionIndexes.at(j))
//						yes=1;
//				}
////				if(yes==0)
////				{
////						int index = -1;
////						int previousDistance = 1000000;
////						int minDistance = 0 ;
////						for(int j = 0 ; j<ped.size();j++)
////						{
//////							int newdistance = EuclideanDistance(newPed.at(i).centroid,ped.at(j).centroid);
//////							if(newdistance<previousDistance)
//////							{
//////								index = j ;
//////
//////								minDistance = newdistance;
//////							}
//////							previousDistance = newdistance ;
////
////
////
////
////						}
////						NotoccludedPeoplePreFrame.push_back(index);
////						//correct kalman ??
////						ped.at(index).kalmanPredict(0);
////						ped.at(index).area = newPed.at(i).area ;
////						ped.at(index).centroid = newPed.at(i).centroid;
////						ped.at(index).kalmanCorrect();
////						ped.at(index).hist = calhis(frame,newPed.at(i).roi);
////
////						ped.at(index).color = newPed.at(i).color ;
////						ped.at(index).label = newPed.at(i).label;
////						ped.at(index).roi = newPed.at(i).roi;
////
////			     }
////
//
//		}
//		cout<<"line 282"<<endl;
//
//		vector<int> occludedPreFrame;
//		for(int i=0; i<ped.size();i++)
//		{
//			int flag = -1;
//			for(int j = 0 ; j<NotoccludedPeoplePreFrame.size();j++)
//			{
//				if(i == NotoccludedPeoplePreFrame.at(j))
//					flag = 1;
//			}
//			if(flag == -1)
//				occludedPreFrame.push_back(i);
//		}
//		cout<<"line 295"<<endl;
//		for(int i = 0 ; i<occludedPreFrame.size();i++)
//		{
//
//			Point p =ped.at(occludedPreFrame.at(i)).kalmanPredict(1);
//			ped.at(occludedPreFrame.at(i)).centroid=p;
//			ped.at(occludedPreFrame.at(i)).kalmanCorrect();
//			Rect current=ped.at(occludedPreFrame.at(i)).roi;
//			Rect update = Rect(p.x - current.width / 2, p.y - current.height / 2, current.width, current.height);
//			ped.at(occludedPreFrame.at(i)).roi=update;
//
//
//		}


//
//		Mat roiImageOccludedBlobs;
//		Mat objectroi = Mat::zeros(frame.size(),frame.type());
//
//
//		//putting blobs in new image
//		for(int j=0;j<OcclusionIndexes.size();j++)
//		{
//			roiImageOccludedBlobs = frame(newPed.at(OcclusionIndexes.at(j)).roi);
//			roiImageOccludedBlobs.copyTo(objectroi(newPed.at(OcclusionIndexes.at(j)).roi));
//
//		}


		//occluded roi recieved


		//trigger

		//go for indivisual tracking of the pedestrians
//		for(int i=0;i<occludedPreFrame.size();i++)
//		{
//			    Rect NewobjectRoi ;
//			    Point centroid;
//			    NewobjectRoi= tracko(objectroi,ped.at(occludedPreFrame.at(i)).hist,ped.at(occludedPreFrame.at(i)).roi );
//			    //implement Kalman
//			    Point p1(NewobjectRoi.tl()) ;
//			    Point p2(NewobjectRoi.br());
//			    int a=(p1.x+p2.x)/2;.
//			    int b=(p1.y+p2.y)/2;
//			    Point p3(a,b);
//			    centroid=p3;
//
//			    // correct it ped.at(i).KF
//			    ped.at(occludedPreFrame.at(i)).area = NewobjectRoi.area() ;
//			    ped.at(occludedPreFrame.at(i)).centroid = centroid;
//			    ped.at(occludedPreFrame.at(i)).occlusionTimeStamp++ ;
//		}
		}
		else
		{
			cout<<"line 349"<<endl;

			if(ped.size() == 0 )
			{			cout<<"line 352"<<endl;

				for(int i =0;i<newPed.size();i++)
				{
					ped.push_back(newPed.at(i));
					ped.at(i).KalmanInit();
					ped.at(i).hist = calhis(frame,ped.at(i).roi);
					indexLabel++;
					ped.at(i).label = indexLabel;
				}
			}
			else if(ped.size() == newPed.size())
			{
				cout<<"line 347"<<endl;

				for(int i=0; i<newPed.size();i++)
						{
							int index = -1;
							int previousDistance = 1000000;
							int minDistance = -1 ;
							for(int j=0 ; j<ped.size();j++)
							{
								int newdistance = EuclideanDistance(newPed.at(i).centroid,ped.at(j).centroid);
								if(newdistance<previousDistance)
								{
									index = j ;
									minDistance = newdistance;
								}
								previousDistance = newdistance ;
							}

									//code later kya karna hai

							ped.at(index).kalmanPredict(0);

							ped.at(index).area = newPed.at(i).area ;
							ped.at(index).centroid = newPed.at(i).centroid;
							cout<<"line 370"<<endl;

							ped.at(index).kalmanCorrect() ;
							cout<<"line 373"<<endl;
							ped.at(index).hist = calhis(frame,newPed.at(i).roi);

							ped.at(index).color = newPed.at(i).color ;
							cout<<"line 377"<<endl;

							//ped.at(index).label = newPed.at(i).label;
							ped.at(index).roi = newPed.at(i).roi;


						}
				cout<<"line 378"<<endl;

			}
			else  if(ped.size() < newPed.size())
					{				cout<<"line 405"<<endl;

						vector<int> indexes ;
						for(int i = 0 ; i<ped.size();i++)
						{
							int index = -1;
							int previousDistance = 1000000;
							for(int j = 0 ; j<newPed.size();j++)
							{
								int newdistance = EuclideanDistance(newPed.at(j).centroid,ped.at(i).centroid);
								if(newdistance<previousDistance)
								{
									index = j ;

								//	minDistance = newdistance;
								}
								previousDistance = newdistance ;
							}
							indexes.push_back(index);
							ped.at(i).kalmanPredict(0);

							ped.at(i).area = newPed.at(index).area ;
							ped.at(i).centroid = newPed.at(index).centroid;
							ped.at(i).kalmanCorrect();
							ped.at(i).hist = calhis(frame,newPed.at(index).roi);
							ped.at(i).color = newPed.at(index).color ;
							//ped.at(i).label = newPed.at(index).label;
							ped.at(i).roi = newPed.at(index).roi;
						}


				for(int i=0;i< newPed.size();i++)
				{
					int newPedestrian = -1 ;
					for(int j =0;j< indexes.size();j++)
					{
						if(indexes.at(j)==i)
							newPedestrian = 0;
					}
					if(newPedestrian !=0)
					{
						ped.push_back(newPed.at(i));	//kalman initialize ??
						ped.at(ped.size()-1).KalmanInit();
						indexLabel++;
						ped.at(ped.size()-1).label = indexLabel;
					}
				}
				}
			else	//koi banda gayab hogya hai
			{				cout<<"line 452"<<endl;

				vector <int> indexes;
				for(int i = 0 ; i<newPed.size();i++)
				{
							int index = -1;
							int previousDistance = 1000000;
						    int minDistance =0;
							for(int j = 0 ; j<ped.size();j++)
							{
								int newdistance = EuclideanDistance(ped.at(j).centroid,newPed.at(i).centroid);
							    if(newdistance<previousDistance)
								{
									//ped =newPed;
									index = j ;

						        	minDistance = newdistance;
							    }
									previousDistance = newdistance ;
							}
							indexes.push_back(index);
							ped.at(index).kalmanPredict(0);

							ped.at(index).area = newPed.at(i).area ;
							ped.at(index).centroid = newPed.at(i).centroid;
							ped.at(index).kalmanCorrect();
							ped.at(index).hist = calhis(frame,newPed.at(i).roi);
							ped.at(index).color = newPed.at(i).color ;
							//ped.at(index).label = newPed.at(i).label;
							ped.at(index).roi = newPed.at(i).roi;

			}
							cout<<"line 484"<<endl;

					for(int i = 0 ; i <ped.size();i++)
					{
						int x = 0;
						for(int j = 0 ;  j<indexes.size();j++)
						{
							if(i == indexes.at(j))
								x =1;
						}
						if(x ==0)
							{
								ped.erase(ped.begin()+i);
								cout<<"pedestrian Erased" << endl;
							}
					}
			}
		}



//	///
//	if(ped.size() == newPed.size() )
//	{
//		for(int i=0; i<newPed.size();i++)
//		{
//			int index = -1;
//			int previousDistance = 1000000;
//			int minDistance = -1 ;
//			for(int j=0 ; j<ped.size();j++)
//			{
//				int newdistance = EuclideanDistance(newPed.at(i).centroid,ped.at(j).centroid);
//				if(newdistance<previousDistance)
//				{
//					index = j ;
//					minDistance = newdistance;
//				}
//				previousDistance = newdistance ;
//			}
//			if(minDistance > 7)
//			{
//					//code later kya karna hai
//			}
//			ped.at(index).KF = newPed.at(i).KF ;
//			ped.at(index).area = newPed.at(i).area ;
//			ped.at(index).centroid = newPed.at(i).centroid;
//			ped.at(index).hist = newPed.at(i).hist ;
//			ped.at(index).color = newPed.at(i).color ;
//			ped.at(index).label = newPed.at(i).label;
//			ped.at(index).roi = newPed.at(i).roi;
//
//
//		}
//	}
//	else if(ped.size() < newPed.size())
//	{
//		vector<int> indexes ;
//		for(int i = 0 ; i<ped.size();i++)
//		{
//			int index = -1;
//			int previousDistance = 1000000;
//			for(int j = 0 ; j<newPed.size();j++)
//			{
//				int newdistance = EuclideanDistance(newPed.at(j).centroid,ped.at(i).centroid);
//				if(newdistance<previousDistance)
//				{
//					index = j ;
//
//				//	minDistance = newdistance;
//				}
//				previousDistance = newdistance ;
//			}
//			indexes.push_back(index);
//			ped.at(i).KF = newPed.at(index).KF ;	//correct kalman ??
//			ped.at(i).area = newPed.at(index).area ;
//			ped.at(i).centroid = newPed.at(index).centroid;
//			ped.at(i).hist = newPed.at(index).hist ;
//			ped.at(i).color = newPed.at(index).color ;
//			ped.at(i).label = newPed.at(index).label;
//			ped.at(i).roi = newPed.at(index).roi;
//	else if(ped.size() < newPed.size())
		//	{
		//		vector<int> indexes ;
		//		for(int i = 0 ; i<ped.size();i++)
		//		{
		//			int index = -1;
		//			int previousDistance = 1000000;
		//			for(int j = 0 ; j<newPed.size();j++)
		//			{
		//				int newdistance = EuclideanDistance(newPed.at(j).centroid,ped.at(i).centroid);
		//				if(newdistance<previousDistance)
		//				{
		//					index = j ;
		//
		//				//	minDistance = newdistance;
		//				}
		//				previousDistance = newdistance ;
		//			}
		//			indexes.push_back(index);
		//			ped.at(i).KF = newPed.at(index).KF ;	//correct kalman ??
		//			ped.at(i).area = newPed.at(index).area ;
		//			ped.at(i).centroid = newPed.at(index).centroid;
		//			ped.at(i).hist = newPed.at(index).hist ;
		//			ped.at(i).color = newPed.at(index).color ;
		//			ped.at(i).label = newPed.at(index).label;
		//			ped.at(i).roi = newPed.at(index).roi;
		//		}		}
//
//		for(int i=0;i< newPed.size();i++)
//		{
//			int newPedestrian = -1 ;
//			for(int j =0;j< indexes.size();j++)
//			{
//				if(indexes.at(j)==i)
//					newPedestrian = 0;
//			}
//			if(newPedestrian !=0)
//			{
//				ped.push_back(newPed.at(i));	//kalman initialize ??
//
//			}
//		}
//
//	}
//	else
//	{
//
//
//
//	}

	//print out all the data

			Mat tempa = frame.clone();
			makeBoundingBoxes(tempa);


}
void makeBoundingBoxes(Mat frame)
{
	for(int i =0;i<ped.size();i++)
	{

		Rect x= Rect(ped.at(i).centroid.x - ped.at(i).roi.width/2,ped.at(i).centroid.y - ped.at(i).roi.height/2-1,ped.at(i).roi.width,ped.at(i).roi.height);
		rectangle(frame,ped.at(i).roi , Scalar(0,0,255), 2, 2, 0);

		stringstream ss;
							//if(index>0)
							ss << ped.at(i).label;
							string str = ss.str();
							putText(frame, str , Point(ped.at(i).centroid.x - ped.at(i).roi.width/2,ped.at(i).centroid.y - ped.at(i).roi.height/2 -5 ), FONT_HERSHEY_DUPLEX, 1.0, ped.at(i).color, 2.0);
	}
	imshow("OUTPUT",frame);
}

float EuclideanDistance(Point a, Point b)
{
	return (std::sqrt(pow(a.x - b.x,2.0) + pow(a.y-b.y,2.0)));

}
int vmin = 50, vmax = 256, smin = 50;
Mat calhis(Mat image, Rect object)
{
	Rect trackWindow;
	int hsize = 16;//previously16
	float hranges[] = { 0, 150 };
	const float* phranges = hranges;

	Mat frame12, hsv, hue, mask, hist;
	//bool paused = false;

	int ch[] = { 0, 0 };
	int _vmin = vmin, _vmax = vmax;
	//frame.copyTo(image);
	//this is for histogram calculation
	cvtColor(image, hsv, COLOR_BGR2HSV);

	inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
		Scalar(180, 256, MAX(_vmin, _vmax)), mask);

	hue.create(hsv.size(), hsv.depth());
	mixChannels(&hsv, 1, &hue, 1, ch, 1);
	//imshow("Histogram", image);

	Mat roi(hue, object), maskroi(mask, object);
	calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
	normalize(hist, hist, 0, 255, NORM_MINMAX);
	return hist;
}
Rect tracko(Mat track, Mat hist, Rect selection)
{
	Rect trackWindow;
	int hsize = 16;//previously16
	float hranges[] = { 0, 150 };
	const float* phranges = hranges;
	Mat backproj, hsv1, hue1, mask1;
	//	Rect trackWindow;
	//bool paused = false;

	int ch[] = { 0, 0 };
	int _vmin = vmin, _vmax = vmax;

	cvtColor(track, hsv1, COLOR_BGR2HSV);
	inRange(hsv1, Scalar(0, smin, MIN(_vmin, _vmax)),
		Scalar(180, 256, MAX(_vmin, _vmax)), mask1);//previousli180
	hue1.create(hsv1.size(), hsv1.depth());
	mixChannels(&hsv1, 1, &hue1, 1, ch, 1);



	trackWindow = selection;

	calcBackProject(&hue1, 1, 0, hist, backproj, &phranges);
	backproj &= mask1;

	RotatedRect trackBox = CamShift(backproj, trackWindow,
		TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
	return trackBox.boundingRect();
}

