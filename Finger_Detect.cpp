#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;
using namespace cv;
 
 int main(){
  bool track = false;
  bool a = !false;
  bool b = !false;
  bool c = false;
  Mat img,hsv,temp,threshed;
  Mat dilateElement = getStructuringElement(MORPH_ELLIPSE,Size(2,2));
  vector <vector <Point> > contour;
  vector <vector <Point> > contour2;
  vector <vector <Point> > contour3;
  vector <vector <Point> > hullp(1);
  vector <vector <int> > hulli(1);
  vector<Vec4i> defect;
  vector <Vec4i> hierarchy;
  Moments moment;
  VideoCapture cam;
  cam.open(CV_CAP_ANY);

  cam.set(CV_CAP_PROP_FRAME_WIDTH,640);
  cam.set(CV_CAP_PROP_FRAME_HEIGHT,480);

  int h1=0,h2=255,s1=0,s2=255,v1=0,v2=255;
  cvNamedWindow("trackWindow",0);

  createTrackbar("H_MIN","trackWindow",&h1,255);
  createTrackbar("H_MAX","trackWindow",&h2,255);
  createTrackbar("S_MIN","trackWindow",&s1,255);
  createTrackbar("S_MAX","trackWindow",&s2,255);
  createTrackbar("V_MIN","trackWindow",&v1,255);
  createTrackbar("V_MAX","trackWindow",&v2,255);
  int nooffinger=0;
  while(1){
    cam >> img;
    waitKey(10);

    cvtColor(img,hsv,COLOR_BGR2HSV);
    inRange(hsv,Scalar(h1,s1,v1),Scalar(h2,s2,v2),temp);

    medianBlur(temp,threshed,21);
    dilate(threshed,temp,dilateElement);
    
    imshow("threshed",threshed);
    
    findContours(threshed,contour,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
    drawContours(img,contour,-1,Scalar(255,0,0),2);
    if(track && contour.size()){
      // float d = 0;
      int j, maxindex, maxarea=0, area,x , y;
      // float prm;
            for(j=0; j< contour.size(); j++)
            {
                moment = moments((Mat)contour.at(j));
                area= moment.m00;
                if(area>maxarea)
                {
                    maxarea = area;
                    maxindex = j;
                }
            }
            moment = moments((Mat)contour.at(maxindex));
            x = moment.m10/maxarea;
            y = moment.m01/maxarea;
            //drawContours(img,contour,-1,Scalar(255,0,0),2);
            //circle(img, Point(x,y), 2, Scalar(0,255,0), 2);
            //cerr<<'\n'<<x<<'\t'<<y;
            contour2.resize(1);
            contour3.resize(1);
            hulli.resize(1);
            hullp.resize(1);
            //if(a){
            //  approxPolyDP(contour[maxindex],contour2[0] , 50, true);
            //  drawContours(img,contour2,0,Scalar(0,0,255),3);
            // //}
            // // convexHull(contour[maxindex],hulli[0],false);
            // // convexHull(contour[maxindex],hullp[0],false);
            // //if(b){
            approxPolyDP(contour[maxindex],contour3[0] , 50, true);
        drawContours(img,contour3,0,Scalar(0,0,255),2);

        int y_min=240,y_min_index = 0;
        j=0;
        while(j < contour3[0].size()){
          if(contour3[0][j].y < y_min){
            y_min = contour3[0][j].y;
            y_min_index = j;
          }
          j++;
        }
        if((contour3[0][y_min_index-1].y - y_min) > 50 && (contour3[0][y_min_index+1].y - y_min) > 50 ){             //segmentation fault here
          circle(img, contour3[0][y_min_index], 2, Scalar(0,255,0), 2);
          nooffinger = 1;
          if((contour3[0][y_min_index-2].y - y_min) < 25 || (contour3[0][y_min_index+2].y - y_min) < 25)
          {
            nooffinger = 2;
          }  
          if((contour3[0][y_min_index-2].y - y_min) < 25 && (contour3[0][y_min_index+2].y - y_min) < 25)
          {
            nooffinger = 3;
            circle(img, contour3[0][y_min_index], 2, Scalar(0,255,0), 2);
            circle(img, contour3[0][y_min_index+2], 2, Scalar(0,255,0), 2);
            circle(img, contour3[0][y_min_index-2], 2, Scalar(0,255,0), 2);
          }
          

        }

            cerr<<'\t'<<nooffinger << "\n";


            // //}
          //if(c) 
            //d = matchShapes(contour2[0],contour3[0],CV_CONTOURS_MATCH_I1,1);
            //drawContours(img,contour,maxindex,Scalar(255,0,0),3);
            //if(b)
              //drawContours(img,contour3,0,Scalar(0,255,0),3);
            //prm = arcLength(contour2[0],true);
      //if(c){
        // if(hulli[0].size()>3){

        // //cout <<  endl <<  "approx poly vertices \t" << contour2[0].size() ;
        // cerr<< "\td="<< d;//<<'\t'<< a << '\t' << b '\t'<<c;
        // convexityDefects(contour[maxindex],hulli[0],defect[0]);
        // }
      // for(int i = 0; i <contour.size()&& contour.size()<10; ++i){
      //      //find the hulls
      //      convexHull(contour[i], hulli[i], true);
      //      convexHull(contour[i], hullp[i], true);
      //      //find the defects  
      //      convexityDefects(contour[i], hulli[i], defect[i]);
      //      cerr<<"yeah";
      //      }
      // }
    }
    imshow("PI",img);

    char ch = waitKey(30);
    if(ch == 't')
      track = !track;
    else if(ch == 'a')
      a = !a;
    else if(ch == 'b')
      b = !b;
    else if(ch == 'c')
      c = !c;

            int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape

        
    //img.release();
  }
 }
