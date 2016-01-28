/*****************************************************************************************************
Author:						Tyler Matthews
FileName:					martianlander.cpp
Purpose:					Perform a martian lander simulation using Eulers method.
							View previous records and repeat simulation
******************************************************************************************************/
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

#define TRUE 1

/* prototypes */
void simulationRun(void);
void userChoice(char *);
void viewRecords(void);
void saveRecords(char, int, int, float, float);
void freeRecords(void);

typedef struct SIM_RECORD /* record */
{
	int minute;
	int second;
	float millisec;
	float speed;
	char win;
} SIM_RECORD;

SIM_RECORD **records; /* array of pointers to records */
int record_num = 0; /* num of records */

float B = 0; /* boost */
float burnR = 100; /* burn reserve */
float speed = 0; /* speed or (v) */
float v2 = 0; /* v2 or vi+1 */
float height = 1000; /* height (x) */
int minute = 0; /* minutes elapsed */
int second = 0; /* seconds elapsed */
float millisec = 0; /* millisecs elapsed */
float g = 3.5; /* constant */
float c = 12.5; /* constant */
float m = 65; /* constant */
float a = 8.0; /* constant */
float vmax = 50; /* constant */
float acceleration = 0; /* accerlartion */

/*****************************************************************************************************
Function Name: 	main
Purpose: 		Call functions to run simulator or stop program
In parameters: 	void
Out parameters:	void
Version: 		1.0
Author: 		Tyler Matthews
******************************************************************************************************/
int main(void)
{
	char choice = 'y'; /* user choice */
	do
	{
		userChoice(&choice);
		if(choice == '1')
			simulationRun();
		else if (choice == '2')
			viewRecords();
	}while(choice != '3');
	 
	freeRecords();
	return 1;
}

/*****************************************************************************************************
Function Name: 	userChoice
Purpose: 		ask the user what they would like to choose. run, view or quit
In parameters: 	an address of a char
Out parameters:	void
Version: 		1.0
Author: 		Tyler Matthews
******************************************************************************************************/
void userChoice(char *choice)
{
	while(TRUE)
	{
		printf("1 to run simulation. 2 to view records. 3 to quit.");
		scanf("%c", choice);
		fflush(stdin);
		if(*choice == '1' || *choice == '2' || *choice == '3')
			break;
		system("cls");
	}
	return;
}

/*****************************************************************************************************
Function Name: 	viewRecords
Purpose: 		view records sorted by speed and time
In parameters: 	void
Out parameters:	void
Version: 		1..0
Author: 		Tyler Matthews
******************************************************************************************************/
void viewRecords(void)
{
	int x = 0; /* loop */
	SIM_RECORD* temp; /* temp record */
	int j; /* loop */
	float num1; /* temp storage */
	float num2; /* temp storage */

	system("cls");
	if(record_num == 0)
		printf("No records\n");
	else
	{
		if(record_num > 1) /* sorting */
		{
			
			/* sort by speed and time, diving by both making both and equal contribution */
		for (x = record_num - 1; x > 0; x--) /* bubble sort algorithm */
			{
				for (j = 1; j <= x; j++)
				{
					num1 = (records[j-1]->speed + (records[j-1]->minute * 60) + records[j-1]->second + (records[j-1]->millisec / 1000)) / 2;
					num2 = (records[j]->speed + (records[j]->minute * 60) + records[j]->second + (records[j]->millisec / 1000)) /2;
					if(num1 > num2)
					{
						temp = records[j-1];
						records[j-1] = records[j];
						records[j] = temp;
					}
				}
			}
		}
		
		printf("Order determined by speed and time as an equal contribution\n");
		for(x = 0; x < record_num; x++)
			printf("Record %d Win: %c Time: %d:%d:%.0f speed: %.3fm/s\n", x+1, records[x]->win, records[x]->minute, 
			records[x]->second, records[x]->millisec, records[x]->speed);
	}
	
	return;
}

