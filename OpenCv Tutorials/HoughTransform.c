#include <highgui.h>
#include <cv.h>
#include <stdio.h>
#include <math.h>

//SHOULD WORK BUT THE MATH LIBRARY REQUIRES -LM WITH THE COMPILER ..DONE THAT BUT THERE ARE STILL SOME ERRORS


// standard Houghtransform (SHT)
// progressive probabilistic Hough transform (PPHT)

//rho are pixels and the units of theta are radians
int main()
{
	//canny edge detector
	IplImage* input =cvLoadImage("bird.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	CvMemStorage* storage =cvCreateMemStorage(0);
	CvSeq* sequence = cvHoughLines2(input,storage ,CV_HOUGH_STANDARD, 3, 3.14/2, 100,0,0 );

	double i, x,y,a,b,ro, theta;
	float* p;
	CvPoint point1 , point2;
	for(i = 0 ; i<sequence->total ;i++)
	{	p = (float*)cvGetSeqElem(sequence,i );

		ro = (double)p[0];
	    theta =(double)p[1];
	    a = cos(theta);
	    b = sin(theta);
	    x=ro*a;
	    y=ro*b;
	    point1 = cvPoint((x+1000*(-b)),(y+1000*a));
	    point2 = cvPoint((x-1000*(-b)),y-1000*b) ;
		cvLine(input,point1,point2,cvScalar(0,0,255,0),3,8,NULL);
	}
	cvNamedWindow("image",0);
	cvShowImage("image",input);
	cvWaitKey(0);
	cvDestroyAllWindows();
}


