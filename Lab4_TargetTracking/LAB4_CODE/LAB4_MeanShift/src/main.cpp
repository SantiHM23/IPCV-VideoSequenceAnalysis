/* Applied Video Sequence Analysis (AVSA)
 *
 *	LAB4
 *
 * 	Author:	Eduardo Daniel Bravo Solis & Santiago Herrero Melo
 */

//#include <stdio>
#include "mean_shift.h"
#include "helper_functions.h"


//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;
bool init = false;


int main(int argc, char ** argv)
{
	std::string video_path = "/mnt/hgfs/SharedFolder_/LAB4_Files/task2_MeanShift/task2_videos_meanshift";
	//std::string video = "bolt1";
	std::string video = "glove";
	//std::string video = "basketball";
	//std::string video = "road";

	std::string image_path = "%08d.jpg"; //path to images
	ifstream inFile; /* For reading ground truth*/
	bool init = false;

	cv::Mat frame;
	cv::Mat frame_gray;
	VideoCapture cap;//reader to grab videoframes

	std::string inputvideo = video_path + "/" + video + "/" + image_path;
	std::string ground_truth = video_path + "/" + video + "/" + "groundtruth.txt";
	inFile.open(ground_truth.c_str());
	if (!inFile) {
	    cout << "Unable to open file datafile.txt"<<endl;
	}
	Object_Location_T first_coord = Read_First_Line_Ground_Truth(ground_truth);

	cap.open(inputvideo);
	if (!cap.isOpened()) {
		std::cout << "Could not open video file " << inputvideo << std::endl;
		return -1;
	}

	Mat ref_frame;
	Mat target_model;
	Mat target_candidate;
	Rect myROI;
	Point2f prev_center;
	//main loop
	for (;;)
	{
		//get frame & check if we achieved the end of the file (e.g. frame.data is empty)
		cap >> frame;
		if (!frame.data)
			break;
		cvtColor( frame, frame_gray, CV_BGR2GRAY );

		if(!init)
		{

			/*Init algorithm*/
			Point2f ref_coord(first_coord.x_c,first_coord.y_c);
			prev_center = ref_coord;
			cout << "center points x = "<<ref_coord.x<<"  y = "<<ref_coord.y<<endl;
			drawCross(frame, ref_coord,Scalar(0,0,255), 5);

			Rect ROI(first_coord.x_tl,first_coord.y_tl,first_coord.w, first_coord.h);
			myROI = ROI;
			target_model = Compute_PDF(frame_gray,myROI);
			cv::rectangle( frame, cv::Point2f(myROI.x,myROI.y ), cv::Point2f( (myROI.x+myROI.width),(myROI.y+myROI.height)), cv::Scalar( 0,0,255 ),3 );
			drawCross(frame, prev_center,Scalar(0,0,255), 5);
			imshow( "frame", frame );
			init = true;

		}
		else
		{
			/*Run already init*/
			bool converge = false;
			while(!converge)
			{
				target_candidate = Compute_PDF(frame_gray,myROI);
				/* 1 - Get weights*/
				Mat weights(cv::Size(myROI.height,myROI.width),CV_32F);
				weights = 0;
				weights = Compute_Weights(frame_gray,target_model,target_candidate,myROI);
				//imshow( "weights", weights );

				/* 2 - Get new location*/
				Point2f new_center = Compute_New_Center(myROI,weights);

				/* 3 - Compute distance*/
				double distance = cv::norm(new_center-prev_center);
				//cout<<"distance "<< distance<<endl;

				/*Check for convergence*/
				if(distance<=1)
				{
					converge = true;
				}

				if(distance>(frame_gray.cols/2))
				{
					cout<< "Error computing new center. Dimensions exceeded"<<endl;;
					break;
				}
				else
				{
					Rect newROI(new_center.x-floor(myROI.width/2),new_center.y-floor(myROI.height/2),myROI.width,myROI.height);
					myROI = newROI;
					//cout<<"new x "<< new_center.x<< "new y "<< new_center.y<<endl;
				}
				prev_center.x = new_center.x;
				prev_center.y = new_center.y;

				//usleep(100000*1);
				//drawCross(frame, prev_center,Scalar(0,0,255), 5);
			}

			cv::rectangle( frame, cv::Point2f(myROI.x,myROI.y ), cv::Point2f( (myROI.x+myROI.width),(myROI.y+myROI.height)), cv::Scalar( 0,0,255 ),3 );
			imshow( "frame", frame );
			if(waitKey(30) == 27) break;
		}

		//exit if ESC key is pressed
		if(waitKey(30) == 27) break;
	}

	return 0;
}
