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

int main()
{
	Mat frame ;
	VideoCapture cap(0);
	if(!cap.isOpened())
	{
		cout<< "The capture could not be started" <<endl;
	}
	int alpha=1;
	namedWindow("Image",CV_WINDOW_AUTOSIZE);
	createTrackbar("Alpha", "Image", &alpha, 10);

	cap.read(frame);
	Mat output(frame.rows,frame.cols,CV_32FC3,Scalar::all(0));

	//cout<<"Frame size"<<frame.channels() <<"destination size"<<output.channels() <<endl;
	while(!frame.empty())
	{

		cv::accumulateWeighted(frame,output,(double)((double)alpha/10),noArray());
		Mat img1 = output;
		img1.convertTo(img1,IPL_DEPTH_8U);
		imshow("Image",img1);
		cap.read(frame);
		int c = waitKey(2);
		if(c==27)
			break;
	}

	destroyAllWindows();
}
