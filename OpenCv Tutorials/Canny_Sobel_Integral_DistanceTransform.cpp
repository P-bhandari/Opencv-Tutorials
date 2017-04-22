#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void get_integral();
void get_distanceTransform();
Mat sobelOperator();
Mat CannyEdges();

int main()
{
	//get_integral();
	get_distanceTransform();
	//CannyEdges();
}

Mat sobelOperator(Mat inputImage)
{
	//Mat inputImage = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	Mat outputImage(inputImage.rows,inputImage.cols,inputImage.depth(),Scalar(0,0,0));
	int kernelSize = 3;
	int dx = 1;
	int dy =  1;
	int depth = -1; //just to keep it simple
	double scale = 1;//scaling of output image
	double beta = -2; //adding the value to the pixel values in the output image
	int borderType = BORDER_DEFAULT;
	Sobel(inputImage,outputImage,depth ,dx ,dy,kernelSize,scale,beta,borderType);
//	GaussianBlur(outputImage,outputImage,Size(3,3),0,0);
	imshow("inputImage",inputImage);
	imshow("outputImage",outputImage);
	waitKey(0);
	destroyAllWindows();
	return outputImage;
}

Mat CannyEdges()
{
	Mat inputImage = imread("car.jpg",CV_LOAD_IMAGE_GRAYSCALE); //has to be 8 bit single channel
	//inputImage.convertTo(inputImage,IPL_DEPTH_8U,1);
	Mat outputImage(inputImage.rows,inputImage.cols,inputImage.depth(),Scalar(0));
	//cout << "image rows = " << inputImage.rows << "image cols = "<< inputImage.cols << endl ;

	namedWindow("outputImage",CV_WINDOW_AUTOSIZE);
	int thresh1 = 20 ;
	int thresh2 = 200 ;
	int apertureSize = 3;
	createTrackbar("lowThreshold","outputImage",&thresh1,255);
	createTrackbar("HighThreshold","outputImage",&thresh2,255);

	while(1)
	{
		Canny(inputImage,outputImage,(double)thresh1,(double)thresh2,apertureSize,false); //true for accurate formula to be used and false for the equivalent easy formula
		cout << "image rows = " << outputImage.rows << "image cols = "<< outputImage.cols << endl ;
		imshow("outputImage",outputImage);
		cout << "size=" << outputImage.size <<endl ;
		int c = waitKey(2);
		if(c==27)
			break;
	}
	return outputImage ;
}

void get_distanceTransform()
{
	Mat image  = imread("car.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	Mat dist_transform(image.rows,image.cols,CV_32FC1,Scalar(0));
	image= CannyEdges();
	//normal transform
	distanceTransform(image,dist_transform,CV_DIST_L2,CV_DIST_MASK_PRECISE ); ///4th argument could be 3 or 5
	//dist_transform.convertTo(dist_transform,IPL_DEPTH_8U,1);
	imshow("original image",image);
	imshow("distance transform",dist_transform);
	waitKey(0);

	//using the input image as a canny edge detector


	destroyAllWindows();
}

void get_integral()
{
	Mat image  = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	Mat sum(image.rows+1,image.cols+1,CV_32FC3,Scalar(0,0,0));
	integral(image,sum,-1);

	sum.convertTo(sum,IPL_DEPTH_8U,1.0/(255.0*255.0));	//SCALING THE IMAGE TO THE RIGHT DEPTH
	imshow("original image",image);
	imshow("integral image",sum);
	waitKey(0);
	destroyAllWindows();
}
