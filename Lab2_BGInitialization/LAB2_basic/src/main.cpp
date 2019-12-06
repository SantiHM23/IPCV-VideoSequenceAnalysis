/* Applied Video Sequence Analysis (AVSA)
 *
 *	LAB2: Background Model and Maintenance
 * 
 * 	Authors:	Eduardo Daniel Bravo Solis and Santiago Herrero Melo
 * 	Based on implementation by Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
 */

#include <iostream> //system libraries
#include <opencv2/opencv.hpp> //opencv libraries
#include <unistd.h>
//Header fseg
#include "fgseg.hpp"

//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;

int main(int argc, char ** argv)
{
	Mat img; // current Frame
	Mat img_diff;
    //char bkg_file_name[50];
    char fgb_seg_file_name[50];

	//Paths for the dataset MAKE SURE TO SET THE CORRECT PATH WITH THE CORRESPONDING FOLDERS AVAILABLES
	std::string results_summary = "/mnt/hgfs/SharedFolder_/Results_algorithms/LAB2_2/summary";
	std::string dataset_path = "/mnt/hgfs/SharedFolder_/SBMnet_dataset_selected/SBMnet_dataset/selected";
	std::string baseline_seq[13] = {"511","advertisementBoard","AVSS2007","badminton","Blurred","Board","boulevard","boulevardJam","busStation","BusStopMorning","CameraParameter","CUHK_Square","DynamicBackground"};
	std::string image_path = "input/in%06d.jpg"; //path to images
	//background subtraction parameters
	double param1 = THRESHOLD_DIFF_BKG; //example for one parameter (declare as many variables as needed)
	double alpha = ALPHA_RUN_AVG;
	fgseg::bgs myalgo(param1,alpha); //object of the bgs class
	
	//Loop for all categories
		int NumSeq = sizeof(baseline_seq)/sizeof(baseline_seq[0]);  //number of sequence per category

		//Loop for all sequence of each category
		for (int s=0; s<NumSeq; s++ )	
		{	
			VideoCapture cap;//reader to grab videoframes
			
			//Compose full path of images		
			std::string inputvideo = dataset_path + "/" + baseline_seq[s] + "/" +image_path;
			std::cout << "Displaying sequence " << baseline_seq[s]<<" FOR BACKGROUND MODEL at " << inputvideo << std::endl;

			//open the video file to check if it exists
			cap.open(inputvideo);	
			if (!cap.isOpened()) {
			std::cout << "Could not open video file " << inputvideo << std::endl;
			return -1;
			}
			//window to visualize results
			namedWindow("Frame");	
			namedWindow("Background Frame");
			namedWindow("Foreground Segmentation Mask");


			for(;;)
			{
				//get frame
				cap >> img;

				//check if we achieved the end of the file (e.g. img.data is empty)
				if (!img.data)
				{
					/*When all the frames in the video were processed, the algorithm finishes all operations */
					myalgo.FinishBackgroundModel();
					/*Save gray scale background*/
					sprintf(fgb_seg_file_name,"%s/%s_black_white.jpg",results_summary.c_str(),baseline_seq[s].c_str());
					imwrite(fgb_seg_file_name,myalgo.getBackgroundModel());
					/*Save color background model*/
					sprintf(fgb_seg_file_name,"%s/%s_color_background.jpg",results_summary.c_str(),baseline_seq[s].c_str());
					imwrite(fgb_seg_file_name,myalgo.getColorBackgroundModel());
					break;
				}
				else
				{
					/*Process new frame to obtain the background model*/
					myalgo.processBackgroundModel(img);
					imshow("Frame", img);
					imshow("Background Frame",myalgo.getBackgroundModel());
					if(waitKey(30) == 27)
					{
						myalgo.restartFrameCounter();
						break;
					}
				}
			}

			//release all resource
			cap.release();
			destroyAllWindows();

		}
	return 0;
}
