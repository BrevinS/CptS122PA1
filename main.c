/*******************************************************************************************
* Programmer: Brevin Simon                                                                 *
* Class: CptS 122, Summer 2018; Lab Section 1L                                             *
* Programming Assignment: PA1                                                              *
* Date: June 9, 2018                                                                       *
* Description: Compute Fitbit data statistics into a .csv file from a .csv file            *
*******************************************************************************************/
#include "header.h"

int main(void)
{
	FILE *infile = NULL, *outfile = NULL;
	//Initializing Structs
	FitbitData computations[1440] = { "",0,0,0,0,0 };
	Data calculations = { 0.0,0.0,0.0,0,0 };
	//Initializing 2 Dimensional Array
	char lines[1440][60] = { { '\0' } };
	//Setting Files, Functions From Class
	infile = openFile("FitbitData.csv", "r");
	outfile = openFile("Results.csv", "w");
	//Reading Files, Unique Code
	readFile(infile, lines);
	//Seperating Data Into Respected Structs
	computeData(lines, computations);
	//Computing Statistics And Moving To Outfile
	compute_sums(calculations, computations, outfile);
	//system("pause");
	//Closing Used Files
	fclose(infile);
	fclose(outfile);
	return 0;
}