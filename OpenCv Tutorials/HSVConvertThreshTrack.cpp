
#include <cv.h>
#include <highgui.h>

using namespace cv;
//merging and spilting image channels
int main(  )
{
    Mat src, hsv,cool;
    if(!(src=imread("rgb.png", 1)).data )
        return -1;

    cvtColor(src, hsv, CV_BGR2HSV);

    vector<Mat> h ;

    imshow("hsv ",hsv);

    cv::split(hsv,h);
    namedWindow("h",WINDOW_AUTOSIZE);
    namedWindow("s",WINDOW_AUTOSIZE);
    namedWindow("v",WINDOW_AUTOSIZE);

    int h_thresh = 0 ;
	createTrackbar("hvalue","h",&h_thresh,255);
    int s_thresh = 0 ;
	createTrackbar("svalue","s",&s_thresh,255);
    int v_thresh = 0 ;
	createTrackbar("vvalue","v",&v_thresh,255);
	Mat H;
	H=h[0].clone();
	Mat S=h[1].clone();
	Mat V=h[2].clone();
	while(1)
	{
		threshold(H,h[0],h_thresh,255,THRESH_BINARY);
		threshold(S,h[1],s_thresh,255,THRESH_BINARY);
		threshold(V,h[2],v_thresh,255,THRESH_BINARY);
	    imshow("h",h[0]);
	    imshow("s",h[1]);
	    imshow("v",h[2]);
	    merge(h,cool);
	    imshow("cool",cool);
	    int c =waitKey(2);
	    if(c==27)
	    	break;
	}


