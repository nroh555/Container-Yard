/* ENGGEN131 Project - C Project - 2021 */
/* Container Yard */

/*This script file is created by Naren Rohan at 29th of September 2021 */

#include <stdio.h>
#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3

void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index);
void PrintFloor(int floor[NUM_ROWS][NUM_COLS]);
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width);
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction);
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd);
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked);

/*
Your comment should go here – it should describe the purpose of the function and a
brief summary of the algorithm you have used to solve the task (this comment must
be written in your own words
*/
int main()
{
	int rowA, colA, rowB, colB, isBlocked, result;
	int floor[NUM_ROWS][NUM_COLS];
	/* Simple test code for Tasks 1 and 2 */
	InitialiseFloor(floor, 'T', 3);
	AddContainer(floor,25, 2, 0);
	AddContainer(floor, 35, 3, 1);
	PrintFloor(floor);
	printf("Available Area: %f sq ft\n", FloorAreaAvailable(floor, 10, 5));

	// Move containerA:
	isBlocked = LocateContainer(floor, 'B', &rowA, &colA, &rowB, &colB);
	printf("ContainerA is at: (%d, %d) -(%d, %d)Blocked? %d\n", rowA, colA, rowB, colB, isBlocked);
	result = MoveContainer(floor, rowA, colA, rowB, colB, isBlocked);
	printf("Result = %d\n", result);
	PrintFloor(floor);

	return 0;
}
/* This function will initialise the floor of the container by setting the boundaries to -1
* and the vacancies to 0 and then determine the entry and exit accordingly */
void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
	int i, j;

	// A for loop that will intialise the floor 
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {

			// Defines the boundaries to be -1
			if ((i == 0) || (i == NUM_ROWS - 1) || (j == 0) || (j == NUM_COLS - 1)) {
				floor[i][j] = BOUNDARY;
			}

			// Defines the vacancies 
			else {
				floor[i][j] = VACANT;
			}

			// Defines the entry and the exit if it is at the top
			if (entryboundary == 'T') {
				floor[0][index] = ENTRY;
				floor[NUM_ROWS - 1][index] = EXIT;

			}

			// Defines the entry and the exit if it is at the bottom
			else if (entryboundary == 'B') {
				floor[NUM_ROWS - 1][index] = ENTRY;
				floor[0][index] = EXIT;
			}

			// Defines the entry and the exit if it is at the left
			else if (entryboundary == 'L') {
				floor[index][0] = ENTRY;
				floor[index][NUM_COLS - 1] = EXIT;
			}
			// Defines the entry and the exit if it is at the right
			else if (entryboundary == 'R'){
				floor[index][NUM_COLS - 1] = ENTRY;
				floor[index][0] = EXIT;
			}
		}
	}
}

/*This function would print out the floor based on the specific characters given */
void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
	// A for loop that will loop through each element within the 2D array
	int i, j;
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {

			// Prints the @ for boundary
			if (floor[i][j] == BOUNDARY) {
				printf("@");
			}

			// Prints an empty space for vacancy
			else if (floor[i][j] == VACANT) {
				printf("%c", 32);
			}

			// Prints U for entry
			else if (floor[i][j] == ENTRY) {
				printf("U");
			}

			// Prints X for exit
			else if(floor[i][j] == EXIT) {
				printf("X");
			}

			// Prints containers
			else {
				printf("%c", floor[i][j]);

			}
		}
		printf("\n");
	}
}

/* This function will calculate the total available area(vacant space) */
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width)
{

	// Initialises the amount of free space
	int free = 0;
	int i, j;

	// A for loop that will determine how many vacant cells are there
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (floor[i][j] == VACANT) {
				free++;
			}
		}
	}
	
	//calculate the area of each grid cell
	double cellArea = length * width;

	// Calculates the total area available for storage
	double spaceEmpty = cellArea * free;

	return spaceEmpty;
}

/*This function will determine the position and placement of a particular container*/
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction)
{
	int i, j, length, letter = 64;

	/*This for loop will loop through each element in the array 
	 * and determine the largest ASCII values of the floor array */
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (floor[i][j] > letter) {
				letter = floor[i][j];
			}
		}
	}

	/*Determines the row and column from the corresponding position*/
	i = position / NUM_ROWS;
	j = position % NUM_COLS;

	int SafeToPlace = 1;

	// Checks if the position is vacant or not corresponding to container size
	for (length = 0; length < size; length++) {
	
		if (direction && floor[i + length][j] != VACANT) {
			SafeToPlace = 0;
		}
		else if (!direction && floor[i][j + length] != VACANT) {
			SafeToPlace = 0;
		}
	}

	// Places the container only if required space is unoccupied
	if(SafeToPlace) {
		for (length = 0; length < size; length++) {

			//Vertical placement
			if (direction) {
				floor[i + length][j] = letter + 1;
			}

			// Horizontal placement
			else {
				floor[i][j + length] = letter + 1;
			}
		}
	}
}

