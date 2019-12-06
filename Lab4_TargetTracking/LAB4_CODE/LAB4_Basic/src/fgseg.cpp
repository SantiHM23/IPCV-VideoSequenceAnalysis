#include <opencv2/opencv.hpp>
#include "fgseg.hpp"

using namespace fgseg;

//default constructor
bgs::bgs(double threshold, double alpha):_threshold(threshold)
{
	_alpha = alpha;
}

//default destructor
bgs::~bgs(void)
{
}

//method to process the "frame" & return binary FG image
cv::Mat bgs::process(cv::Mat frame, int channel)
{
	//copy input frame to internal variable
	cvtColor(frame, frame, CV_BGR2HSV);  //Comment this line if want to work with RGB channels
	Mat bgr[3];
	split(frame,bgr);
	bgr[channel].copyTo(_frame);

	//ADD YOUR CODE HERE
	absdiff(_frame, _bkg, _fgmask);
	_fgmask = _fgmask > _threshold;

	//Background update
	cv::Mat _bkg1 = _alpha*(_frame) + (1-_alpha) * _bkg;
	_bkg1.copyTo(_bkg, (1-_fgmask));

	return _fgmask;
}

//method to remove shadows in the binary FG image
cv::Mat bgs::removeShadows()
{
	//ADD YOUR CODE HERE
	//...


	return _shadowmask;
}

//ADD ADDITIONAL FUNCTIONS HERE
void bgs::setBackgroundModel(cv::Mat Frame, int channel)
{
	cvtColor(Frame, Frame, CV_BGR2HSV);  //Comment this line if want to work with RGB channels
	Mat bgr[3];
	split(Frame,bgr);
	bgr[channel].copyTo(_bkg);
}

cv::Mat bgs::getBackgroundModel()
{
	return _bkg;
}
