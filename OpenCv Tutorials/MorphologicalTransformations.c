#include <highgui.h>
#include <cv.h>
#include <stdio.h>

int main()
{
	IplImage* img= cvLoadImage("cat.jpg",CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("ORIGINAL IMAGE",CV_WINDOW_AUTOSIZE);
	CvSize size = cvSize(img->width,img->height);
	cvNamedWindow("Open Image",CV_WINDOW_AUTOSIZE);
	IplConvKernel* kernel= cvCreateStructuringElementEx(4,4,2,2,CV_SHAPE_RECT,NULL  ); //4x4 kernel
	IplImage* opening_output = cvCreateImage(size, img->depth,img->nChannels);
	cvMorphologyEx(img,opening_output,NULL,kernel,CV_MOP_OPEN,1); //opening of image first erode and then dilate

	//for gradient
	cvNamedWindow("gradient of Image",CV_WINDOW_AUTOSIZE);

	IplImage* gradient_output = cvCreateImage(size, img->depth,img->nChannels);
	IplImage* temp = cvCreateImage(size, img->depth,img->nChannels);
	cvMorphologyEx(img,gradient_output,temp,kernel,CV_MOP_GRADIENT,1); //closing of image first dilate and then erode
	cvShowImage("gradient of Image",gradient_output);

	cvNamedWindow("Closing Image",CV_WINDOW_AUTOSIZE);

	//TOP HAT  :-difference between input image and its opening
	IplImage* tophat_output = cvCreateImage(size, img->depth,img->nChannels);
	cvMorphologyEx(img,tophat_output,temp,kernel,CV_MOP_TOPHAT,1); //closing of image first dilate and then erode
	cvNamedWindow("tophat Image",CV_WINDOW_AUTOSIZE);
	cvShowImage("tophat Image",tophat_output);

	//BLACKHAT :- difference between closing and its input image
		IplImage* blackhat_output = cvCreateImage(size, img->depth,img->nChannels);
		cvMorphologyEx(img,blackhat_output,temp,kernel,CV_MOP_BLACKHAT,1); //closing of image first dilate and then erode
		cvNamedWindow("blackhat Image",CV_WINDOW_AUTOSIZE);
		cvShowImage("blackhat Image",blackhat_output);

	IplImage* closing_output = cvCreateImage(size, img->depth,img->nChannels);
	cvMorphologyEx(img,closing_output,NULL,kernel,CV_MOP_CLOSE,1); //closing of image first dilate and then erode
	cvShowImage("Closing Image",closing_output);
	cvShowImage("ORIGINAL IMAGE",img);
	cvShowImage("Open Image",opening_output);
	cvMoveWindow("Closing Image",200,0);
	cvMoveWindow("ORIGINAL IMAGE",0,0);
	cvMoveWindow("Open Image",0,200);
	cvMoveWindow("gradient of Image",400,200);
	cvWaitKey(0);
	cvDestroyAllWindows();
}
