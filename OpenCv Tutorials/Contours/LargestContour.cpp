#include <iostream>
#include "opencv2\highgui\highgui.hpp"
#include "opencv\cv.h"

using namespace cv;
using namespace std;
int main()
{
 int largest_area=0;
 int largest_contour_index=0;
 Rect bounding_rect;

 Mat src = imread("src.jpg"); //Load source image
 Mat thr(src.rows,src.cols,CV_8UC1);
 Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
 cvtColor(src,thr,CV_BGR2GRAY); //Convert to gray
 threshold(thr, thr,25, 255,THRESH_BINARY); //Threshold the gray

    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;

    findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image

     for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
      {
       double a=contourArea( contours[i],false);  //  Find the area of contour
       if(a>largest_area){
       largest_area=a;
       largest_contour_index=i;                //Store the index of largest contour
       bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
       }

      }

 Scalar color( 255,255,255);
 drawContours( dst, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
 rectangle(src, bounding_rect,  Scalar(0,255,0),1, 8,0);
 imshow( "src", src );
 imshow( "largest Contour", dst );
 waitKey(0);
}
