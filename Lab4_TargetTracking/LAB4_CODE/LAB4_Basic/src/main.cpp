/* Applied Video Sequence Analysis (AVSA)
 *
 *	LAB1: Background Subtraction - Unix version
 *
 * 	Author:	Eduardo Daniel Bravo Solis & Santiago Herrero Melo
 */

//#include <stdio>
#include <iostream> //system libraries
#include <opencv2/opencv.hpp> //opencv libraries
#include <unistd.h>
#include <opencv2/features2d.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui.hpp"

//Header fseg
#include "fgseg.hpp"
#include "blobfuns.hpp"

#define DEBUG_MODE false
#define drawCross( center, color, d ) \
line( frame, Point( center.x - d, center.y - d ),Point( center.x + d, center.y + d ), color, 1, LINE_AA, 0); \
line( frame, Point( center.x + d, center.y - d ),Point( center.x - d, center.y + d ), color, 1, LINE_AA, 0 )


//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;
bool init = false;

int main(int argc, char ** argv)
{
	//std::string inputvideo = "/mnt/hgfs/SharedFolder_/LAB4_Files/task1_videos_kalman/singleball.mp4";
	//std::string inputvideo = "/mnt/hgfs/SharedFolder_/LAB4_Files/task1_videos_kalman/video2.mp4";
	std::string inputvideo = "/mnt/hgfs/SharedFolder_/LAB4_Files/task1_videos_kalman/video6.mp4";

	cv::Mat frame;
	Mat fgmask; // foreground mask
	VideoCapture cap;//reader to grab videoframes
	std::vector<cvBlob> bloblist; // list for blobs
	KalmanFilter KF(4, 2, 0);
	//KalmanFilter KF(2, 1, 0);
	Mat state(4, 2, CV_32F); /* (phi, delta_phi) */
	//Mat state(2, 1, CV_32F); /* (phi, delta_phi) */
	Mat processNoise(4, 2, CV_32F);
	//Mat measurement = Mat::zeros(1, 1, CV_32F);
	cv::Mat measurement(cv::Size(1,2), CV_32F, Scalar(0));

	//MOG2 approach
	Ptr<BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2(500,100,true);

	//SimpleBlobDetector detector;
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();

	cap.open(inputvideo);
	if (!cap.isOpened()) {
		std::cout << "Could not open video file " << inputvideo << std::endl;
		return -1;
	}
	cap >> frame; //get first video frame

	/*Initialize Kalman Filter variables */

	KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 1, 0, 0,   0, 1, 0, 0,    0, 0, 1, 1,   0, 0, 0, 1);
	//KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
    //setIdentity(KF.measurementMatrix);
    KF.measurementMatrix = (Mat_<float>(2, 4) << 1,0,0,0, 0,0,1,0);
    //setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
    KF.processNoiseCov = (Mat_<float>(4, 4) << 25,0,0,0, 0,10,0,0, 0,0,25,0, 0,0,0,10);
    //setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
    KF.measurementNoiseCov = (Mat_<float>(2, 2) << 25,0, 0,25);
    //setIdentity(KF.errorCovPost, Scalar::all(1));
    KF.errorCovPost = (Mat_<float>(4, 4) << 10000,0,0,0, 0,10,0,0, 0,0,10000,0, 0,0,0,10);
	KF.statePre.at<float>(0) = 0;
	KF.statePre.at<float>(1) = 0;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;
    //randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));


	//main loop
		for (;;)
		{
			bloblist.clear();
			//get frame & check if we achieved the end of the file (e.g. frame.data is empty)
			cap >> frame;
			if (!frame.data)
				break;

			pMOG2->apply(frame, fgmask); //uncomment this line for opencv 3.1+
#if DEBUG_MODE
			imshow("stationary sfgmask", fgmask);
#endif

			extractBlobs(fgmask, bloblist);
			if (bloblist.size() > 1)
			{
				printf("\nWarning - more than one object detected\n");
			}
			if (bloblist.size() == 1)
			{


				cvBlob new_blob = bloblist.back();
				cout<< "center coordinates x = " << new_blob.x_c << " y = " << new_blob.y_c << endl;
				Point2f statePt(state.at<float>(0,0),state.at<float>(0,1));

				if(!init)
				{
					KF.statePre.at<float>(0,0) = new_blob.x_c;
					KF.statePre.at<float>(0,2) = new_blob.y_c;
					Mat test = KF.statePre;
					init = true;

				}

				/* Kalman filter operations */

				/*Prediction*/
	            Mat prediction = KF.predict();
	            Point2f predictPt(prediction.at<float>(0,0),prediction.at<float>(0,2));

	            /*Measurement*/
	            Point2f center(new_blob.x_c, new_blob.y_c);
	            measurement.at<float>(0,0)=(float)new_blob.x_c;
	            measurement.at<float>(0,1)=(float)new_blob.y_c;

	            Point2f measPt(measurement.at<float>(0,0),measurement.at<float>(0,1));

	            /*Correct - Update*/
	            Mat estimated = KF.correct(measurement);
	            Point2f estPt(estimated.at<float>(0,0),estimated.at<float>(2,0));

#if DEBUG_MODE
	            cout<< "***********************************"<<endl;
	            cout<< "statePt x = " << statePt.x << " y = " << statePt.y << endl;
	            cout<< "measPt x = " << measPt.x << " y = " << measPt.y << endl;
	            cout<< "predictPt x = " << predictPt.x << " y = " << predictPt.y << endl;
	            usleep(100000*5);
#endif
	            drawCross( estPt, Scalar(255,255,255), 3 );
	            drawCross( measPt, Scalar(0,0,255), 5 );
	            drawCross( predictPt, Scalar(0,255,0), 3 );
	            line( frame, estPt, measPt, Scalar(0,0,255), 3, LINE_AA, 0 );
	            line( frame, estPt, predictPt, Scalar(0,255,255), 3, LINE_AA, 0 );


	            imshow( "Kalman", frame );



			}
			/*If no blob was found but Kalman tracker already init, predict possition*/
			if((bloblist.size() == 0) && init)
			{

				/*Prediction*/
				Mat prediction = KF.predict();
				Point2f predictPt(prediction.at<float>(0,0),prediction.at<float>(0,2));

				drawCross( predictPt, Scalar(0,255,0), 3 );

#if DEBUG_MODE
				cout<< "***********************************"<<endl;
				cout<< "Blob not found, performing prediction only"<<endl;
				cout<< "predictPt x = " << predictPt.x << " y = " << predictPt.y << endl;
				usleep(100000*5);
#endif
				imshow( "Kalman", frame );


			}

			//exit if ESC key is pressed
			if(waitKey(30) == 27) break;
		}

	return 0;
}
