#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("bird.jpg",CV_LOAD_IMAGE_COLOR);
	Vec3f intensity;

	 intensity.val[0]=2;
	 intensity.val[1]=2;
	intensity.val[2]=2;
	 img.at<Vec3f>(Point(100,3))=intensity;
	cout << "data = "<<img.at<Vec3f>(Point(100,3)) << endl ;

}


///////?#### important info down here
int main()
{

	 string a = "hist";
	 string b = ".jpg";
	 int c = 1;

	 Mat img = imread("/home/newton/Documents/NTU Internship/stereo matching/SparseSetero Code/Baselines/training/disp_occ/000002_10.png",-1);
	 imshow("a",img);cout<<"Start"<<endl;
	 uchar* p = img.data;
	 for(int i = 0 ; i<img.rows;i++)
		 for(int j = 0;  j<img.cols;j++)
			 cout<<"Point = "<<j<<","<<i<<" = "<<img.at<short>(Point(i,j))/256<<endl;

	//cout<< img.at<short>(373,779)/256<<endl;
	 waitKey();

}
