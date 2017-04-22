#include <highgui.h>
#include <cv.h>
#include <stdio.h>

//doesnt work , it doesnt even run properly , no idea
IplImage* cvMyGaussianSmooth(IplImage* image);

int main()
{
	printf("hi");
	IplImage* image = cvLoadImage("girl.jpg",CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("gaussian Smooth",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("originalImage",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Mysmooth",CV_WINDOW_AUTOSIZE);
	IplImage* gaussian_Image = cvCloneImage(image);
	cvSmooth(image , gaussian_Image,CV_GAUSSIAN,3,3,0,0); //gaussian blur
	cvShowImage("gaussian Smooth", gaussian_Image);
	cvShowImage("originalImage",image);
	IplImage* myImage =cvCloneImage(image);
    myImage= cvMyGaussianSmooth(image);
	cvShowImage("Mysmooth",myImage);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&image);
	cvReleaseImage(&myImage);
	cvReleaseImage(&gaussian_Image);
}

IplImage* cvMyGaussianSmooth(IplImage* image)
{
	int h , w, x,y;
	int* img_data =(int)image->imageData ;
	int width_step = image->widthStep;
	int channels, nChannels = image->nChannels;
	int weighted_sum=0;
	int height = image->height;
	int width = image->width;
	//get x and y
	for(y=0 ; y <width ;y++)
	{
		for(x= 0 ; x<height;x++)
		{
			for(channels = 0;  channels <3;channels++)
			{
				for(h=-1; h<2;h++)
				{
					for(w=-1;w<2;w++)
					{
						//assume you are in x ,y position
						if(img_data[(x+h)*width_step + (y+w)*nChannels+channels]!=NULL)
						weighted_sum=img_data[(x+h)*width_step + (y+w)*nChannels+channels];
					}
				}
				img_data[x*width_step+y*nChannels+channels]=weighted_sum/9;
				weighted_sum=0;

			}
		}
	}

	return image ;
}
