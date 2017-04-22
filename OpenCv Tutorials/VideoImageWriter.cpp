#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void save_image(Mat frame);
void save_video();
int main()
{
	VideoCapture capture(1);
	Mat frame ;
	while(1)
	{
		capture.read(frame);
		imshow("Frame",frame);
		char c = waitKey(2);
		if(c ==32)
		save_image(frame);
	}
		//save_video();
	return 0 ;
}
void save_video()
{
	Mat frame ;
	VideoCapture cap(0);
	if(!cap.isOpened())
	{
		cout<< "The capture could not be started" <<endl;
	}
	double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);

	Size frameSize(width,height);
//	double fps = cap.get(CV_CAP_PROP_FPS);
	int type = CV_FOURCC('M','P','E','G');
	VideoWriter Vwriter("writtenVedio.avi",type,20,frameSize,true);

//	cout << "The FPS  = "<< fps <<endl ;
	if(!Vwriter.isOpened())
	{
		cout << "The writer could not be started" <<endl;
	}
	cap.read(frame);
	while(!frame.empty())
	{
		Vwriter.write(frame);
		imshow("WrittenVedio",frame);
		cap.read(frame);

		if(waitKey(2)==27)
			break;
	}
}

void save_image(Mat image)
{
	//Mat image(200,300,CV_32FC3 ,Scalar(0,230,200));
	vector<int> compression_param;
	compression_param.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_param.push_back(99);
	bool success = imwrite("PIYUSH.jpg",image,compression_param);
	if(!success)
	{
		cout<< "Failed Loading File" <<endl;
	}
	else
	{
		Mat writtenImage = imread("PIYUSH.jpg",CV_LOAD_IMAGE_COLOR);
		imshow("New Image",writtenImage);
		waitKey(0);
		destroyAllWindows();
	}
}
