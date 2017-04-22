#include <highgui.h>
#include <cv.h>
#include <stdio.h>


//cvThreshold() function handles only 8-bit or floating-point grayscale source images.
IplImage* Threshold(IplImage* source , IplImage* dest,float threshold,float max_value,int threshold_type);
IplImage* AdaptiveThreshold(IplImage* source , IplImage* dest,float max_Value,int adaptive_method,int threshold_type,int block_size,float param1);
int main()
{

   IplImage* image = cvLoadImage("bird.jpg",CV_LOAD_IMAGE_COLOR);
   get_rgb(image);
   IplImage* image2 =cvLoadImage("chessboard.jpg",CV_LOAD_IMAGE_GRAYSCALE);
   func1(image2); //to show the difference between threshold and adaptive threshold
}

void func1(IplImage* input)
{
	IplImage* output = cvCreateImage(cvGetSize(input),8,1);
	cvNamedWindow("GRAYSCALE original",0);
	cvNamedWindow("afterAdThresh GRAYSCALE",0);
	output = AdaptiveThreshold(input ,output,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,19,0.5);
	cvShowImage("GRAYSCALE original",input);
	cvShowImage("afterAdThresh GRAYSCALE",output);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&input);
	cvReleaseImage(&output);
}
void get_rgb(IplImage* input)
{
	IplImage* r = cvCreateImage(cvGetSize(input),8,1);
	IplImage* g = cvCreateImage(cvGetSize(input),8,1);
	IplImage* b = cvCreateImage(cvGetSize(input),8,1);

	cvSplit(input,b,g,r,NULL);
	cvNamedWindow("original image",0);
	cvNamedWindow("original r",0);
	cvNamedWindow("original g",0);
	cvNamedWindow("original b",0);
	cvNamedWindow("original image",input);
	cvShowImage("original r",r);
	cvShowImage("original g",g);
	cvShowImage("original b",b);
	cvShowImage("original image",input);
	cvWaitKey(0);
	cvNamedWindow("afterthresh r",0);
	cvNamedWindow("afterthresh g",0);
	cvNamedWindow("afterthresh b",0);
	r = Threshold(r,r,60,255,CV_THRESH_BINARY);
	g = Threshold(g,g,60,255,CV_THRESH_BINARY);
	b = Threshold(b,b,60,255,CV_THRESH_BINARY);
	cvShowImage("afterthresh r",r);
	cvShowImage("afterthresh g",g);
	cvShowImage("afterthresh b",b);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&input);
	cvReleaseImage(&r);
	cvReleaseImage(&g);
	cvReleaseImage(&b);
}


IplImage* Threshold(IplImage* source , IplImage* dest,float threshold,float max_value,int threshold_type)
{
	cvThreshold(source,dest,threshold,max_value, threshold_type);
	return dest ;
}
IplImage* AdaptiveThreshold(IplImage* source , IplImage* dest,float max_Value,int adaptive_method,int threshold_type,int block_size,float param1)
{
	cvAdaptiveThreshold(source ,  dest,max_Value, adaptive_method,threshold_type, block_size, param1);
	return dest;
}





