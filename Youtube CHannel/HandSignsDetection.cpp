#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;
int iLowH = 0;
int iHighH = 179;
int iLowS = 37;
int iHighS = 134;
int iLowV = 0;
int iHighV = 255;

Point point(20,20); //to select where the rectangle would be made
int roi_width = 400  ;
int roi_height= 400 ;

int matchTheState(vector<Point> present_hand_state , vector<vector<Point > > MyContours);
void save_image(Mat image)
{

	vector<int> compression_param;
	compression_param.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_param.push_back(99);
	bool success = imwrite("ThumbsUp.jpg",image,compression_param);
	if(!success)
	{
		cout<< "Failed Loading File" <<endl;
	}
	else
	{
		Mat writtenImage = imread("ThumbsUp.jpg",CV_LOAD_IMAGE_COLOR);
		imshow("Written Image",writtenImage);

	}
}
vector<vector<Point> > getTestCases()
{

//	Mat test1 = imread("Fist.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	//Canny(test1,test1, 100, 200, 3);
//	vector<vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//
//	findContours( test1, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
//
//	Mat drawing = Mat::zeros( test1.size(), CV_8UC3 );
//	cout<<"Contour size ="<<contours.size()<<endl;
//	Scalar color(255,0,0);
//	 int largest_area=0;
//	 int largest_contour_index=0;
//	 for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
//	      {
//	       double a=contourArea( contours[i],false);  //  Find the area of contour
//	       if(a>largest_area){
//	       largest_area=a;
//	       largest_contour_index=i;                //Store the index of largest contour
//	     }
//
//	      }
//	  cv::approxPolyDP(Mat(contours[largest_contour_index]),contours[largest_contour_index],3,1);	//typecastsing is important
//
//	drawContours( drawing, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
//
//
//	imshow("Test contour",drawing);
//	return contours[largest_contour_index];

	vector<Mat> testCases ;

	vector<vector<Point > > MyContours ;
	Mat test = imread("TestFullPalm.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	testCases.push_back(test);//1
     test = imread("Test1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	testCases.push_back(test);//2
	 test = imread("Test2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	testCases.push_back(test);//3
	 test = imread("Test3.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	testCases.push_back(test);//4
	test = imread("Fist.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	testCases.push_back(test);//5
	test = imread("ThumbsUp.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	testCases.push_back(test);//6



	for(int i = 0 ; i<testCases.size();i++)
	{
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat temp ;
		testCases.at(i).copyTo(temp);
		findContours( temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		int largest_area=0;
		 int largest_contour_index=0;
		 for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
		 {
		       double a=contourArea( contours[i],false);  //  Find the area of contour
		       if(a>largest_area){
		       largest_area=a;
		       largest_contour_index=i;                //Store the index of largest contour
		  }

		  }
		 cv::approxPolyDP(Mat(contours[largest_contour_index]),contours[largest_contour_index],3,1);
		 MyContours.push_back(contours[largest_contour_index]);

	}

	return MyContours;
}

vector<Point> findtheContour(Mat image)
{
	Mat gray ;
	image.copyTo(gray);
	Canny(gray, gray, 100, 200, 3);
	imshow("Canny Output",gray);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
//	Rect imageroi = Rect(point.x,point.y,roi_width,roi_height);	//the last two parameters are the width and height of the rectangle
//	Mat temp = gray(imageroi);

	findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	/// Draw contours
	Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
//	for( int i = 0; i< contours.size(); i++ )
//	{
//	   Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//	   drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//	}

	//largest contour
	 int largest_area=0;
	 int largest_contour_index=0;
	 for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
	 {
	       double a=contourArea( contours[i],false);  //  Find the area of contour
	       if(a>largest_area)
	       {
	    	   largest_area=a;
	    	   largest_contour_index=i;                //Store the index of largest contour
	   //      bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
	       }
	 }

	Scalar color( 255,0,0);
	  cv::approxPolyDP(Mat(contours[largest_contour_index]),contours[largest_contour_index],3,1);	//typecastsing is important
	  drawContours( drawing, contours,largest_contour_index, color, 0 ); // Draw the largest contour using previously stored index.
//	vector<vector<Point> >hull;
//    convexHull( Mat(contours[largest_contour_index]), hull[0], false );
//    drawContours( drawing, hull, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );

	imshow( "Result window", drawing );
	 char c = waitKey(2);
		 if(c==32)
		 {
			save_image(drawing);
		 }
	if(!contours[largest_contour_index].empty())
		return contours[largest_contour_index];
}
vector<Point> detectHand(Mat imgOriginal)
{

		    namedWindow("Thresholded Image",1);


		      //Create trackbars in "Control" window
		     createTrackbar("LowH", "Thresholded Image", &iLowH, 179); //Hue (0 - 179)
		     createTrackbar("HighH", "Thresholded Image", &iHighH, 179);

		     createTrackbar("LowS","Thresholded Image", &iLowS, 255); //Saturation (0 - 255)
		     createTrackbar("HighS", "Thresholded Image", &iHighS, 255);

		     createTrackbar("LowV", "Thresholded Image", &iLowV, 255); //Value (0 - 255)
		     createTrackbar("HighV", "Thresholded Image", &iHighV, 255);





		    imshow("Original Image",imgOriginal);


		    Mat imgHSV;

		    	   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		    	  Mat imgThresholded;

		    	   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

//		    	   //morphological opening (remove small objects from the foreground)
		    	   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
		    	   dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
//
		    	    //morphological closing (fill small holes in the foreground)
		    	   dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		    	   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		   		rectangle(imgThresholded,Point(0,imgThresholded.rows -10),Point(imgThresholded.cols,imgThresholded.rows),Scalar(0,0,0),3,8,0);

		    	   imshow("Thresholded Image", imgThresholded); //show the thresholded image
		           vector<Point> present_hand_state= findtheContour(imgThresholded);
		           return present_hand_state;
//		    	   if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//		           {
//		                cout << "esc key is pressed by user" << endl;
//		                break;
//		           }



}


int main()
{

	VideoCapture cap(1); //capture the video from web cam

		    if ( !cap.isOpened() )  // if not success, exit program
		    {
		         cout << "Cannot open the web cam" << endl;

		    }
	vector<vector<Point> > MyContours = getTestCases();
	string array[] = {"FullPalm","One","Two","Three","Fist","ThumbsUp"};
	while(1)
	{
	    Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
	    if (!bSuccess) //if not success, break loop
		{
		       cout << "Cannot read a frame from video stream" << endl;
		       break;
		}
	    Mat send ;
	    imgOriginal.copyTo(send);
		vector<Point> present_hand_state = detectHand(send) ;

		 double area=contourArea( present_hand_state,false);
		 cout<<area<<endl;
		int a = matchTheState(present_hand_state,MyContours) ;
//		double match_value =  cv::matchShapes(test1,present_hand_state,CV_CONTOURS_MATCH_I2,0);
//		cout<<"match value"<<match_value<<endl;

		if(a!=100 & area > 500)
			//cv::putText(imgOriginal,array[a],Point(50,50),FONT_HERSHEY_DUPLEX,2,Scalar(255,0,0),2,1,false);
//		//rectangle(imgOriginal,point,Point(point.x+roi_width,point.y+roi_height),Scalar(0,255,0),3,8,0);

		imshow("ImgOriginal",imgOriginal);

		waitKey(23);
	}
}

int matchTheState(vector<Point> present_hand_state , vector<vector<Point > > MyContours)
{
	double array[MyContours.size()];
	int answer = 0;
	for(int i =0 ; i<MyContours.size();i++)
	{
		double match_value =  cv::matchShapes(MyContours[i],present_hand_state,CV_CONTOURS_MATCH_I2,0);
		array[i] = match_value ;
		if(array[answer]>array[i])
			answer =i;
	}
	if(array[answer] <0.9)
		return answer ;
	else
		return 100;
}
