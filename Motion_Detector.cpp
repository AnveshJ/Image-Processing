#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from webcam
  // cap.open("http://admin:@192.168.1.101:188/videostream.cgi?user=admin&pwd=&.mjpg");

  if ( !cap.isOpened() )  // if not success, exit program
  {
      cout << "Cannot open the web cam" << endl;
      return -1;
  }
  Vec3b pixel;
  int xav, yav, h, s, v, H, S, V;
  Mat img, img1, image, HSV, Thresholded, Final, HSV1;
  int iLastX = -1; 
  int iLastY = -1;
 
  while(true)
    {
        cap.read(img);
        Mat imgLines = Mat::zeros( img.size(), CV_8UC3 );;
        cap.read(img1);

        image = abs(img - img1) ;
        cvtColor(image, HSV, COLOR_BGR2HSV);
        inRange(HSV, Scalar(0, 20, 20), Scalar(255, 255, 255), Thresholded);
        medianBlur(Thresholded, Thresholded, 15);
        medianBlur(Thresholded, Thresholded, 15);
        medianBlur(Thresholded, Thresholded, 15);
        Moments oMoments = moments(Thresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;

 
        if (dArea > 10000)
        {
            //calculate the position of the ball
            int posX = dM10 / dArea;
            int posY = dM01 / dArea;   
        
            if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
            {
                line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
            }
            xav= (posX+iLastX)/2;
            yav= (posY+iLastY)/2;     
            std::cerr << "\n" << "X = " << xav << "\n" ;
            std::cerr << "Y = " << yav << "\n" ;
  
            pixel = img.at<Vec3b>(Point(xav, yav));
            h=(int)pixel[0] - 20;
            s=(int)pixel[1] - 50;
            v=(int)pixel[2] - 50;
            H=(int)pixel[0] + 20;
            S=(int)pixel[1] + 50;
            V=(int)pixel[2] + 50;
            if(h<0)h=0;
            if(s<0)s=0;
            if(v<0)v=0;
            if(H>179)h=179;
            if(S>255)S=255;
            if(V>255)V=255;
            cvtColor(image, HSV1, COLOR_BGR2HSV);
            if((iLastX != posX) && (iLastY != posY))
            {
              inRange(HSV1, Scalar(h, s, v), Scalar(H, S, V), Final);
            }
            else
            {
              inRange(HSV1, Scalar(0, 0, 0), Scalar(255, 255, 255), Final);
            }
            namedWindow("Final", CV_WINDOW_AUTOSIZE);
            medianBlur(Final, Final, 15);
            medianBlur(Final, Final, 15);
            imshow("Final", Final);  
            iLastX=posX;
            iLastY=posY;
        }
        img = img + imgLines;
        namedWindow("Image", CV_WINDOW_AUTOSIZE);
        imshow("Image", img);
        namedWindow("MotionDetector", CV_WINDOW_AUTOSIZE);
        imshow("MotionDetector", Thresholded);

        int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape
    }
    return 0;
  }