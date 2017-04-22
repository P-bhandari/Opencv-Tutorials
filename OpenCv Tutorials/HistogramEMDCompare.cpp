/*#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*. Th e other way of using cvCopyHist() is to set *dst to NULL. In this case, a new
histogram will be allocated that has the same size as src and then the data and bin
ranges will be copied (this is analogous to the image function cvCloneImage() ).

void cvCopyHist(const CvHistogram* src, CvHistogram** dst ); */


/*accumulate – Accumulation flag. If it is set, the histogram is not cleared in the beginning when it

 is allocated. This feature enables you to compute a single histogram from several sets of arrays,
  or to update the histogram in time.*/

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main(  )
{
    Mat src, hsv;
    src=imread("car.jpg",CV_LOAD_IMAGE_COLOR);
    //    return -1;

    cvtColor(src, hsv, CV_BGR2HSV);

    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    calcHist( &hsv, 1, channels, Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    double maxVal=0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                        Point( (h+1)*scale - 1, (s+1)*scale - 1),
                        Scalar::all(intensity),
                        CV_FILLED );
        }

    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );
    waitKey(0);
    destroyAllWindows();
}


//another type
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main()
{
	Mat hsv,src = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	cvtColor(src,hsv,CV_BGR2HSV);
	imshow("hsv image",hsv);
	MatND hist; 	//for n dimensional array
	int channels[] ={0,1} ; // corresponding to h and s value of the hsv image

	//number of bins for each dim
	int h_dims = 30 , s_dims = 32; //32 is the max value for present version of opencv
	int histSize[]={h_dims,s_dims} ;
	float h_ranges[]={0,179} ; //range of h dim
	float s_ranges[]={0,255} ; //range of s dim

	const float* ranges[]={h_ranges,s_ranges} ; 	//has to be constant otherwise error would come**
//	calcHist(&hsv,1,channels,Mat()/*nomask is required*/,hist,2,histSize,ranges,true/*continous*/,false);
	calcHist(&hsv,1,channels,Mat(),hist,2,histSize,ranges,true,false);

	double max_value,intensity;
	int scale =10;
	minMaxLoc(hist,0,&max_value,0,0); //to find the minimum and maximum value and find their location too
	Mat histImg(scale*s_dims,scale*h_dims,CV_8UC1,Scalar(0));
	cout<<"Hist rows= "<< hist.rows<<"Hist cols = "<<hist.cols<<endl;

	for(int h = 0 ; h <h_dims;h++)
	{
		for(int s = 0 ; s<s_dims;s++)
		{
			float present_value = hist.at<float>(h,s);
			intensity = (present_value*255)/max_value ;
			Point p1(h*scale,s*scale);
			Point p2((h+1)*scale-1,(s+1)*scale-1);	//-1 so that there is no overlapping among the pixels
			rectangle(histImg,p1,p2,Scalar(intensity),CV_FILLED);
		}
	}
	Mat equalizedHist;
	equalizeHist(histImg,equalizedHist);

	//correlation between histograms
/*	histImg.convertTo(histImg,CV_32F,1,0);
	equalizedHist.convertTo(equalizedHist,CV_32F,1,0);
//	equalizedHist = histImg ;
	double compareValue= compareHist(histImg,equalizedHist,CV_COMP_CORREL);
	cout <<"The measure of similarity between the histograms is = "<<compareValue <<endl;*/


	//calculating EMD
	histImg.convertTo(histImg,CV_32F,1,0);
	equalizedHist.convertTo(equalizedHist,CV_32F,1,0);
	//histImg = equalizedHist;
	float emd = EMD(histImg,equalizedHist,CV_DIST_L2);
	cout <<"The earth movers distance = "<<emd <<endl;

	imshow("hist Image",histImg);
	imshow("EqualizedHist Image",equalizedHist);
	waitKey(0);
	destroyAllWindows();
}
