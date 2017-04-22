#include <highgui.h>
#include <cv.h>
#include <stdio.h>

int main()
{
	//canny edge detector
	IplImage* input =(char*)cvLoadImage("bird.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	canny(input);
}

void canny(IplImage* input)
{
	cvNamedWindow("original image",0);
	cvNamedWindow("output image",0);
	//cvConvertImage(input,input,)
	input->depth = 8 ;
	IplImage* output =cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,1);
	cvCanny(input,output,10,180,3);
	cvShowImage("original image",input);
	cvShowImage("output image",output);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&input);
	cvReleaseImage(&output);
}

