/* Applied Video Sequence Analysis (AVSA)
 *
 *	LAB1: Background Subtraction - Unix version
 *
 * 	Author:	Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
 */

//#include <stdio>
#include <iostream> //system libraries
#include <opencv2/opencv.hpp> //opencv libraries

//Header fseg
#include "fgseg.hpp"

//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;

int main(int argc, char ** argv)
{
	Mat img; // current Frame
	Mat ref_img; //Background/Referece
	Mat mask;
	//double t = 0; //variable for execution time

	//Paths for the dataset
	// In this example we assume that the dataset is available at
	// "/home/myuser/dataset2012lite/dataset/baseline/highway/input/in%06d.jpg"
	std::string dataset_path = "/mnt/hgfs/SharedFolder_/AVSA_LAB1_dataset2012lite/dataset2012lite/dataset"; //SET THIS DIRECTORY according to your download
	std::string results_path = "/mnt/hgfs/SharedFolder_/Results_lab1_color_space/results/baseline";//SET THIS DIRECTORY for storing the results
	std::string dataset_cat[1] = {"baseline"};
	//std::string baseline_seq[1] = {"highway"};
	std::string baseline_seq[4] = {"highway","office","pedestrians","PETS2006"};
	std::string image_path = "input/in%06d.jpg"; //path to images

	//background subtraction parameters
	double param1 = 45; //example for one parameter (declare as many variables as needed)
	double alpha = 0.01;
	int channel = 1; //From 0 to 2, select the channel of the image you want to work with
	fgseg::bgs myalgo(param1, alpha); //object of the bgs class

	int NumCat = sizeof(dataset_cat)/sizeof(dataset_cat[0]); //number of categories

	//Loop for all categories
	for (int c=0; c<NumCat; c++ )
	{
		int NumSeq = sizeof(baseline_seq)/sizeof(baseline_seq[0]);  //number of sequence per category

		//Loop for all sequence of each category
		for (int s=0; s<NumSeq; s++ )
		{
			VideoCapture cap;//reader to grab videoframes

			//Compose full path of images
			std::string inputvideo = dataset_path + "/" + dataset_cat[c] + "/" + baseline_seq[s] + "/" +image_path;
			std::cout << "Displaying sequence at " << inputvideo << std::endl;

			//open the videofile to check if it exists
			cap.open(inputvideo);
			if (!cap.isOpened()) {
			std::cout << "Could not open video file " << inputvideo << std::endl;
			return -1;
			}

			//window to visualize results
			namedWindow("Frame");
			namedWindow("fgmask");
			namedWindow("bkg");

			//main loop
			int counter = 0;
			for (;;) {
				

				//get frame
				cap >> img;
				counter++;
				//check if we achieved the end of the file (e.g. img.data is empty)
				if (!img.data)
					break;

				//display images
		               imshow("Frame", img);
		               

				//Apply your bgs algorithm
		        if(counter==1)
		        {
		        	myalgo.setBackgroundModel(img, channel);
				std::cout << "Working with video " << baseline_seq[s].c_str() << std::endl; 
		        	
		        }
				
		        ref_img = myalgo.getBackgroundModel();
				mask = myalgo.process(img, channel);
				
				imshow("bkg", ref_img);
				imshow("fgmask", mask);

				//show results of your bgs algorithm
				//...

				//save results of your bgs algorithm
				char img_file_name[50];
				
				sprintf(img_file_name,"%s/%s/out%06d.png",results_path.c_str(),baseline_seq[s].c_str(),counter);
				
				imwrite(img_file_name, mask);

				//exit if ESC key is pressed
				if(waitKey(30) == 27) break;
			}

			//release all resource
			cap.release();
			destroyAllWindows();
		}
	}
	return 0;
}
