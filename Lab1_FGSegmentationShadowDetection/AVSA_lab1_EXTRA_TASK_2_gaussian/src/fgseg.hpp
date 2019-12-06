#include <opencv2/opencv.hpp>

#ifndef FGSEG_H_INCLUDE
#define FGSEG_H_INCLUDE

/*
 * Configurable parameters of the algorithm
 */
/*ALPHA -learning rate of the model*/
#define ALPHA               (0.05)
/*INIT_SIGMA_SQUARED - initial value for the variance (sigma squared) matrix*/
#define INIT_SIGMA_SQUARED  (100)
/*std deviation threshold */
#define STDDTH               (8)


using namespace cv;
using namespace std;

static const int NUM_SIGMAS = 3;
static const int DEFAULT_INIT_SIGMA = 25;

namespace fgseg {
	

	//Declaration of FGSeg class
	class bgs{
	public: 
		
		//constructor with parameter "threshold"
		bgs(double z,double alpha);

		//destructor
		~bgs(void); 

		//method to process the "frame" & return binary FG image
		cv::Mat process(cv::Mat Frame);

		//Method to process new frame and produce foreground mask
		cv::Mat get_new_fgmask(cv::Mat frame);
			
		//method to remove shadows in the binary FG image
		cv::Mat removeShadows();
		
		//Set Background Model
		void setBackgroundModel(cv::Mat frame);
		//Get Background Model
		cv::Mat getBackgroundModel(void);

		//returns the binary FG image
		cv::Mat getFG(){return _fgmask;};
		
		//returns the binary mask with detected shadows
		cv::Mat getShadowMask(){return _shadowmask;};

		/*MatExpr operator ***(const Mat& a);*/

		//ADD ADITIONAL METHODS HERE
		//...


	private:
		float _alpha; //Parameter for adaptation speed
		cv::Mat _bkg; //Background model
		cv::Mat _bkg_color; //Backgroun model in RGB
		cv::Mat	_frame; //current frame
		cv::Mat _fgmask; //binary image for foreground (FG)
		cv::Mat _shadowmask; //binary mask for detected shadows
		cv::Mat _mean;  //Matrix with mean values of each pixel
		cv::Mat _sigma_2; //Matrix with variance (sigma squared) values of each pixel


		//ADD ADITIONAL VARIABLES HERE
		//...

	};//end of class bgs

}//end of namespace

#endif
