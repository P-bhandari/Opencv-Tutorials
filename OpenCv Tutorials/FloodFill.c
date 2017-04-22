#include <highgui.h>
#include <cv.h>
#include <stdio.h>

int main()
{
	FloodFill();

}

void FloodFill()
{
	IplImage* image = cvLoadImage("cat.jpg", CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("original image",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("flood filled image",CV_WINDOW_AUTOSIZE);
	IplImage* clone =cvCloneImage(image);
	CvPoint seedPoint = cvPoint(20,20); //the seed point is on CV_FLOODFILL_FIXED_RANGE is used to compare with
	CvScalar newVal = cvScalar(0,0,0,0);
	CvScalar loDiff = cvScalar(10,10,10,0);
	CvScalar upDiff = cvScalar(20,20,20,0);
	int flag = 4|(255<<8 );
	IplImage* mask =cvCreateImage(cvSize(image->width+2 , image->height+2),IPL_DEPTH_8U,1); //has to be single channel 8 bit image with the size as shown
	//cvFloodFill(clone,seedPoint ,newVal,loDiff,upDiff,NULL,flag,NULL);  the loDiff and upDiff are taken into account
	cvFloodFill(clone,seedPoint ,newVal,loDiff,upDiff,NULL,CV_FLOODFILL_FIXED_RANGE
,mask);
	cvShowImage("original image",image);
	cvShowImage("flood filled image",clone);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&image);
	cvReleaseImage(&clone);
}