/*****************************************************************************************************
Function Name: 	simulationRun
Purpose: 		runs the simulation and displays a win or a loss
In parameters: 	void
Out parameters:	void
Version: 		1.0
Author: 		Tyler Matthews
******************************************************************************************************/
void simulationRun(void)
{
	clock_t beginclock; /* clock to mark beginning */
	clock_t runclock; /* clock to mark each iteration */
	float secondFloat = 0; /* seconds */
	float secondFloat2 = 0; /* new value of seconds */
	int imill = 0; /*temp to calculate millisecs */
	char choice = 0; /* boost press */
	char win; /* user win or lose */
	float timePressed = 0; /* used for delay for boost */
	int pressed = 0;  /* used for delay for boost */

	system("cls");
	beginclock = clock(); /* mark beginning */
	while(TRUE)
	{	
		if(GetAsyncKeyState('W'))
		{
			if(burnR > 0)
			{
				if(pressed == 0) /* dealy for button press */
				{
					B++;
					timePressed = (float)imill;
					pressed = 1;
				}
			}
			choice = 1;
		}
		else if(GetAsyncKeyState('E'))
		{
			if(burnR > 0)
			{
				if(pressed == 0) /* dealy for button press */
				{
					if(B > 0)
						B--;
					timePressed = (float)imill;
					pressed = 1;
				}
			}
			choice = 2;
		}
		if(choice == 0)
			printf("starting descent: burn = %.3f; burn reserve = %.3f "
			"\nminute = %d; second = %d; millisec = %.0f " 
			"\nspeed of descent = %.3fm/s"
			"\nheight above Martian surface = %.3fmeteres",
			B, burnR, minute, second, millisec, speed, height);
		else if(choice == 1)
			printf("W was pressed - increase burn: burn = %.3f; burn reserve = %.3f "
			"\nminute = %d; second = %d; millisec = %.0f " 
			"\nspeed of descent = %.3fm/s"
			"\nheight above Martian surface = %.3fmeteres",
			B, burnR, minute, second, millisec, speed, height);
		else
			printf("E was pressed - decrease burn: burn = %.3f; burn reserve = %.3f "
			"\nminute = %d; second = %d; millisec = %.0f " 
			"\nspeed of descent = %.3fm/s"
			"\nheight above Martian surface = %.3fmeteres",
			B, burnR, minute, second, millisec, speed, height);

		if(height <= 0) /* end condition */
		{
			if(speed < 1)
			{
				win = 'Y';
				printf("\nYou Landed!"
					"\nTime Taken = %d:%d:%.0f\n", minute, second, millisec);
			}
			else
			{
				win = 'N';
				printf("\nYou crashed!"
					"\nTime Taken = %d:%d:%.0f \n", minute, second, millisec);
			}
			saveRecords(win, minute, second, millisec, speed);
			burnR = 100;  /* reset */
			height = 1000;
			minute = second = 0;
			acceleration = v2 = millisec = B = speed = 0;
			fflush(stdin);
			break;
		}

		/* time */
		runclock = clock();
		secondFloat = ( float ) ( runclock - beginclock ) /  CLOCKS_PER_SEC;
		if(minute > 0)
		{
			second = (int)secondFloat;
			second = second % (minute * 60);
		}
		else
			second = (int)secondFloat;
		imill = (int)secondFloat;
		millisec = secondFloat - imill;
		millisec = millisec * 1000;
		if(second == 60)
		{
			minute++;
			second = 0;
		}

		/* main calculations */
		acceleration = g-(c/m)*(speed+a*((speed/vmax)*(speed/vmax)))-B;
		v2 = speed + (acceleration * (secondFloat - secondFloat2));
		height = height - ((v2 + speed) / 2) * (secondFloat - secondFloat2);
		speed = v2;
		
		if(burnR > 0)
			burnR = burnR - (B * (secondFloat - secondFloat2));
		else/* (burnR <= 0) */
		{
			burnR = 0;
			B = 0;
		}
		secondFloat2 = secondFloat;
		
		if(pressed) /* delay for button press */
		{
			if(timePressed < (secondFloat - .989))
				pressed = 0;
		}

		/* reset console cursor  */
		system("cls");
		char a = getc(stdin);
	}
	return;
}

/*****************************************************************************************************
Function Name: 	saveRecords
Purpose: 		saves records in dynamic memory
In parameters: 	win: y or n
				speed: users speed
				minute: time in minutes
				second: time in seconds
				millisec: time in millisecs
Out parameters:	void
Version: 		1.0
Author: 		Tyler Matthews
******************************************************************************************************/
void saveRecords(char win, int minute, int second, float millisec, float speed)
{
	SIM_RECORD** tempRecords;
	int x = 0;

	record_num++;

	tempRecords = (SIM_RECORD**)malloc(sizeof(SIM_RECORD*) * record_num);
	if(record_num > 1)
		for(x = 0; x < (record_num - 1); x++) /* copy address */
			tempRecords[x] = records[x];

	tempRecords[record_num -1] = (SIM_RECORD *)malloc(sizeof(SIM_RECORD) * 1); /* allocate */

	tempRecords[record_num - 1]->second = second; /* assign */
	tempRecords[record_num - 1]->millisec = millisec;
	tempRecords[record_num - 1]->minute = minute;
	tempRecords[record_num - 1]->speed = speed;
	tempRecords[record_num - 1]->win = win;

	free(records);
	records = tempRecords;

	return;
}


/*****************************************************************************************************
Function Name: 	freeRecords
Purpose: 		free records
In parameters: 	void
Out parameters:	void
Version: 		1.0
Author: 		Tyler Matthews
******************************************************************************************************/
void freeRecords(void)
{
	int x = 0;
	if(record_num > 0)
	{
		for(x = 0; x < record_num; x++)
			free(records[x]);
	}
	free(records);
	return;
}
