#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main()
{
	/* Different ways to acccess pixel in an image
	 * The first one is the direct addressing method :-
	 */


	Mat image = imread("car.jpg",CV_LOAD_IMAGE_COLOR);
	for(int i = 0 ; i <image.rows;i++)
		for(int j = 0; j<image.cols;j++)
			for(int k = 0 ; k<3;k++)
				cout<< "The data at i , j = "<<(int)*(image.data + 28*image.step[0]+28*image.step[1]+1)<<endl;	//Accesing the data of a coloured image

	/*		i ---> row
	 * 		j----> column
	 * 		k----> bgr number
	 */


	/*The second method is the plane method*/

	vector<Mat> planes;
	split(image,planes);

	for(int i = 0 ; i <image.rows;i++)
		for(int j = 0; j<image.cols;j++)
			for(int k = 0 ; k<3;k++)
				cout<<"The plane " <<k<<"value = "<<(int)planes[k].at<uchar>(i,j)<<endl;



	merge(planes,image);
	imshow("image",image);
	waitKey(0);
}
