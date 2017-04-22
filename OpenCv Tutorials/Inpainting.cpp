#include <cmath>
#include <cstdio>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;


///////EXAMPLE1



///****function signature
//
//void inpaint(
//    InputArray  src,     // The source image
//    InputArray  mask,    // Inpainting mask, non-zero pixels indicate _
//                         // the area to be inpainted
//    OutputArray dst,     // Output image
//    double      radius,  // Radius of circular neighborhood
//    int         flags    // Inpainting method
//)

int main()
{
	cv::Mat src = cv::imread("inpaintimg.jpg");	//the damaged image is used in inpaint function
	if (src.empty())
	    return -1;

	cv::Mat mask;		//this mask would hold the point around which inpainting algorithm is to be applied,
						//Non-zero pixels indicate the area that needs to be inpainted.

	cv::cvtColor(src, mask, CV_BGR2GRAY);
	cv::threshold(mask, mask, 220, 255, CV_THRESH_BINARY);

	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(mask.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
	    cv::Rect r = cv::boundingRect(contours[i]);


	    if(else )
	    if (std::abs(1 - (src.cols/r.width)) > 0.2)
	        cv::drawContours(mask, contours, i, CV_RGB(0,0,0), CV_FILLED);
	}

	cv::Mat dst;
	cv::inpaint(src, mask, dst, 1, cv::INPAINT_TELEA);
	imshow("Inpainted image",dst);
	imshow("Damaged image",src);
	waitKey(0);
	destroyAllWindows();
}



///////////////////////
//****EXAMPLE GIT HUB***///
static void help()
{
    cout << "\nCool inpainging demo. Inpainting repairs damage to images by floodfilling the damage \n"
            << "with surrounding image areas.\n"
            "Using OpenCV version %s\n" << CV_VERSION << "\n"
    "Usage:\n"
        "./inpaint [image_name -- Default ../data/fruits.jpg]\n" << endl;

    cout << "Hot keys: \n"
        "\tESC - quit the program\n"
        "\tr - restore the original image\n"
        "\ti or SPACE - run inpainting algorithm\n"
        "\t\t(before running it, paint something on the image)\n" << endl;
}

Mat img, inpaintMask;
Point prevPt(-1,-1);

static void onMouse( int event, int x, int y, int flags, void* )
{
    if( event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON) )
        prevPt = Point(-1,-1);
    else if( event == EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) )
    {
        Point pt(x,y);
        if( prevPt.x < 0 )
           prevPt = pt;
        line( inpaintMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        line( img, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", img);
    }
}


int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"car.jpg";
    Mat img0 = imread(filename, -1);
    if(img0.empty())
    {
        cout << "Couldn't open the image " << filename << ". Usage: inpaint <image_name>\n" << endl;
        return 0;
    }

    help();

    namedWindow( "image", 1 );

    img = img0.clone();
    inpaintMask = Mat::zeros(img.size(), CV_8U);

    imshow("image", img);
    setMouseCallback( "image", onMouse, 0 );

    for(;;)
    {
        char c = (char)waitKey();

        if( c == 27 )
            break;

        if( c == 'r' )
        {
            inpaintMask = Scalar::all(0);
            img0.copyTo(img);
            imshow("image", img);
        }

        if( c == 'i' || c == ' ' )
        {
            Mat inpainted;
            inpaint(img, inpaintMask, inpainted, 3, INPAINT_TELEA);
            imshow("inpainted image", inpainted);
        }
    }

    return 0;
}
