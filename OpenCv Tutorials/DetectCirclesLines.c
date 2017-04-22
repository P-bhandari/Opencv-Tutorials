#include <highgui.h>
#include <cv.h>
#include <stdio.h>
#include <math.h>
// standard Houghtransform (SHT)
// progressive probabilistic Hough transform (PPHT)
//The threshold value is the
//value in the accumulator plane that must be reached for the routine to report a line

//for circle -> A center is kept if it has sufficient support from the nonzero pixels in the edge
//image and if it is a sufficient distance from any previously selected center.

//rho are pixels and the units of theta are radians
int main()
{
	//canny edge detector
/*	IplImage* input =cvLoadImage("bird.jpg",CV_LOAD_IMAGE_GRAYSCALE);
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
	cvDestroyAllWindows();*/
	IplImage* input = cvLoadImage("circles.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	getCircles(input);

}


//http://stackoverflow.com/questions/10653433/opencv-error-in-cvhoughcircles-usage
void getCircles(IplImage* input)
{
	IplImage* clone = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 3);
	cvNamedWindow("original image",0);
	cvNamedWindow("output image",0);
	cvShowImage("original image",input);
	CvMemStorage* storage = cvCreateMemStorage(0);
	cvCanny(input,input, 20, 200,3);
	CvSeq* result = cvHoughCircles(input,storage,CV_HOUGH_GRADIENT,8,10,100,300,0,1000);
	int i ;
	cvShowImage("original image",clone);
	float* p ;
	printf("result total %d\n",result->total );
	for(i=0 ; i <result->total;i++)
	{
		printf("f\n");
		p= (float*)cvGetSeqElem(result,i);
		CvPoint pt = cvPoint(cvRound(p[0]),cvRound(p[1]));
		printf("point%d x = %d  and y = %d",i,pt.x,pt.y);
		cvCircle(clone,pt,10,cvScalar(0,255,0,0),-1,8,0);
	}

	cvShowImage("output image",clone);
	cvWaitKey(0);
	cvReleaseImage(&input);
	cvReleaseImage(&clone);

}

