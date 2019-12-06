#include <opencv2/opencv.hpp>

#ifndef FGSEG_H_INCLUDE
#define FGSEG_H_INCLUDE

using namespace cv;
using namespace std;

namespace fgseg {
	

	//Declaration of FGSeg class
	class bgs{
	public: 
		
		//constructor with parameter "threshold"
		bgs(double threshold,double alpha);

		//destructor
		~bgs(void); 

		//method to process the "frame" & return binary FG image
		cv::Mat process(cv::Mat Frame);
			
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

		//ADD ADITIONAL METHODS HERE
		//...
	private:
		float _alpha; //Parameter for adaptation speed
		cv::Mat _bkg; //Background model
		cv::Mat _bkg_color; //Backgroun model in RGB
		cv::Mat	_frame; //current frame
		cv::Mat _fgmask; //binary image for foreground (FG)
		cv::Mat _shadowmask; //binary mask for detected shadows


		double _threshold;
		//ADD ADITIONAL VARIABLES HERE
		//...

	};//end of class bgs

}//end of namespace

#endif
