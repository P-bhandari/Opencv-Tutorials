#include <highgui.h>
#include <cv.h>
#include <stdio.h>
IplImage* PyramidDown(IplImage* image);
IplImage* PyramidUp(IplImage* image);
int main()
{
   ImagePyramid();

}

void ImagePyramid()
{
	IplImage* input_image= cvLoadImage("girl.jpg",CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("original Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("PYRUP", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("PYRDOWN", CV_WINDOW_AUTOSIZE);
	cvShowImage("original Image",input_image);
	IplImage* p_pyrDownImage = PyramidDown(input_image);
	cvShowImage("PYRDOWN",p_pyrDownImage);
	IplImage* p_pyrUpImage = PyramidUp(input_image);
	cvShowImage("PYRUP",p_pyrUpImage);
	cvWaitKey(0);
	p_pyrUpImage = PyramidUp(p_pyrDownImage);
	cvShowImage("PYRUP",p_pyrUpImage); //try to remove it and see if the change persists
	cvWaitKey(0);						//no it doesnt work that way
	p_pyrUpImage = PyramidUp(p_pyrUpImage);
	cvShowImage("PYRUP",p_pyrUpImage); //try to remove it and see if the change persists
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&input_image);
	cvReleaseImage(&p_pyrUpImage);
	cvReleaseImage(&p_pyrDownImage);

}
IplImage* PyramidDown(IplImage* image)
{
	IplImage* output=cvCreateImage(cvSize(image->width/2,image->height/2),8,3) ;
	cvPyrDown(image, output,CV_GAUSSIAN_5x5 );
//	cvShowImage("PYRDOWN",output); //a window once created can be used by anyfunction

	return output;
}
IplImage* PyramidUp(IplImage* image)
{
	IplImage* output=cvCreateImage(cvSize(image->width*2,image->height*2),8,3) ;
	cvPyrUp(image, output,CV_GAUSSIAN_5x5 );
	return output;
}


/*To produce layer (i+1) in the Gaussian pyramid, we do the following:

Convolve G_{i} with a Gaussian kernel: (smoothing it)

\frac{1}{16} \begin{bmatrix} 1 & 4 & 6 & 4 & 1  \\ 4 & 16 & 24 & 16 & 4  \\ 6 & 24 & 36 & 24 & 6  \\ 4 & 16 & 24 & 16 & 4  \\ 1 & 4 & 6 & 4 & 1 \end{bmatrix}

****That above was the gaussian kernel****
Remove every even-numbered row and column.

You can easily notice that the resulting image will be exactly one-quarter the area of its predecessor. Iterating this process on the input image G_{0} (original image) produces the entire pyramid.

The procedure above was useful to downsample an image. What if we want to make it bigger?:

First, upsize the image to twice the original in each dimension, wit the new even rows and columns filled with zeros (0)
Perform a convolution with the same kernel shown above (multiplied by 4) to approximate the values of the “missing pixels”*/
