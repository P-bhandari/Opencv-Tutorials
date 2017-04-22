#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

     }
}

int main(int argc, char** argv)
{
     // Read image from file
     Mat img = imread("car.jpg");

      //if fail to read the image
     if ( img.empty() )
     {
          cout << "Error loading the image" << endl;
          return -1;
     }

      //Create a window
     namedWindow("My Window", 1);

      //set the callback function for any mouse event
     setMouseCallback("My Window", CallBackFunc, NULL);

      //show the image
     imshow("My Window", img);

      // Wait until user press some key
     waitKey(0);

      return 0;

}