/*This function will return the coordinates for the head and tail of the container as well as determine
if it has been blocked or not*/
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd)
{
	int i, j;	

	/*A nested for loop that will determine the tail of the container*/
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (floor[i][j] == move) {
				*rowEnd = i;
				*colEnd = j;
			}
		}
	}

	/*A nested for loop that will determine the head of the container*/
	for (i = NUM_ROWS - 1; i >= 0; i--) {
		for (j = NUM_COLS - 1; j >= 0; j--) {
			if (floor[i][j] == move) {
				*rowStart = i;
				*colStart = j;
			}
		}
	}

	/*Determines the orientation of the container*/
	if (*rowStart == *rowEnd && (floor[*rowStart][*colStart -1] != VACANT && floor[*rowEnd][*colEnd+1] != VACANT)) {

		// Blocked for this horizontal orientation
		return 0;
	}

	else if (*colStart == *colEnd && (floor[*rowStart - 1][*colStart] != VACANT && floor[*rowEnd + 1][*colEnd] != VACANT)) {

		// Blocked for this vertical orientation
		return 0;
	}

	return 1;

}

/*This function will determine if a particular container can be move or not
and therefore move the container until it hits a boundary,exit,entry or another container.
It will then return specified values to notify the postion of the moved container
Created by Naren Rohan at 14/10/2021*/
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked)
{

	// Creates variables that will keep track of the respective movement
	int moveLeft = 0, moveRight = 0, moveTop = 0, moveBottom = 0;
	int j = 1;

	// Deals when container is horizontal
	if (r0 == r1 && isBlocked) {

		// Deals for left movement first
		if (floor[r0][c0 - 1] == VACANT) {

			// This will move the head of the container first
			while (floor[r0][c0 - j] == VACANT) {
				floor[r0][c0 - j] = floor[r0][c0];
				j++;
			}

			// This will move the tail of the container after
			for (moveLeft = 0; moveLeft < j - 1; moveLeft++) {
				floor[r0][c1 - moveLeft] = VACANT;
			}
		}

		// Deals with right movement if left movement fails
		else if (floor[r1][c1 + 1] == VACANT) {
			while (floor[r1][c1 + j] == VACANT) {
				floor[r1][c1 + j] = floor[r1][c1];
				j++;
			}
			for (moveRight = 0; moveRight < j - 1; moveRight++) {
				floor[r0][c0 + moveRight] = VACANT;
			}
		}

		// Deals with potential exit conditions first
		if (floor[r0][c1 + moveRight + 1] == EXIT || floor[r0][c0 - moveLeft - 1] == EXIT) {
			return 2;
		}

		// Deals with potential entry conditions
		else if (floor[r0][c1 + moveRight + 1] == ENTRY || floor[r0][c0 - moveLeft - 1] == ENTRY) {
			return 1;
		}

		else {
			return 0;
		}

	}

	// Deals when the container is vertical
	else if (c0 == c1 && isBlocked) {

		// Deals with top movement first
		if (floor[r0 - 1][c0] == VACANT) {
			while (floor[r0 - j][c0] == VACANT) {
				floor[r0 - j][c0] = floor[r0][c0];
				j++;
			}
			for (moveTop = 0; moveTop < j - 1; moveTop++) {
				floor[r1 - moveTop][c0] = VACANT;
			}
		}

		// Deals with bottom movement after top movement fails
		else if (floor[r1 + 1][c1] == VACANT) {
			while (floor[r1 + j][c1] == VACANT) {
				floor[r1 + j][c1] = floor[r1][c1];
				j++;
			}
			for (moveBottom = 0; moveBottom < j - 1; moveBottom++) {
				floor[r0 + moveBottom][c0] = VACANT;
			}
		}

		// Deals with possible exit conditions first
		if (floor[r0 - moveTop - 1][c0] == EXIT || floor[r1 + moveBottom + 1][c0] == EXIT) {
			return 2;
		}

		// Deals with possible entry conditions
		else if (floor[r0 - moveTop - 1][c0] == ENTRY || floor[r1 + moveBottom + 1][c0] == ENTRY) {
			return 1;
		}

		else {
			return 0;
		}
	}

	// Deals with conditions when container is blocked
	else {

		// Deals for horizontal case for potential entry and exit conditions
		if (r0 == r1) {

			if (floor[r0][c0 - 1] == EXIT || floor[r0][c1 + 1] == EXIT) {
				return 2;
			}
			else if (floor[r0][c0 - 1] == ENTRY || floor[r0][c1 + 1] == ENTRY) {
				return 1;
			}
		}

		// Deals for vertical case for potential entry or exit conditions
		else if (c0 == c1) {

			if (floor[r0 - 1][c0] == EXIT || floor[r1 + 1][c0] == EXIT) {
				return 2;
			}

			else if (floor[r0 - 1][c0] == ENTRY || floor[r1 + 1][c0] == ENTRY) {
				return 1;
			}
		}

		return -1;

	}
}
