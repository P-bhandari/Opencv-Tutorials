#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void contour(Mat image);
 int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Thresholded Image",CV_WINDOW_AUTOSIZE);
  int iLowH = 0;
 int iHighH = 179;

  int iLowS = 0;
 int iHighS = 255;

  int iLowV = 0;
 int iHighV = 255;

  //Create trackbars in "Control" window
 cvCreateTrackbar("LowH", "Thresholded Image", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Thresholded Image", &iHighH, 179);

  cvCreateTrackbar("LowS","Thresholded Image", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Thresholded Image", &iHighS, 255);

  cvCreateTrackbar("LowV", "Thresholded Image", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Thresholded Image", &iHighV, 255);

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

    Mat imgHSV;

   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

  Mat imgThresholded;

   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

  //morphological opening (remove small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //morphological closing (fill small holes in the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


  imshow("Thresholded Image", imgThresholded); //show the thresholded image
  imshow("Original", imgOriginal); //show the original image
  contour(imgThresholded);
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

   return 0;

}

 void contour(Mat gray)
 {
     namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
   //  imshow( "Display window", image );
  //   Mat gray;
   //  cvtColor(image, gray, CV_BGR2GRAY);
     Canny(gray, gray, 100, 200, 3);
     /// Find contours
     vector<vector<Point> > contours;
     vector<Vec4i> hierarchy;
     RNG rng(12345);
     findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


     ////////////////////
       vector<Moments> mu(contours.size() );
        for( int i = 0; i < contours.size(); i++ )
           { mu[i] = moments( contours[i], false ); }

        ///  Get the mass centers:
        vector<Point2f> mc( contours.size() );
        for( int i = 0; i < contours.size(); i++ )
           { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }



        ///////////////////
     /// Draw contours

     Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
     for( int i = 0; i< contours.size(); i++ )
     {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );

         ////////////////////
           vector<Moments> mu(contours.size() );
            for( int i = 0; i < contours.size(); i++ )
               { mu[i] = moments( contours[i], false ); }

            ///  Get the mass centers:
            vector<Point2f> mc( contours.size() );
            for( int i = 0; i < contours.size(); i++ )
               { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }



            ///////////////////
         cout<<"Center = "<<mc[i].x << ","<< mc[i].y<<endl;
     }

     imshow( "Result window", drawing );
  //   waitKey(0);

 }
