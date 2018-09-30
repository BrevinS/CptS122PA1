#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
#define MAX 50

//Defining this struct before I use it on line 13
//Taken from the eecs website
typedef enum sleep
{
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

//Taken as part of the assignment
typedef struct fitbit
{
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;

//Made this to keep track of data easier
typedef struct data
{
	double caloriesSum;
	double distanceSum;
	double heartRateavg;
	int floorsSum;
	int stepsSum;
} Data;

//Andy's Function
FILE *openFile(char *filename, char *mode);

//Fix unentered data
char *fixNodata(char *string);

//Made this to read the data
int readFile(FILE *infile, char lines[][60]);

//Computes the data using the array and adds the data into the struct
void computeData(char lines[][60], FitbitData computations[1440]);

//Computes the statatics from the data, added the outfile to this so I can add the values to the
//excel document without having to add too much more code
void compute_sums(Data Calculations, FitbitData Computations[1440], FILE *outfile);
