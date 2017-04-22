#include <cmath>
#include <cstdio>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>
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

//Mean shift algorithm is implemented ...It helps in grouping up similar pixels for
//faster computation in other algorithms.





//******Function skeleton
//void pyrMeanShiftFiltering(InputArray src, OutputArray dst, double sp,
//							double sr, int maxLevel=1,
//							TermCriteria termcrit=TermCriteria( TermCriteria::MAX_ITER+TermCriteria::EPS,5,1) );
int main()
{
	Mat input = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	Mat dest ;
	int sp=10;	//– The spatial window radius.
	int sr=25;	//– The color window radius.

	int maxLevel =1 ;
	TermCriteria termcrit=TermCriteria( TermCriteria::MAX_ITER+TermCriteria::EPS,5,1);
	namedWindow("Input Image",1);
	imshow("Input Image",input);
//    createTrackbar("SaptialWindowRadius", "Input Image", &sp, input.rows/2);
//    createTrackbar("ColorWindowRadius", "Input Image", &sr, 255);

    while(1)
    {
    	pyrMeanShiftFiltering(input,dest,sp,sr,1,termcrit);
    	imshow("Output",dest);
    	int c = waitKey(400);
    	if(c==27)
    	{
    		break;
    	}

    }


}
