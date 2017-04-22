#include <highgui.h>
#include <cv.h>
#include <stdio.h>

CvMat* get_warp_matrix(IplImage* src,IplImage* dst);
CvMat* get_rotation_matrix(IplImage* src,IplImage* dst);
CvMat* get_perspective_matrix(IplImage* src,IplImage* dst);

void affine(IplImage* src, IplImage* dst);					//warp affine transform :: uses 2x3 warp matrix
void WarpPerspective(IplImage* src, IplImage* dst);			//warp perspective transform :: uses a 3x3 matrix



//Affine transformations
int main()
{
	IplImage* input = cvLoadImage("bird.jpg",1);
	IplImage* output =cvCloneImage(input);
	cvNamedWindow("input image",0);
	cvShowImage("input image",input);
	WarpPerspective(input , output);
	cvDestroyAllWindows();
	cvReleaseImage(&input);
	cvReleaseImage(&output);
}

void WarpPerspective(IplImage* src,IplImage* dst)		//it is a type of dense perspective transform
{
	dst->origin= src->origin;
	cvNamedWindow("WarpPerspectiveOutput",0);
	int	 flags = CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS;
	CvMat* perspective_mat = get_perspective_matrix(src,dst);
	cvWarpPerspective(src,dst,perspective_mat,flags,cvScalarAll(0));

	cvShowImage("WarpPerspectiveOutput",dst);
	cvWaitKey(0);

}
CvMat*  get_perspective_matrix(IplImage* src,IplImage* dst)
{
	CvMat* perspective_mat = cvCreateMat(3,3,CV_32FC1);//the 3x3 matrix
	CvPoint2D32f pts_src[4] , pts_dst[4] ;

	//compute perspective matrix

	//define the source points
		pts_src[0].x = 0 ;
		pts_src[0].y = 0 ;
		pts_src[1].x = src->width-1;
		pts_src[1].y = 0 ;
		pts_src[2].x = 0;
		pts_src[2].y = src->height-1;
		pts_src[3].x = src->width-1;
		pts_src[3].y = src->height-1;

	//define destination points

		pts_dst[0].x = src->width*0.05;
		pts_dst[0].y = src->height*0.33;
		pts_dst[1].x = src->width*0.9;
		pts_dst[1].y = src->height*0.25;
		pts_dst[2].x = src->width*0.2;
		pts_dst[2].y = src->height*0.7;
		pts_dst[3].x = src->width*1;
		pts_dst[3].y = src->height*1;

	//get the matrix for the computation
		cvGetPerspectiveTransform(pts_src,pts_dst,perspective_mat);
		return perspective_mat ;
}
void affine(IplImage* src, IplImage* dst)
{

	dst->origin  = src->origin;
	int	 flags = CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS;


	CvMat* warp_mat = get_warp_matrix(src,dst);
	//compute the affine transform


	cvWarpAffine(src,dst,warp_mat,flags,cvScalarAll(0));
	cvCopy(dst,src,NULL);
	CvMat* rot_mat = get_rotation_matrix(src,dst);  //rotation matrix
	cvWarpAffine(src,dst,rot_mat,flags,cvScalarAll(0));
	cvNamedWindow("WarpAffineOutput",0);
	cvShowImage("WarpAffineOutput",dst);
	cvWaitKey(0);


}

CvMat* get_rotation_matrix(IplImage* src,IplImage* dst)
{
	CvPoint2D32f center =cvPoint2D32f(src->width/2,src->height/2);
	CvMat* rot_mat =cvCreateMat(2,3,CV_32FC1);
	double angle =-90.0;
	double scale = 0.6;
	cv2DRotationMatrix(center,angle,scale,rot_mat);
	return rot_mat;
}


CvMat* get_warp_matrix(IplImage* src,IplImage* dst)
{
	CvPoint2D32f pts_src[3] ,pts_dst[3];
	CvMat* warp_mat = cvCreateMat(2,3,CV_32FC1);

	//compute warp mat

	//define the source points
	pts_src[0].x = 0 ;
	pts_src[0].y = 0 ;
	pts_src[1].x = src->width-1;
	pts_src[1].y = 0 ;
	pts_src[2].x = 0;
	pts_src[2].y = src->height-1;

	//the side are parrallel in warp transform (y) ,that is why we define only 3 points
	//define the dest points
	pts_dst[0].x = src->width*0.00;
	pts_dst[0].y = src->height*0.0;
	pts_dst[1].x = src->width*0.85;
	pts_dst[1].y = src->height*0.35;
	pts_dst[2].x = src->width*0.65;
	pts_dst[2].y = src->height*0.9;

	cvGetAffineTransform( pts_src, pts_dst, warp_mat ); //warp matrix is 2x3
	cvNamedWindow("WarpAffineOutput",0);

	return warp_mat ;
}
