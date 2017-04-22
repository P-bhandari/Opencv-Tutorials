
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;


//MOMENTS

/*the moments resulting
from that computation are not the best parameters for such comparisons in most practi-
cal cases. In particular, one would often like to use normalized moments (so that objects
of the same shape but dissimilar sizes give similar values).*/

/*  // spatial moments
    double  m00, m10, m01, m20, m11, m02, m30, m21, m12, m03;
    // central moments
    double  mu20, mu11, mu02, mu30, mu21, mu12, mu03;
    // central normalized moments
    double  nu20, nu11, nu02, nu30, nu21, nu12, nu03;*/


int main()
{
	Mat image = imread("thresholded.png",CV_LOAD_IMAGE_GRAYSCALE);
	Mat input(image.rows,image.cols,IPL_DEPTH_8U,Scalar::all(0));
	Size size(640,580);
	resize(image,input,size);

	Moments huemo, mo=moments(input,true);
	double hu[7];

	HuMoments(mo,hu);
	cout<<"hu[0] "<<hu[0]<<endl;


	cout<<"m00  "<<mo.m00<<"\nm01  "<<mo.m01 <<endl;
	imshow("input",input);
	waitKey(0);

}
