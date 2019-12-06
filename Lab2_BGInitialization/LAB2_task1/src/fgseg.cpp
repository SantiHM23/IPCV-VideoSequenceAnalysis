#include <opencv2/opencv.hpp>
#include "fgseg.hpp" 


using namespace fgseg;

//default constructor
bgs::bgs(double threshold,double alpha)
{
	_threshold = threshold;
	_alpha = alpha; //default value
	_frame_counter = 0;
}

//default destructor
bgs::~bgs(void)
{
}
void bgs::initializeFrames(cv::Mat frame)
{
	frame.copyTo(_frame_color);
	frame.copyTo(_bkg_color);
	_bkg_color=0;
	cvtColor( frame, _bkg, CV_BGR2GRAY );
	_bkg.copyTo(_bkg_counter);
	_bkg.copyTo(_final_background);
	_final_background =0;
	_bkg_counter = 0;
}

void bgs::processBackgroundModel(cv::Mat frame)
{
	_frame_counter++;
	if(_frame_counter == 1)
	{
		initializeFrames(frame);
	}
	//copy input frame to internal variable
	frame.copyTo(_frame_color);
	cvtColor( frame, _frame, CV_BGR2GRAY );

	/*Obtain frame difference*/
	absdiff(_frame,_bkg,_fgmask);
	/*_fgmask = _bkg - _frame;*/
	_fgmask = _fgmask >_threshold;
	/*Progressive update of background model through selective running average*/
	cv::Mat _bkg1 = _alpha*(_frame)+(1-_alpha)*_bkg;
	_bkg1.copyTo(_bkg,(1-_fgmask));

	_bkg_counter = _bkg_counter + (1-_fgmask);
	_bkg_counter= _bkg_counter.mul(1-_fgmask);

	_frame.copyTo(_final_background,(_bkg_counter>BKG_THRESHOLD));
	_frame_color.copyTo(_bkg_color,(_bkg_counter>BKG_THRESHOLD));
}

void bgs::FinishBackgroundModel(void)
{
	/* Instruction to fill anywhere where background was not set*/

	/*In case the frame number is smaller than the background threshold*/
	if(_frame_counter < BKG_THRESHOLD)
	{
		_frame.copyTo(_final_background,(_bkg_counter>=_frame_counter));
		_frame_color.copyTo(_bkg_color,(_bkg_counter>_frame_counter));
	}
	cv::Mat zeros_fill = (_final_background==0);
	_frame.copyTo(_final_background,zeros_fill);
	_frame_color.copyTo(_bkg_color,zeros_fill);

	_bkg =_final_background;



	restartFrameCounter();
}


void bgs::processFGSegmentation(cv::Mat frame)
{
	//copy input frame to internal variable
	frame.copyTo(_frame_color);
	frame.copyTo(_frame);

	cvtColor( _frame, _frame, CV_BGR2GRAY );
	_frame_counter++;

	/*Obtain frame difference*/
	absdiff(_frame,_bkg,_fgmask);
	_fgmask = _fgmask >_threshold;
	/*Progressive update of background model through selective running average*/
	cv::Mat _bkg1 = _alpha*(_frame)+(1-_alpha)*_bkg;
	_bkg1.copyTo(_bkg,(1-_fgmask));
}

