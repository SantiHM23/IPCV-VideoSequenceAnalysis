/*
 * mean_shift_CBWH.cpp
 *
 *  Created on: May 4, 2019
 *      Author: daniel
 */

#include "mean_shift_CBWH.h"
using namespace std;

Mat Compute_PDF(Mat frame,Rect myROI)
{
	/// Establish the number of bins
	int histSize = BINS_NUMB;
	Mat croppedImage = frame(myROI);

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	Mat hist_model;
	calcHist( &croppedImage, 1, 0, Mat(), hist_model, 1, &histSize, &histRange, true, false );
	/*Normalize histogram - sum of elements equals 1*/
	normalize(hist_model, hist_model, 1.0, 0.0, NORM_L1);
	return hist_model;

}

Mat Compute_Bkg_PDF(Mat frame,Rect bkg_ROI, Rect myROI)
{
	/// Establish the number of bins
	//Mat croppedImage = frame(myROI);
	Mat mask;
	mask= Mat::zeros(frame.rows,frame.cols,CV_8UC1);
#if 0
	cout<<"bkg_ROI.tl().x "<<bkg_ROI.tl().x <<"bkg_ROI.tl().y "<<bkg_ROI.tl().y<<endl;
	cout<<"myROI.tl().x "<<myROI.tl().x <<"myROI.tl().y "<<myROI.tl().y<<endl;

	cout<<"bkg_ROI.br().x "<<bkg_ROI.br().x <<"bkg_ROI.br().y "<<bkg_ROI.br().y<<endl;
	cout<<"myROI.br().x "<<myROI.br().x <<"myROI.br().y "<<myROI.br().y<<endl;
#endif
	for(int i=bkg_ROI.tl().x;i<bkg_ROI.br().x-1;i++)
	{
		for(int j=bkg_ROI.tl().y;j<bkg_ROI.br().y-1;j++)
		{
			mask.at<char>(j,i) = 1;

			if( i>=myROI.tl().x && i<=myROI.br().x && j>=myROI.tl().y && j<=myROI.br().y )
			{
				mask.at<char>(j,i) = 0;
			}
		}
	}
	Mat mask_1;
	mask_1 = mask*255;

	int histSize = BINS_NUMB;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	Mat hist_model;
	calcHist( &frame, 1, 0, mask, hist_model, 1, &histSize, &histRange, true, false );
	/*Normalize histogram - sum of elements equals 1*/
	normalize(hist_model, hist_model, 1.0, 0.0, NORM_L1);
	//std::cout<<hist_model<<std::endl;
	return hist_model;

}

Mat Get_V_Min_Value(Mat O_u)
{
	Mat V_u=O_u;
	double max_value;
	cv::minMaxLoc(O_u, NULL,&max_value);
	float min_Val = max_value;

	for(int i = 0; i<O_u.rows ; i++)
	{
		if(O_u.at<float>(i) < min_Val && O_u.at<float>(i) != 0)
		{
			min_Val = (float)O_u.at<float>(i);
		}

	}
	V_u = min_Val/O_u;
	threshold(V_u,V_u,1,1,THRESH_TRUNC);
	return V_u;
}

Mat Compute_Weights(Mat Frame, Mat target_PDF,Mat Candidate_PDF, Rect ROI, Mat V_u)
{
    int col_index = ROI.x;
    int row_index = ROI.y;

    int rows = ROI.height;
    int cols = ROI.width;

	Mat weight(rows,cols,CV_32F);

	for(int i=0;i<rows;i++)
	{
		col_index = ROI.x;
		for(int j=0;j<cols;j++)
		{
			int pixel = (Frame.at<uchar>(row_index,col_index));
			pixel = pixel/BIN_WIDTH;
			float v = sqrt(V_u.at<float>(pixel));
			weight.at<float>(i,j) = v*(float)((sqrt(target_PDF.at<float>(pixel)/Candidate_PDF.at<float>(pixel))));
			col_index++;
		}
		row_index++;
	}


	return weight;
}

Point2f Compute_New_Center(Rect myROI,Mat weight)
{
	Point2f center(myROI.x+floor((myROI.width)/2),myROI.y+floor((myROI.height)/2));

	float delta_x = 0.0;
	float delta_y = 0.0;
	float sum_wij = 0.0;
	int mult = 1;
	float centrex = (float)((weight.cols-1)/2.0);
	float centrey = (float)((weight.rows-1)/2.0);

	int counter = 0;
	for(int i=0;i<weight.rows;i++)
	{
		for(int j=0;j<weight.cols;j++)
		{
			float norm_i = (float)((float)i-centrey)/centrey;
			float norm_j = (float)((float)j-centrex)/centrex;
			mult = 0;
			/*Consider only inner circle*/
			if((pow(norm_i,2)+pow(norm_j,2)) >1)
			{
				mult = 1;
			}
			delta_x += (float)(norm_j*weight.at<float>(i,j)*mult);
			delta_y += (float)(norm_i*weight.at<float>(i,j)*mult);
			sum_wij += (float)(weight.at<float>(i,j)*mult);
			counter++;
		}
	}

	center.x += (int)((delta_x/sum_wij)*centrex);
	center.y += (int)((delta_y/sum_wij)*centrey);


	return center;
}
