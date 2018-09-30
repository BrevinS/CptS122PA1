#include "header.h"

//From lecture, Could have just used fopen in main but my files weren't working so I
//deciding to just try professor o' fallon's code for this part, my files weren't
//working because you cannot choose "existing file" for the FitbitData.csv
FILE *openFile(char *filename, char *mode) {
	FILE *infile = NULL;
	infile = fopen(filename, mode);
	if (infile != NULL) // successfully opened the file
	{
		puts("Opened file successfully!");
	}
	else
	{
		puts("Error\n");
	}
	return infile;
}

char *fixNodata(char *string) {
	int length = strlen(string), temp = 0;
	for (int i = 0; i < length; i++) {
		if (string[i] == ',' && string[i + 1] == ',') {
			for (int j = 0, j2 = 1; j < length - i; j++, j2++) {
				temp = string[i + j2];
				//Insert star into empty place
				string[i + j] = temp;
			}
			string[i + 1] = '*';
		}
	}
	//printf("String: %s", string);
	return string;
}

//Reads info from file
//NOTE: TRIED USING "!feof" BUT WOULD NOT COUNT TO 1440 (instead would count 1440 to 1480)
//In future PA's I will not hardcode such values
int readFile(FILE *infile, char lines[][60]) {
	int index = 0, i = 0;
	char remove[60];
	//Base Case
	//remove first lines
	fgets(remove, 60, infile);
	fgets(lines[i], 60, infile);
	//printf("%s", lines[i]);
	//For all values not equal to 1440
	//!getline
	while (i != 1440)
	{
		i++;
		fgets(lines[i], 60, infile);
		//printf("%s", lines[i]);
	}
	return i;
}

//Example Line: 0:00:00,0.968900025,0,0,63,0,1
//Reads data and sends it into the structs
void computeData(char lines[][60], FitbitData computations[1440]) {
	int i = 0;
	char *nullPtr = NULL;

	//Searching through each line and using string token to assign each value
	while (i < 1440) {
		//Adding starts using fixNodata for empty data when two comas show up
		nullPtr = strtok(fixNodata(lines[i]), ",");
		strcpy(computations[i].minute, nullPtr);

		nullPtr = strtok(NULL, ",");
		computations[i].calories = atof(nullPtr);

		nullPtr = strtok(NULL, ",");
		computations[i].distance = atof(nullPtr);

		nullPtr = strtok(NULL, ",");
		computations[i].floors = atoi(nullPtr);

		nullPtr = strtok(NULL, ",");
		computations[i].heartRate = atoi(nullPtr);

		nullPtr = strtok(NULL, ",");
		computations[i].steps = atoi(nullPtr);

		nullPtr = strtok(NULL, "\n");
		if (nullPtr == NULL) {
			//No integer
			computations[i].sleepLevel = 0;
		}
		else {
			computations[i].sleepLevel = atoi(nullPtr);
		}
		//printf("%d\n", computations[i].sleepLevel);
		i++;
	}
}

//Compute sums of calories, distance, floors, and steps
void compute_sums(Data Calculations, FitbitData Computations[1440], FILE *outfile) {
	int countvalidHR = 0, countBadsleep = 0, saveRecentsequence = 0, flag;
	double maxSteps = Computations[0].steps;
	char badsleepStart[9], badsleepEnd[9];
	//Adding the vales from the lines 2d array into their respective struct datatypes
	for (int i = 0; i < 1440; i++) {
		//Summing up everything
		Calculations.caloriesSum += Computations[i].calories;
		Calculations.distanceSum += Computations[i].distance;
		Calculations.floorsSum += Computations[i].floors;
		Calculations.stepsSum += Computations[i].steps;
		//Confirming that the heartrate is not bad data such as zero or NULL
		if (Computations[i].heartRate > 1 && Computations[i].heartRate < 300) {
			//Counting Legitimate heartrate value to be used for the average
			countvalidHR++;
			Calculations.heartRateavg += Computations[i].heartRate;
		}
		//Finding max (too lazy to write function)
		if (i > 1) {
			if (Computations[i].steps > maxSteps) {
				maxSteps = Computations[i].steps;
			}
		}
		//Finding longest range of sleep > 1, answer around 18
		if (Computations[i].sleepLevel == 2 || Computations[i].sleepLevel == 3) {
			//This will be the remaining the value whent he bad sleep ends
			strcpy(badsleepEnd, Computations[i].minute);
			//Tracking how long the badsleep is
			countBadsleep++;
		}
		else if (Computations[i].sleepLevel == 0 || Computations[i].sleepLevel == 1) {
			/*strcpy(badsleepStart, Computations[i - countBadsleep].minute);*/
			if (countBadsleep > saveRecentsequence) {
				saveRecentsequence = countBadsleep;
				//By doing this I can track the bad sleep start by subtracting it by how long it took to end
				strcpy(badsleepStart, Computations[i - countBadsleep].minute);
			}
			//reset for future values
			countBadsleep = 0;
		}
	}
	//Averaging the heartrate data
	Calculations.heartRateavg /= countvalidHR;
	//Printing for the console for faster checking of values
	printf("Total Calories: %lf\n", Calculations.caloriesSum);
	printf("Total Distance: %lf\n", Calculations.distanceSum);
	printf("Total Floors: %d\n", Calculations.floorsSum);
	printf("Total Steps: %d\n", Calculations.stepsSum);
	printf("Average Heart Rate: %lf\n", Calculations.heartRateavg);
	printf("Highest Steps In Minute: %lf\n", maxSteps);
	printf("Bad Sleep Sum: %s:%s\n", badsleepStart, badsleepEnd);
	//File printing into the outfile
	fprintf(outfile, "Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep\n");
	fprintf(outfile, "%lf, %lf, %d, %d, %lf, %lf, %s:%s", Calculations.caloriesSum, Calculations.distanceSum, Calculations.floorsSum, Calculations.stepsSum, Calculations.heartRateavg, maxSteps, badsleepStart, badsleepEnd);
}