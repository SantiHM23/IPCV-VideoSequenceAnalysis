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
		bgs(double threshold, double alpha);

		//destructor
		~bgs(void);

		void setBackgroundModel(cv::Mat Frame, int channel);

		cv::Mat getBackgroundModel(void);

		//method to process the "frame" & return binary FG image
		cv::Mat process(cv::Mat Frame, int channel);

		//method to remove shadows in the binary FG image
		cv::Mat removeShadows();

		//returns the binary FG image
		cv::Mat getFG(){return _fgmask;};

		//returns the binary mask with detected shadows
		cv::Mat getShadowMask(){return _shadowmask;};

		//ADD ADITIONAL METHODS HERE
		//...
	private:
		cv::Mat _bkg; //Background model in color
		cv::Mat	_frame; //current frame
		cv::Mat _fgmask; //binary image for foreground (FG)
		cv::Mat _shadowmask; //binary mask for detected shadows

		double _alpha; //Adaptation speed parameter
		double _threshold;
		//ADD ADITIONAL VARIABLES HERE
		//...

	};//end of class bgs

}//end of namespace

#endif
