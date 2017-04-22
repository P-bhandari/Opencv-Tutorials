#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/**
 * @function main

 */
int compareHistograms(Mat img1,Mat img2);

int main( )
{
	Mat img1,img2;
	compareHistograms(img1,img2);

}
int compareHistograms(Mat img1,Mat img2)
{
   // Mat img1,img2 ;
    Mat out1,out2;
    /// Load three images with different environment settings


    img1.copyTo(out1);
    img2.copyTo(out2);

 //   src_test2 = imread( argv[3], 1 );

    /// Convert to HSV
    cvtColor( out1, img1, COLOR_BGR2HSV );
    cvtColor( out2, img2, COLOR_BGR2HSV );
   // cvtColor( src_test2, hsv_test2, COLOR_BGR2HSV );

//    hsv_half_down = hsv_base( Range( hsv_base.rows/2, hsv_base.rows - 1 ), Range( 0, hsv_base.cols - 1 ) );

    /// Using 50 bins for hue and 60 for saturation
    int h_bins = 100; int s_bins = 100;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };


    /// Histograms
    MatND hist1;
    MatND hist2;
//    MatND hist_test1;
//    MatND hist_test2;

    /// Calculate the histograms for the HSV images
    calcHist( &img1, 1, channels, Mat(), hist1, 2, histSize, ranges, true, false );
    normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &img2, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false );
    normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat() );

//    calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
//    normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );
//
//    calcHist( &hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false );
//    normalize( hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Apply the histogram comparison methods
//    for( int i = 0; i < 4; i++ )
//    {
       // int compare_method = i;
        double p1 = compareHist( hist1, hist2 ,CV_COMP_CORREL );
//        double base_half = compareHist( hist_base, hist_half_down, compare_method );
//        double base_test1 = compareHist( hist_base, hist_test1, compare_method );
//        double base_test2 = compareHist( hist_base, hist_test2, compare_method );

//        printf( " Method [%d] Perfect, Base-Half, Base-Test(1), Base-Test(2) : %f, %f, %f, %f \n", i, base_base, base_half , base_test1, base_test2 );
//    }
return p1;
		//   printf( "Done \n" );

//    return 0;
}
