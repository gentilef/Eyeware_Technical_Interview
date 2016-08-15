#include "opencv2/opencv.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace cv;
using namespace std;

static int mouse_x = -1;
static int mouse_y = -1;
static bool on_click = false;
static bool init = false;

//Detect mouse event
void DetectPosMouse(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == CV_EVENT_LBUTTONDOWN )
     {
    	 cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    	 mouse_x = x;
         mouse_y = y;
         on_click = true;
         init = true;
     }
}
//Find the pixel in the image with the closest rgb input value
void BestMatch(Mat &image, int &x, int&y, int &r, int &g, int &b)
{
	int dist = std::abs(b - image.at<cv::Vec3b>(1,1)[0]) + std::abs(g - image.at<cv::Vec3b>(1,1)[1]) + std::abs(r - image.at<cv::Vec3b>(1,1)[2]);	//initialize the distance
	x = 1;
	y = 1;
	for(int i=1; i <= image.rows; i++ )
	{
		for(int j=1; j<=image.cols; j++)
		{
			int comp = std::abs(b - image.at<cv::Vec3b>(i,j)[0]) + std::abs(g - image.at<cv::Vec3b>(i,j)[1]) + std::abs(r - image.at<cv::Vec3b>(i,j)[2]);
			if(comp < dist)
			{
				dist = comp;
				x=i;
				y=j;
			}
		}
	}
/*	b = image.at<cv::Vec3b>(x,y)[0];
	g = image.at<cv::Vec3b>(x,y)[1];
	r = image.at<cv::Vec3b>(x,y)[2];
	cout << "Blue: " << b << " Green: " << g << " Red: " << r << endl;
	waitKey(0);         */
}

int main(int, char**)
{
	static const double SECONDS = 1; //every SECONDS the image is stored. Unity of measure: s.
	int ref_b=0;
    int ref_g=0;
    int ref_r=0;
    int count = 0;//frame's counter
    system("mkdir -p frames");	//create the folder "frames" in the parent directory where the frames will be saved


	VideoCapture cap(0); // open the default camera
	double fps = cap.get(CV_CAP_PROP_FPS); //frame rate of the camera
	int interval = fps*SECONDS;	//define the frame interval to store the image


    if(!cap.isOpened())
    {
    	cerr << "Error opening the default camera";
    }

    namedWindow("MyCamera",1);
    for(;;)
    {
    	Mat frame;
        cap >> frame; // get a new frame from camera

        if(count%interval == 0)
        {
        	std::string result = "./frames/Frame" + to_string (count) + ".jpg";	//filename of the image stored
        	//std::string result = "./frames/Frame_";
        	//result.append(count, '.jpg');
        	imwrite(result, frame);
        }

        setMouseCallback("MyCamera", DetectPosMouse, &frame);	//listen to the mouse


        if((on_click == false) & (init == true))
        {
        	BestMatch(frame, mouse_x, mouse_y, ref_r,ref_g,ref_b);
        }

        if(on_click == true)
        {
        	ref_b = frame.at<cv::Vec3b>(mouse_x,mouse_y)[0];
        	ref_g = frame.at<cv::Vec3b>(mouse_x,mouse_y)[1];
        	ref_r = frame.at<cv::Vec3b>(mouse_x,mouse_y)[2];
        	cout << "Blue: " << ref_b << " Green: " << ref_g << " Red: " << ref_r << endl;
        	on_click = false;
        }
        if(init == true)
        {
        	circle(frame,Point(mouse_x, mouse_y),10,Scalar(0,0,255),-1);
        }

       imshow("MyCamera", frame);	//show the frame (where we drew the circle)
       count ++;	//increase the frame counter
       if(waitKey(27) >= 0) break; // press esc to close the stream
    }
    return 0;
}



