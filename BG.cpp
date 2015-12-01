#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

Mat Frame, BG;
Mat Intial, HSV;
Mat Thresholded;

int main()
{
	VideoCapture cap(0);

	cap.read(Intial);
	while(cap.read(Intial))
	{
	    while(true)
	    {
	    	cap.read(Frame);
	    	BG = abs (Frame - Intial);
	    	int d = waitKey(10);
        	if( (char)d == 27 ) { break; }
        	cvtColor(BG, HSV, COLOR_BGR2HSV);
        	inRange(HSV, Scalar(0, 20, 20), Scalar(255, 255, 255), Thresholded);
        	medianBlur(Thresholded, Thresholded, 15);
        	medianBlur(Thresholded, Thresholded, 15);
        	medianBlur(Thresholded, Thresholded, 15);
        	imshow("BG", Thresholded);
	    }
	    int c = waitKey(10);
	    if( (char)c == 114 ) { continue; }
        else if( (char)c == 27 ) { break; }
    } 
    return 0;
}