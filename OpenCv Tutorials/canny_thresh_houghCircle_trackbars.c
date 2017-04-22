#include <stdio.h>
#include <stdlib.h>
#include <highgui.h>
#include <cv.h>

void switch_min_dist( int position );
void switch_dp( int position );
void switch_callback_h( int position );
void switch_callback_l( int position );
void switch_threshold( int position );
void switch_threshold_max( int position );


int threshold_switch_value = 0;
int thresholdInt = 0;
int threshold_max_switch_value = 0;
int threshold_maxInt = 0;
int min_dist_switch_value = 0;
int min_distInt = 0;
int dp_switch_value = 0;
int dpInt = 0;
int high_switch_value = 0;
int highInt = 0;
int low_switch_value = 0;
int lowInt = 0;

void switch_threshold( int position ){
    thresholdInt = position;
}
void switch_threshold_max( int position ){
    threshold_maxInt = position;
}
void switch_min_dist( int position ){
    min_distInt = position;
}
void switch_dp( int position ){
    dpInt = position;
}
void switch_callback_h( int position ){
    highInt = position;
}
void switch_callback_l( int position ){
    lowInt = position;
}

// initialize the main function
int main(int argc, char *argv[])
{
    IplImage* picture = cvLoadImage("circles.jpg",CV_LOAD_IMAGE_COLOR);
    IplImage* greyImg = cvCreateImage(cvGetSize(picture), IPL_DEPTH_8U, 1);
    IplImage* cannyImg = cvCreateImage(cvGetSize(picture), IPL_DEPTH_8U, 1);
    IplImage* drawnImg = cvCreateImage(cvGetSize(picture), IPL_DEPTH_8U, 3);
    IplImage* contrastImg = cvCreateImage(cvGetSize(picture), IPL_DEPTH_8U, 1);

    cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Canny", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Threshold", CV_WINDOW_NORMAL);

    cvCvtColor(picture, greyImg, CV_BGR2GRAY);
    cvEqualizeHist(greyImg, greyImg);

    CvMemStorage* storage = cvCreateMemStorage(0);

    while (1) {

        // Create trackbars
        cvCopy(picture, drawnImg,NULL); // picture to be displayed

        cvCreateTrackbar( "min_dist", "Image", &min_dist_switch_value, 49, switch_min_dist );
        cvCreateTrackbar( "dp", "Image", &dp_switch_value, 9, switch_dp );
        cvCreateTrackbar( "High", "Canny", &high_switch_value, 499, switch_callback_h );
        cvCreateTrackbar( "Low", "Canny", &low_switch_value, 499, switch_callback_l );
        cvCreateTrackbar( "Threshold", "Threshold", &threshold_switch_value, 199, switch_threshold );
        cvCreateTrackbar( "Max", "Threshold", &threshold_max_switch_value, 500, switch_threshold_max );

        int N = 7;

        double dp = dpInt+1;
        double min_dist = min_distInt+1;
        double lowThresh = lowInt + 1;
        double highTresh = highInt + 1;
        double threshold = thresholdInt+1;
        double threshold_max = threshold_maxInt+1;


        cvThreshold(greyImg, contrastImg, threshold, threshold_max, CV_THRESH_TOZERO_INV);
        cvCanny(contrastImg, cannyImg, lowThresh*N*N, highTresh*N*N, N);

        //        CvSeq* circles =cvHoughCircles(greyImg, storage, CV_HOUGH_GRADIENT, 35, 25);
        CvSeq* circles =cvHoughCircles(cannyImg, storage, CV_HOUGH_GRADIENT, dp, min_dist,100,300,0,1000);
        // dp is image resolution
        // min_dist is the minimum distance between circles
        int i;
        for ( i = 0; i < (circles ? circles->total : 0); i++)
        {
            float* p = (float*)cvGetSeqElem( circles, i );
            cvCircle( drawnImg, cvPoint(cvRound(p[0]),cvRound(p[1])),10, CV_RGB(0,255,0), -1, 8, 0 );
        }

        cvShowImage("Image", drawnImg);
        cvShowImage("Canny", cannyImg);
        cvShowImage("Threshold", contrastImg);

        char b;

        while (b != 27) {
            b = cvWaitKey(1);
        }
        b=0;
    }
}
