/*
 * helper_functions.cpp
 *
 *  Created on: May 1, 2019
 *      Author: daniel
 */

#include "helper_functions.h"

#define DEBUG_MODE true


using namespace std;


Object_Location_T Read_First_Line_Ground_Truth(std::string ground_truth)
{
	Object_Location_T ground_t= {};
	std::ifstream ifs(ground_truth.c_str());
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
			(std::istreambuf_iterator<char>()    ) );
	std::string line_o;

	std::stringstream ss(content);
	std::getline(ss, line_o);

	stringstream line_stream(line_o);
	int a;
	int x_array[4]={};
	int y_array[4]={};
	char comma;

	if (line_stream >> x_array[0] >> comma >> a >> comma >> y_array[0] >> comma >> a >> comma >> \
			x_array[1] >> comma >> a >> comma >> y_array[1] >> comma >> a >> comma >> \
			x_array[2] >> comma >> a >> comma >> y_array[2] >> comma >> a >> comma >> \
			x_array[3] >> comma >> a >> comma >> y_array[3] >> comma )
	{
		cout <<"Correct parsing"<<endl;
	}

	/*Get top left value x*/
	ground_t.x_tl = x_array[0];
	int x_max = x_array[0];
	for(int i=0;i<4;i++)
	{
		if(x_array[i]<=ground_t.x_tl)
		{
			ground_t.x_tl = x_array[i];
		}
		if(x_array[i] >= x_max)
		{
			x_max = x_array[i];
		}
	}
	/*Get top left value y*/
	ground_t.y_tl = y_array[0];
	int y_max = y_array[0];
	for(int i=0;i<4;i++)
	{
		if(y_array[i]<=ground_t.y_tl)
		{
			ground_t.y_tl = y_array[i];
		}
		if(y_array[i] >= y_max)
		{
			y_max = y_array[i];
		}

	}

	ground_t.w = x_max-ground_t.x_tl;
	ground_t.h = y_max-ground_t.y_tl;

	ground_t.x_c = ground_t.x_tl+floor(ground_t.w/2);
	ground_t.y_c = ground_t.y_tl+floor(ground_t.h/2);

	return ground_t;
}
