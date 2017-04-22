#include <highgui.h>
#include <cv.h>
#include <stdio.h>

int main()
{
	reSize();

}

void reSize()
{
	//If the ROI is set in the source image then
	//that ROI will be resized to fit in the destination image. Likewise, if an ROI is set in the
	//destination image then the source will be resized to fit into the ROI.

	IplImage* img= cvLoadImage("cat.jpg",CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("image1",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("resized image",CV_WINDOW_AUTOSIZE);
	IplImage* resizeImage = cvCreateImage(cvSize(img->width/2, img->height/2),8,3);
	//cvResize(img,resizeImage,CV_INTER_LINEAR);
	cvResize(img,resizeImage,CV_INTER_NN);//using nearest neighbour concept
	cvShowImage("image1",img);
	cvShowImage("resized image",resizeImage);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&img);
	cvReleaseImage(&resizeImage);

}
