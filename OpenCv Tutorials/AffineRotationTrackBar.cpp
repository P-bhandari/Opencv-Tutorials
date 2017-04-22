#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int iAngle = 180;
int iScale = 50;
int iBorderMode = 0;
Mat imgOriginal ;
int iImageCenterY = 0;
int iImageCenterX = 0;
const char* pzRotatedImage = "Rotated Image";

void CallbackForTrackBar(int, void*)
{
 Mat matRotation = getRotationMatrix2D(  Point( iImageCenterX, iImageCenterY ), (iAngle - 180), iScale / 50.0 );

 // Rotate the image
 Mat imgRotated;
 warpAffine( imgOriginal, imgRotated, matRotation, imgOriginal.size(), INTER_LINEAR, iBorderMode, Scalar() );

  imshow( pzRotatedImage, imgRotated );

}

void betterRotationOfImage();
void rotateImage();
int main()
{

//	rotateImage();
	betterRotationOfImage();

}

void betterRotationOfImage()
{
	 // Load the image
	 imgOriginal = imread("bird.jpg",1);

	  iImageCenterY = imgOriginal.rows / 2;
	 iImageCenterX = imgOriginal.cols / 2;

	  //show the original image
	 const char* pzOriginalImage = "Original Image";
	 namedWindow( pzOriginalImage, CV_WINDOW_AUTOSIZE );
	 imshow( pzOriginalImage, imgOriginal );

	  //create the "Rotated Image" window and 3 trackbars in it
	 namedWindow( pzRotatedImage, CV_WINDOW_AUTOSIZE );
	 createTrackbar("Angle", pzRotatedImage, &iAngle, 360, CallbackForTrackBar);
	 createTrackbar("Scale", pzRotatedImage, &iScale, 100, CallbackForTrackBar);
	 createTrackbar("Border Mode", pzRotatedImage, &iBorderMode, 5, CallbackForTrackBar);

	 int iDummy = 0;

	  CallbackForTrackBar(iDummy, &iDummy);

	  waitKey(0);


}
void rotateImage()
{
	Mat originalImage = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
		Point centerOfRotation(originalImage.cols/2,originalImage.rows/2);
		Mat rotationMatrix ;

		int iAngle = 180;
		namedWindow("rotation",CV_WINDOW_AUTOSIZE);
		createTrackbar("Angle","rotation",&iAngle,360);
		while(1)
		{
			rotationMatrix = getRotationMatrix2D(centerOfRotation,iAngle-180,1);
			Mat imgRotated ;

			warpAffine(originalImage,imgRotated,rotationMatrix,originalImage.size());

			imshow("rotation",imgRotated);
			int c = cvWaitKey(2);
			if(c==27)
				break;
		}
}
