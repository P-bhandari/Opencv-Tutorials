#include <cmath>
#include <cstdio>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include<opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;
int main()
{
Mat img1 = imread("Piyush1.jpg", 1);
Mat img2 = imread("Piyush2.jpg", 1);
cv::resize(img1,img1,Size(640,480));
cv::resize(img2,img2,Size(640,480));

if(img1.empty() || img2.empty())
{
    printf("Can't read one of the images\n");
    return -1;
}

// detecting keypoints
SurfFeatureDetector detector(400);
vector<KeyPoint> keypoints1, keypoints2;
detector.detect(img1, keypoints1);
detector.detect(img2, keypoints2);
Mat img_keypoints_1;
drawKeypoints( img1, keypoints1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
imshow("Image KeyPoints",img_keypoints_1);
// computing descriptors
SurfDescriptorExtractor extractor;
Mat descriptors1, descriptors2;
extractor.compute(img1, keypoints1, descriptors1);
extractor.compute(img2, keypoints2, descriptors2);

// matching descriptors
BruteForceMatcher<L2<float> > matcher;
vector<DMatch> matches;
matcher.match(descriptors1, descriptors2, matches);

// drawing the results
namedWindow("matches", 1);
Mat img_matches;
drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);
imshow("matches", img_matches);
waitKey(0);
}
