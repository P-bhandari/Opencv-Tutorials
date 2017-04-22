#include <highgui.h>
#include <cv.h>
#include <stdio.h>
//doesnot work
IplImage* update_Xmap(CvMat* input,CvMat* mapx);
IplImage* update_Ymap(CvMat* input,CvMat* mapy);
int main()
{
	IplImage* input =cvLoadImage("bird.jpg",CV_LOAD_IMAGE_COLOR);
	IplImage* output= cvCreateImage(cvGetSize(input),input->depth,3);
	IplImage* mapx = cvCreateImage(cvGetSize(input),IPL_DEPTH_32F,1);
	IplImage* mapy = cvCreateImage(cvGetSize(input),IPL_DEPTH_32F,1);
	int flags =  CV_INTER_LINEAR ;
	CvScalar fillval = cvScalar(0,0,0,0); //the pixel with no destination pixels in input image are given this value
	Remap(input,output,mapx,mapy,flags,fillval);

}
void Remap(IplImage* input,IplImage* output,IplImage* mapx,IplImage* mapy,int flags,CvScalar fillval)
{
	mapx = update_Xmap(input ,mapx);
	mapy = update_Ymap(input ,mapy);
	cvNamedWindow("input image",0);
	cvNamedWindow("output image",0);
	cvShowImage("input image",input);
	cvRemap(input,output,mapx,mapy,flags,fillval);
	cvShowImage("output image",output);
	cvWaitKey(0);
	cvReleaseImage(&input);
	cvReleaseImage(&output);
}
IplImage* update_Xmap(CvMat* input,CvMat* mapx)
{
	int i ,j;
	for(i=0 ; i < input->rows; i++)
	{
		for(j=0 ; j<input->cols;j++)
			cvSet2D(mapx,i,j,cvGet2D(input,i,j)) ;
	}
	return mapx;
}

IplImage* update_Ymap(CvMat* input,CvMat* mapy)
{
	int i ,j;
		for(i=0 ; i < input->rows; i++)
		{
			for(j=0 ; j<input->cols;j++)
				cvSet2D(mapy,i,j,cvGet2D(input,i,j)) ;
		}
		return mapy;
}
