#include <opencv2/opencv.hpp>
#include "fgseg.hpp" 

using namespace fgseg;

//default constructor
bgs::bgs(double threshold,double alpha):_threshold(threshold)
{
	_alpha = alpha; //default value
}

//default destructor
bgs::~bgs(void)
{
}

//Set Background Model
void bgs::setBackgroundModel(cv::Mat frame)
{
	frame.copyTo(_bkg_color);
	cvtColor( frame, _bkg, CV_BGR2GRAY );

}

cv::Mat bgs::getBackgroundModel(void)
{
	return _bkg;
}

//method to process the "frame" & return binary FG image
cv::Mat bgs::process(cv::Mat frame)
{
	//copy input frame to internal variable
	frame.copyTo(_frame);
	
	cvtColor( _frame, _frame, CV_BGR2GRAY );

	//ADD YOUR CODE HERE
	//...

	/*Obtain frame difference*/
	absdiff(_frame,_bkg,_fgmask);
	/*_fgmask = _bkg - _frame;*/
	_fgmask = _fgmask >_threshold;

	/*Progressive update of background model through selective running average*/
	cv::Mat _bkg1 = _alpha*(_frame)+(1-_alpha)*_bkg;
	_bkg1.copyTo(_bkg,(1-_fgmask));

	return _fgmask;	
}

//method to remove shadows in the binary FG image
cv::Mat bgs::removeShadows()
{
	//ADD YOUR CODE HERE
	//...
	
	
	return _shadowmask;
}
