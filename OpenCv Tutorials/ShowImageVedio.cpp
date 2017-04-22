#include <opencv2/highgui/highgui.hpp>
#include <iostream>
int get_Vedio();
int get_VedioFromCamera();
using namespace cv;
using namespace std;

int main( int argc, const char** argv )
{
     Mat img = imread("cat.jpg", CV_LOAD_IMAGE_UNCHANGED); //read the image data in the file "MyPic.JPG" and store it in 'img'

     if (img.empty()) //check whether the image is loaded or not
     {
          cout << "Error : Image cannot be loaded..!!" << endl;
          //system("pause"); //wait for a key press
          return -1;
     }

     namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window

     waitKey(0); //wait infinite time for a keypress

     destroyWindow("MyWindow"); //estroy the window with the name, "MyWindow"

     get_Vedio();
     get_VedioFromCamera();

     return 0;
}

int get_VedioFromCamera()
{
	VideoCapture cap(0);
	Mat frame;
	float width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	float height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	if(!cap.isOpened())
	{
		cout<<"Error: the file couldnot be opened "<<endl;
		return -1;
	}
//	double fps = cap.get(CV_CAP_PROP_FPS);
	//cout << "FPS of the vedio is:"<<fps<<endl ;
	cap.read(frame);
	while(!frame.empty())
	{
		imshow("Vedio",frame);
		cap.read(frame);
		if(waitKey(20)==27)
			break;
	}
}
int get_Vedio()
{
	VideoCapture cap("hi.avi");
	if(!cap.isOpened())
	{
		cout<< "The vedio file cannot be loaded" <<endl;
		return -1;
	}

	cap.set(CV_CAP_PROP_POS_MSEC,1000); //start from 1sec
	Mat frame ;
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout << "FPS of the vedio is:"<<fps<<endl ;

	cap.read(frame);
	while(!frame.empty())
	{
		imshow("vedio",frame);
		cap.read(frame);
		if(waitKey(30)==27 )
		{
			break;
		}
	}

	destroyAllWindows();


}

