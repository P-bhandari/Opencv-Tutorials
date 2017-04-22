#include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>

 using namespace cv;
 using namespace std;

 Mat src; Mat src_gray;
 int thresh = 100;
 int max_thresh = 255;
 RNG rng(12345);

 /// Function header
 void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
 {
	VideoCapture cap(0); //capture the video from web cam

   if ( !cap.isOpened() )  // if not success, exit program
   {
        cout << "Cannot open the web cam" << endl;
        return -1;
   }
   namedWindow("Source",1);
   createTrackbar( " Threshold:", "Source", &thresh, max_thresh );

while(1)
{
	bool bSuccess = cap.read(src); // read a new frame from video

	if (!bSuccess) //if not success, break loop
	{
	        cout << "Cannot read a frame from video stream" << endl;
	        break;
    }
   /// Convert image to gray and blur it
   cvtColor( src, src_gray, CV_BGR2GRAY );
   blur( src_gray, src_gray, Size(3,3) );
   thresh_callback( 0, 0 );
   /// Create Window
   char* source_window = "Source";
   imshow( source_window, src );

   char key =waitKey(2);
   if(key==27)
	   break;

}

   return(0);
 }

 /** @function thresh_callback */
 void thresh_callback(int, void* )
 {
   Mat src_copy = src.clone();
   Mat threshold_output;
   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;

   /// Detect edges using Threshold
   threshold( src_gray, threshold_output, thresh, 255, CV_THRESH_OTSU );

   /// Find contours
   findContours( threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

   /// Find the convex hull object for each contour
   vector<vector<Point> >hull( contours.size() );
   for( int i = 0; i < contours.size(); i++ )
      {  convexHull( Mat(contours[i]), hull[i], false ); }

   /// Draw contours + hull results
   Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
   for( int i = 0; i< contours.size(); i++ )
      {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      }

   /// Show in a window
   namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
   imshow( "Hull demo", drawing );
 }
