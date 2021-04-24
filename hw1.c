//Timothy Young, CS201, Program #1: Plinko Board Monte Carlo Simulation.
#include "hw1given.h"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>//Used for isxdigit() function(Checks for valid hexadecimal input.)
#include<unistd.h>

int main(int argc, char* argv [])
{
	int rows;//Number of Rows from text file:
	int columns;//Number of Columns/Characters in each line:
	char * ptr;
	FILE *fp;//File pointer to open external data file.
	char* dimensionsP;//Ptr to hold dimension string read from first line.
	int score = 0;//Records the amount of times the puck has landed in a certain slot.
	int* slotNumbers;//Integer string for slot numbers to be coordinated with respective scores.
	int counter = 0;//Input from user how many times they would like to run simulation.
	int a = 0;//Control variable for do-while().
	int blah = 0;

	//Dynamically allocating the string to hold the dimensions read from the text file:
	dimensionsP = (char*)malloc(21*sizeof(char));//Max of 20 characters in first line per requirement.
	char* dimensions = dimensionsP;//Decalre a pointer to allocated memory(avoids memory leaks!)

	printf("Please enter the amount of times you would like to run the simulation:\n");
	scanf("%d",&counter);//Read in the number of times to run the simulation.

	//Dynamically allocating an integer array(used as a bucket to catch score values from while loop):
	int* endP = (int*)malloc(counter*sizeof(int));
	int* end = endP;//Declaring a pointer set to the memory just allocated(avoid leaks).

	for(int i = 0;i<counter;i++)//Initializing integer array to 0:
	{
		end[i] = 0;
	}

	//Seeding the random number generator:
	InitRandom();

	do{
		fp = fopen(argv[1],"r");//Open text file.
		if(fp == NULL)//If NULL, close file immediatley.
		{
			printf("ERROR OPENING TEXT FILE. EXITING.\n");
			exit(EXIT_FAILURE);//EXIT(ERROR)
		}

		//Reading the first line of the text file into dimensions:
		fgets(dimensions,21,fp);//Read up to 20(n) characters in one line!

		//Parsing the string into two seperate integers:
		columns = strtol(dimensions,&ptr,10)+1;
		rows = strtol(ptr+1,NULL,10);//Pointer arithmetic allowed us to parse the line after the delimeter!

		unsigned int start = GetRandom(1, columns-3);//Provides a random starting position for puck.
		printf("_____________________________________\n");

		//Dynamically allocating a virtual plinko board(2-D array):
		char** allocated_Board;//Starts with a ptr to a ptr.
		allocated_Board = (char**)malloc(sizeof(char*)*rows);//First allocate the rows of the board.
		for(int i = 0;i<rows;i++)
		{
			allocated_Board[i]=(char*)malloc(sizeof(char)*columns);//Then allocate space for the columns.
		}

		//Allocating integer string dynamically to hold slot numbers:
		slotNumbers = (int*)malloc(sizeof(int)*columns);

		char** board = allocated_Board;//Avoiding memory leaks.Pointer to dynamic variable.
		int* slotNumbersP = slotNumbers;//Use this variable to manipulate data. Do not use original dynamic variable, avoids memory leaks.

		//Begin reading in board from the text file:
		for(int i = 0;i<rows;i++)
		{
			fscanf(fp, "%s\n", board[i]);//fscanf worked flawlessly. Fgets() caused a TON of issues.
		}

		//Place puck at random starting slot(T):
		board[0][start] = 'T';

		//LeftorRight() is a modified random function to return a -1 or +1 based on the seed at runtime:
		int move = LeftOrRight();

		for(int i=1;i<rows;i++)
		{
			if(board[i][start] == '0')//Always move down first.
			{
				board[i][start] = 'T';//Set slot to T.
				move = LeftOrRight();//Call LeftOrRight() to improve randomization.
			}
			if(board[i][start] == 'X')//Double-Checking that the slot below is not open.
			{
				if(board[i][start+move] == '0')//If possible, move in the direction move designates(left/right).
				{
					board[i][start+move] = 'T';
					start = start + move;//Go to towards the direction of move:
					move = LeftOrRight();
				}
				else//If the spot is not open, move left or right opposite of move:
				{	
					if(move == -1)//If seed = -1, move right.
					{
						board[i][start+1] = 'T';
						start = start+1;//Go right.
						move = LeftOrRight();
					}	
					else//(move == 1)//If seed is +1, move left.
					{
						board[i][start-1] = 'T';
						start = start-1;//Go left.
						move = LeftOrRight();
					}
				}	
			}
		}
		//This loops is needed to store the winning slots into the integer string slotNumbersP:
		for(int i = 1;i<columns-3;i++)
		{
			if(board[rows-1][i] == 'T')
			{	
				slotNumbersP[a] = i;//Winning slots are stored in integer string.
				++score;//Keep track of score for probability calculations.
			}	
		}
		//This loop will print the entirety of the plinko board after dropping a disk:
		for(int i = 0;i<rows;i++)
		{
			printf("%s\n",board[i]);
		}
		//Using end(integer string) bucket to catch the integer string created in while loop:
		end[a] = slotNumbersP[a];//slotNumbersP is local to the loop, cannot allocate outside of loop.

		a = a+1;//Increment control variable.
		system("read -p 'Press Enter to continue...' var");//Pause the system to increase randomization of timer seeding.

		//De-allocate the slotNumber integer string:
		free(slotNumbers);

		//De-allocate the rows of the plink board.
		for(int i = 0;i<rows;i++)
		{
			free(allocated_Board[i]);
		}
		//De-allocate the entire board.
		free(allocated_Board);

		fclose(fp);//Close external file.
	}while(a<counter);//while a<amount of times simulation is to be ran(input from user):

	//Dynamically allocating another intger array:
	int* countP = (int*)malloc(columns*sizeof(int));
	int* count = countP;	

	//Double-nested for loop used for comparing integer string 'end' and calculating probabilities:
	for(int j = 0;j<columns;j++)//J will sequence between the number of columns in text file(check every number):
	{	
		blah = 0;//Restart count at 0 for every iteration of j:
		for(int i = 0;i<counter+1;i++)
		{
			if(end[i] == j)//If one of the slots recorded at spot i equals the number j, increment blah.
			{	//J will equal the slot number the disk fell into to, now we need to count:
				blah = blah + 1;
				count[j] = blah;//Place the amount of repeitions in count[j].
			}	
		}
	}		
	//Dynamically allocating a floating point array for decimals:
	float* totalP = (float*)malloc(columns*sizeof(float));
	float* total = totalP;//Declaring a pointer to allocated memory(avoids memory leaks).

	//This loop produces the probability calculations at the end of the program:
	for(int i = 1;i<columns;i++)
	{
		total[i] = (float)count[i]/(float)counter;//Casting is needed to convert integers to floating point.
		if(total[i]<=1 && total[i]>0)//Only display values that are probabilities:
			printf("Probability of Slot[%d] = %.3f\n",i,total[i]);
	}	

	//Shows counter:
	printf("Counter = %d\n",counter);
	//Shows number of rows:
	printf("Rows = %d\n",rows);
	//Shows number of columns:
	printf("Columns = %d\n",columns);

	free(dimensionsP);//De-allocate dimensions character string.
	free(endP);//De-allocate loop integer array.
	free(countP);//De-allocate bucket integer array.
	free(totalP);//De-allocate floating point array.
return 0;
}

