#include <highgui.h>
#include <cv.h>
#include <stdio.h>


int main()
{

   IplImage* image = cvLoadImage("bird.jpg",CV_LOAD_IMAGE_COLOR);
   cvNamedWindow("original Image",0);
   cvShowImage("original Image",image);
   cvNamedWindow("5x5 smoothing",0);
   cvNamedWindow("11x11 smoothing",0);
   IplImage* five_S = cvCreateImage(cvGetSize(image),8,3);
   IplImage* eleven_S = cvCreateImage(cvGetSize(image),8,3);
   cvSmooth(image,five_S,CV_GAUSSIAN,5,5,0,0);
   cvShowImage("5x5 smoothing",five_S);
   cvWaitKey(0);
   cvSmooth(five_S,five_S,CV_GAUSSIAN,5,5,0,0);
   cvShowImage("5x5 smoothing",five_S);
   cvWaitKey(0);
   cvSmooth(image,eleven_S,CV_GAUSSIAN,11,11,0,0);
   cvShowImage("11x11 smoothing",eleven_S);
   cvWaitKey(0);
   cvDestroyAllWindows();
   cvReleaseImage(&image);
   cvReleaseImage(&five_S);
   cvReleaseImage(&eleven_S);
}

#include <highgui.h>
#include <cv.h>
#include <stdio.h>


int main()
{

   IplImage* image = cvCreateImage(cvSize(100,100),8,1);
   cvSetZero(image);
   //cvmSet(image,50,50,255);
   cvSetReal2D( image, 50, 50, 255 );
  /* int* imageData = (int)image->imageData ;
   imageData[50*image->widthStep+50]=255;*/
   cvNamedWindow("original Image",0);
   cvShowImage("original Image",image);
   cvSmooth(image,image,CV_GAUSSIAN,5,5,0,0);
   cvSmooth(image,image,CV_GAUSSIAN,5,5,0,0);
   cvWaitKey(0);
   cvNamedWindow("new",0);
   cvShowImage("new",image);
   cvWaitKey(0);
   cvDestroyAllWindows();
   cvReleaseImage(&image);

}

