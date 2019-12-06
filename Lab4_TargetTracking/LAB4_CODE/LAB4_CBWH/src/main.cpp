/* Applied Video Sequence Analysis (AVSA)
 *
 */

//#include <stdio>
#include "mean_shift_CBWH.h"
#include "helper_functions.h"

#define MAX_ITERATION_NUM   50
#define MIN_DISTANCE  1

//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;
bool init = false;


int main(int argc, char ** argv)
{
	std::string video_path = "/mnt/hgfs/SharedFolder_/LAB4_Files/task2_MeanShift/task2_videos_meanshift";
	//std::string video = "ball2";
	std::string video = "glove";
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
	Mat O_u; /*Background model*/
	Mat target_candidate;
	Rect myROI;
	Rect bkg_ROI;
	Point2f prev_center;
	Mat V_u;
	//main loop
	int frame_conter = 0;
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

			/*Target definition*/
			Rect ROI(first_coord.x_tl,first_coord.y_tl,first_coord.w, first_coord.h);
			myROI = ROI;
			target_model = Compute_PDF(frame_gray,myROI);
			cv::rectangle( frame, cv::Point2f(myROI.x,myROI.y ), cv::Point2f( (myROI.x+myROI.width),(myROI.y+myROI.height)), cv::Scalar( 0,0,255 ),3 );
			drawCross(frame, prev_center,Scalar(0,0,255), 5);

			/*Background definition*/
			Object_Location_T bkg_region={};
			bkg_region.x_tl = first_coord.x_tl - floor(first_coord.w/2);
			bkg_region.y_tl = first_coord.y_tl - floor(first_coord.h/2);
			bkg_region.w = 2*first_coord.w;
			bkg_region.h = 2*first_coord.h;
			Rect ROI_2(bkg_region.x_tl,bkg_region.y_tl,bkg_region.w, bkg_region.h);
			bkg_ROI = ROI_2;
			O_u = Compute_Bkg_PDF(frame_gray,bkg_ROI,myROI);
			cv::rectangle( frame, cv::Point2f(bkg_ROI.x,bkg_ROI.y ), cv::Point2f( (bkg_ROI.x+bkg_ROI.width),(bkg_ROI.y+bkg_ROI.height)), cv::Scalar( 0,0,255 ),3 );

			V_u = Get_V_Min_Value(O_u);
			imshow( "frame", frame );
			init = true;
			//usleep(1000000*2);

		}
		else
		{
			/*Run already init*/
			bool converge = false;
			while(!converge)
			{
				frame_conter++;
				target_candidate = Compute_PDF(frame_gray,myROI);
				/* 1 - Get weights*/
				Mat weights(cv::Size(myROI.height,myROI.width),CV_32F);
				weights = 0;
				weights = Compute_Weights(frame_gray,target_model,target_candidate,myROI,V_u);
				//imshow( "weights", weights );
				if(waitKey(30) == 27) break;

				/* 2 - Get new location*/
				Point2f new_center = Compute_New_Center(myROI,weights);

				/* 3 - Compute distance*/
				double distance = cv::norm(new_center-prev_center);
				//cout<<"distance "<< distance<<endl;

				/*Check for convergence*/
				if(distance<=MIN_DISTANCE || frame_conter>MAX_ITERATION_NUM)
				{
					frame_conter = 0;
					converge = true;

					Rect newROI(new_center.x-floor(myROI.width/2),new_center.y-floor(myROI.height/2),myROI.width,myROI.height);
					myROI = newROI;
					//cout<<"new x "<< new_center.x<< "new y "<< new_center.y<<endl;

					int x_tl = myROI.x - floor(myROI.width/2);
					int y_tl = myROI.y - floor(myROI.height/2);
					int w = 2*myROI.width;
					int h = 2*myROI.height;
					Rect bkg_ROI(x_tl,y_tl,w, h);

					Mat O_u_1 = Compute_Bkg_PDF(frame_gray,bkg_ROI,myROI);
					Mat V_u_1 = Get_V_Min_Value(O_u);

					Mat O_u_1_sqrt;
					cv::sqrt(O_u_1.mul(O_u),O_u_1_sqrt);
					double rho = cv::sum(O_u_1_sqrt)[0];

					//cout<<rho<<endl;
					if(rho < 0.2)
					{
						O_u =O_u_1;
						V_u = V_u_1;
					}

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

			}
				cv::rectangle( frame, cv::Point2f(myROI.x,myROI.y ), cv::Point2f( (myROI.x+myROI.width),(myROI.y+myROI.height)), cv::Scalar( 0,0,255 ),3 );
				//drawCross(frame, prev_center,Scalar(0,0,255), 5);
				imshow( "frame", frame );

			//usleep(1000000*2);
			if(waitKey(30) == 27) break;
		}


		//exit if ESC key is pressed
		if(waitKey(30) == 27) break;
	}

	return 0;
}
