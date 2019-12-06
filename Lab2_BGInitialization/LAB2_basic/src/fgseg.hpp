#ifndef FGSEG_H_INCLUDE
#define FGSEG_H_INCLUDE
/*
 * LAB2: Background Model and Maintenance
 *
 * 	Authors:	Eduardo Daniel Bravo Solis and Santiago Herrero Melo
 * 	Based on implementation by Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
*/

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define THRESHOLD_DIFF_BKG     (40)
#define ALPHA_RUN_AVG          (0.05)
#define BKG_THRESHOLD          (50)
#define BKG_ESTIMATION         (200)

typedef enum BFground_Seg_Stage_Tag
{
	PROCESS_FINISHED = 0,
	GET_BACKGROUND_MODEL,
	GET_FG_SEGMENTATION
}BFground_Seg_Stage_T;

namespace fgseg {

	//Declaration of FGSeg class
	class bgs{
	public: 
		
		//constructor with parameter "threshold"
		bgs(double threshold,double alpha);

		//destructor
		~bgs(void); 

		/*Process the frames to obtain the background model*/
		void processBackgroundModel(cv::Mat frame);

		/*Finishes the background model stage*/
		void FinishBackgroundModel();

		/*Performs the foreground background segmentation*/
		void processFGSegmentation(cv::Mat frame);

		//Set Image Size
		void initializeFrames(cv::Mat frame);

		//Get Background Model
		cv::Mat getBackgroundModel(void){return _final_background;};

		//Get Color Background Model
		cv::Mat getColorBackgroundModel(void){return _bkg_color;};

		//returns the binary FG image
		cv::Mat getFG(){return _fgmask;};

		//returns the number of frame processed
		int getFrameCounter(){return _frame_counter;};

		// restart frame counter
		void restartFrameCounter(){_frame_counter=0;};

	private:
		float _alpha; //Parameter for adaptation speed
		int _frame_counter; //Number of frames processed
		double _threshold;
		cv::Mat _bkg; //Background model
		cv::Mat _bkg_color; //Background model in RGB
		cv::Mat	_frame; //current frame
		cv::Mat	_frame_color; //current frame
		cv::Mat _fgmask; //binary image for foreground (FG)
		cv::Mat _bkg_counter; //Counter of the foreground objects
		cv::Mat _final_background; //final background gayscale model

	};//end of class bgs

}//end of namespace

#endif
