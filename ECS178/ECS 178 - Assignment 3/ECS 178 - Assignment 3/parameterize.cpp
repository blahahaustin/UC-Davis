#include "ass3.h"

// Declare global variables to hold the parameterizations for mode 1.
extern vector<point> ctrlPts;

vector<double> computeUni()
{
	vector<double> uni;
	int n = ctrlPts.size() - 1;

	for (unsigned int i = 0; i <= n; i++)
	{
		//cout << i << endl;
		uni.push_back( (double)i / (double)n);
		//cout << "uni[" << i << "] = " << uni[i] << endl;
	}
	
	return uni;
}

vector<double> computeChord()
{
	vector<double> chord;	

	double L = 0.0;

	//Get initial chord position.
	chord.push_back(L);

	//Calculate total length of the control polygon, get positions of the chords.
	for (unsigned int i = 1; i < ctrlPts.size(); i++)
	{
		//Calculate the distance between points.
		double D;
		D = sqrt(pow((ctrlPts[i].x - ctrlPts[i - 1].x), 2)
			+ pow((ctrlPts[i].y - ctrlPts[i - 1].y), 2));

		//Add distance to the total length.
		L = L + D;

		//Push chord position to our vector.
		chord.push_back(L);
	}

	//Get normalized value for the chord position.
	for (unsigned int i = 0; i < chord.size(); i++)
	{
		chord[i] = chord[i] / L;
		//cout << "chord[" << i << "] = " << chord[i] << endl;
	}

	return chord;
}

vector<double> computeCent()
{
	vector<double> cent;
	double L = 0.0;
	cent.push_back(L);

	//Calculate total length^1/2 of the control polygon, get positions of the chords.
	for (unsigned int i = 1; i < ctrlPts.size(); i++)
	{
		//Calculate the distance between points.
		double D;
		D = sqrt(pow((ctrlPts[i].x - ctrlPts[i - 1].x), 2)
			+ pow((ctrlPts[i].y - ctrlPts[i - 1].y), 2));
		D = sqrt(D);

		//Add distance to the total length.
		L = L + D;

		//Push chord position to our vector.
		cent.push_back(L);
	}

	//Get normalized value for the centripedal position.
	for (unsigned int i = 0; i < cent.size(); i++)
	{
		cent[i] = cent[i] / L;
		//cout << "cent[" << i << "] = " << cent[i] << endl;
	}

	return cent;
}