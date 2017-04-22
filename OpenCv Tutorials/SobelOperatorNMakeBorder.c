//The larger kernels
//give a better approximation to the derivative because the smaller kernels are very sen-
//sitive to noise.
//MAKE BORDER NOT WORKING
#include <highgui.h>
#include <cv.h>
#include <stdio.h>

IplImage* sobelLaplaceOperation(CvArr* src,CvArr* dest,int xorder,int yorder,int aperture_size);
int main()
{
	IplImage* input = cvLoadImage("bird.jpg",CV_LOAD_IMAGE_COLOR);
	IplImage* output = cvCreateImage(cvGetSize(input),8,3);
	//MakeBorder();

 	//sobelLaplaceOperation(input , output,0,1,CV_SCHARR);//complete it
	sobelLaplaceOperation(input , output,0,1,CV_SCHARR);
}

IplImage* sobelLaplaceOperation(CvArr* src,CvArr* dest,int xorder,int yorder,int aperture_size)
{
	//cvSobel(src,dest,xorder,yorder,aperture_size);
	cvLaplace(src, dest,3);
	cvNamedWindow("sobel input Image",0);
	cvNamedWindow("sobel output Image",0);
	cvShowImage("sobel input Image",src);
	cvShowImage("sobel output Image",dest);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dest);
}

void MakeBorder()
{
		IplImage* image = cvLoadImage("bird.jpg",CV_LOAD_IMAGE_COLOR);
		cvNamedWindow("originalImage",0);
		cvNamedWindow("outputImage",0);
		cvShowImage("original Image",image);
		IplImage* output = cvCreateImage(cvGetSize(image),8,3);
		cvCopyMakeBorder(image,output,cvPoint(2,2),IPL_BORDER_CONSTANT,cvScalarAll(1));//for a 5x5 kernel (N-1)/2 =2
		cvShowImage("outputImage",output);
		cvWaitKey(0);
		cvReleaseImage(&image);
		cvReleaseImage(&output);


}
