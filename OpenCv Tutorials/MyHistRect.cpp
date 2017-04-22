#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;
void makeRect(Mat &input);
void myhist(Mat &input,Mat &stored);	//only one dimensional iamges
void drawhist(Mat &input,Mat &stored);
int main()
{
	Mat input = imread("car.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	if(input.empty())
	{
		cout<<"The file could not be opened"<<endl;
		return 0 ;
	}
	imshow("Input Image",input);

	Mat stored(1,257,CV_32FC1); //There are 257 slots as last slot corresponds to the max of any dim in hist
	myhist(input,stored);
	drawhist(input,stored);

	waitKey(0);
	destroyAllWindows();
}

void drawhist(Mat &input,Mat &stored)
{
	Mat histfig(720,512,CV_32FC1,Scalar::all(255));//512 = 256*2 each line would be 2 pixels wide

	float* y = stored.ptr<float>(0);
	float max_value = y[256];
	for(int i =0; i<256;i++)
	{
		Point p1(2*i,0);
		Point p2(2*i,(y[i]/max_value)*720);
		line(histfig,p1,p2,Scalar(0,0,0),2,CV_AA);
		imshow("hist image",histfig);

	}
}
void makeRect(Mat &input)
{
	Point p1(20,20);
	Point p2(410,410);
	rectangle(input,p1,p2,Scalar(0,0,255),3,8,0);
	imshow("input",input);
	waitKey(0);
}
void myhist(Mat &input,Mat &stored)
{

	stored.setTo(0);
	float* value = stored.ptr<float>(0);

	float highest=0;
	int i,j;

	for( i=0 ; i<input.rows;i++)
	{
		uchar* i_row= input.ptr<uchar>(i);	//has to be uchar cause we have defined it this ways
		//cout<<"the value of "<< i << "row first element is "<<(int)i_row[0]<<endl ;
		for(j=0;j<input.cols;j++)
		{
			value[(int)i_row[j]]++ ;

			if(highest < value[(int)i_row[j]])
				highest= value[(int)i_row[j]];

		}
	}
	//namedWindow("output",1);
	value[256]=highest;

}

