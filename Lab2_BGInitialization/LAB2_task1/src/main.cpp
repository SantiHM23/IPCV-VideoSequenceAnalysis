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
    BFground_Seg_Stage_T alg_stage = PROCESS_FINISHED;

	//Paths for the dataset
	std::string dataset_path = "/mnt/hgfs/SharedFolder_/AVSA_LAB1_dataset2012lite/dataset2012lite/dataset/baseline"; //SET THIS DIRECTORY according to your download
	std::string results_path = "/mnt/hgfs/SharedFolder_/Results_algorithms/LAB2_2";
	std::string baseline_seq[4] = {"highway","office","pedestrians","PETS2006"};

	std::string results_summary = "/mnt/hgfs/SharedFolder_/Results_algorithms/LAB2_2/summary";
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

		/************************************* First stage to get the background model*/
		alg_stage = GET_BACKGROUND_MODEL;
		VideoCapture cap;//reader to grab video frames

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

		while(GET_BACKGROUND_MODEL == alg_stage)
		{
			//get frame
			cap >> img;

			//check if we achieved the end of the file (e.g. img.data is empty)
			if (!img.data)
			{
				/*When all the frames in the video were processed, the algorithm finishes all operations */
				myalgo.FinishBackgroundModel();
				sprintf(fgb_seg_file_name,"%s/%s_black_white.jpg",results_summary.c_str(),baseline_seq[s].c_str());
				imwrite(fgb_seg_file_name,myalgo.getBackgroundModel());
				sprintf(fgb_seg_file_name,"%s/%s_color_background.jpg",results_summary.c_str(),baseline_seq[s].c_str());
				imwrite(fgb_seg_file_name,myalgo.getColorBackgroundModel());

				alg_stage = GET_FG_SEGMENTATION;
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
					alg_stage = PROCESS_FINISHED;
					break;
				}
			}
		}

		/**************************** Second stage, get foreground-background segmentation using info from previous stage*/
		VideoCapture cap2;//reader to grab video frames

		//Compose full path of images
		inputvideo = dataset_path + "/" + baseline_seq[s] + "/" +image_path;
		std::cout << "Displaying sequence "<< baseline_seq[s]<< " FOR FG-BG SEGMENTATION at " << inputvideo << std::endl;

		//open the videofile to check if it exists
		cap2.open(inputvideo);
		if (!cap2.isOpened()) {
			std::cout << "Could not open video file " << inputvideo << std::endl;
			return -1;
		}

		while(GET_FG_SEGMENTATION == alg_stage)
		{
			//get frame
			cap2 >> img;

			if (!img.data)
			{
				/*When all the frames in the video were processed, the algorithm finishes all operations */
				myalgo.restartFrameCounter();
				alg_stage = PROCESS_FINISHED;

			}
			else
			{
				myalgo.processFGSegmentation(img);
				imshow("Frame", img);
				imshow("Foreground Segmentation Mask",myalgo.getFG());
				sprintf(fgb_seg_file_name,"%s/%s/out%06d.png",results_path.c_str(),baseline_seq[s].c_str(),myalgo.getFrameCounter());
				imwrite( fgb_seg_file_name,myalgo.getFG());
				if(waitKey(30) == 27)
				{
					myalgo.restartFrameCounter();
					alg_stage = PROCESS_FINISHED;
					break;
				}
			}
		}

		//release all resource
		cap.release();
		cap2.release();
		destroyAllWindows();

	}
	return 0;
}
