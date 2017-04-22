#include <highgui.h>
#include <cv.h>
#include <stdio.h>

int main()
{printf("here here");
	IplImage* img= cvLoadImage("cat.jpg",CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("image1",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("hi",CV_WINDOW_AUTOSIZE);
	IplImage* erosion = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,3);
	IplImage* dilate = cvCreateImage(cvSize(img->width , img->height),IPL_DEPTH_8U,3);

	IplConvKernel* kernel= cvCreateStructuringElementEx(4,4,2,2,CV_SHAPE_RECT,NULL  );
	cvShowImage("hi",NULL);
	cvErode(img,  erosion,NULL ,1);//erosion
	cvDilate(img, dilate,NULL ,1);//dialate
	cvNamedWindow("erosion",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dilate",CV_WINDOW_AUTOSIZE);
	cvMoveWindow("image1",200,300);
	cvShowImage("image1", img);
	cvShowImage("erosion",erosion);
	cvShowImage("dialte", dilate);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&img);
	cvReleaseImage(&dilate);
	cvReleaseImage(&erosion);
}
