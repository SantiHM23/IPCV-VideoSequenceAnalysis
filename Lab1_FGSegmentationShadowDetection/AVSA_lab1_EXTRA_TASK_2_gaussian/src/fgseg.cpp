#include <opencv2/opencv.hpp>
#include "fgseg.hpp" 

using namespace fgseg;

//default constructor
bgs::bgs(double threshold,double alpha)
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
	_bkg.copyTo(_mean);
	_bkg.copyTo(_sigma_2);
	_sigma_2=INIT_SIGMA_SQUARED;
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

	_fgmask = get_new_fgmask(_frame);
	_fgmask = _fgmask>0;

	return _fgmask;	
}

//method to remove shadows in the binary FG image
cv::Mat bgs::removeShadows()
{
	
	return _shadowmask;
}

cv::Mat bgs::get_new_fgmask(cv::Mat frame)
{
	cv::Mat mask(frame.rows,frame.cols, CV_8UC1);
	double mean;
	double sigma;
	int pix_value=0;
	int mask_value = 0;

	for(int row = 0;row<frame.rows;row++)
	{
		for(int column = 0;column<frame.cols;column++)
		{
			mean = _mean.at<uchar>(row, column);
			sigma = sqrt(_sigma_2.at<uchar>(row, column));
			pix_value = frame.at<uchar>(row, column);


			if( abs(pix_value-mean) > STDDTH*sigma)
			{
				mask_value = 1;
			}
			else
			{
				mask_value = 0;
			}
			mask.at<uchar>(row,column)=mask_value;


			//Update sigma_2 and mean for background pixels
			if(0 == mask_value)
			{
				_mean.at<uchar>(row, column)=_alpha*pix_value+(1-_alpha)*mean;
			}
				_sigma_2.at<uchar>(row, column)=_alpha*pow((pix_value-mean),2)+(1-_alpha)*_sigma_2.at<uchar>(row, column);

		}
	}

	return mask;
}
