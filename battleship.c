/*
 * Programmer: Matt Sawyer
 * Class: CptS 121, Spring 2014
 * Battleship
 * Date: 3/25/2014
 *                                                                    
 *Description: Contains all the defined versions of the function
 *             prototypes from the header file, battleship.h
 *            
 */

#include "header.h"


//This function displays the rules of the game, along with a decal on top.
void welcome_screen (void) {
	printf ("+-+-+-+-+-+-+-+-+-+-+\n");
	printf ("|B|a|t|t|l|e|s|h|i|p|\n");
	printf ("+-+-+-+-+-+-+-+-+-+-+\n"); 
	printf ("\n");
	printf ("RULES:\n\n");
	printf ("1. This is a two player game.\n\n");
	printf ("2. Player1 is you and Player2 is the computer.\n\n");
	printf ("3. Player1 can manually place ships or have ship locations randomly generated.\n\n");
	printf ("4. There are 5 types of ships, the 5 cell long Carrier [c],\n\n"); 
	printf ("   the 4 cell long Battleship [b], the 3 cell Cruiser [r],\n\n");
	printf ("   the 3 cell Submarine [s], and the 2 cell Destroyer [d].\n\n");
	printf ("5. The player to go first is selected randomly.\n\n");
	printf ("6. Try to guess the location of opponent's ships, [*] signals hit [m] miss.\n\n");
	printf ("7. First one to hit all the opposing player's ships wins.\n\n");
}


//This function displays the gameboard with the initial values of the array gameboard.
void initialize_game_board (Cell gameboard[][COLS])
{
	int i = 0, j = 0;

	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) 
		{
			gameboard[i][j].symbol          = WATER;
			gameboard[i][j].position.row    = i;
			gameboard[i][j].position.column = j;
		}
}


//This function prints the game board on the screen.
void display_board (Cell gameboard [][COLS], Boolean showPegs)
{
	int i = 0, j = 0;

	printf ("  0 1 2 3 4 5 6 7 8 9\n");

	for (i = 0; i < ROWS; i++)
	{
		printf ("%d ", i);

		for (j = 0; j < COLS; j++)
		{
			if (showPegs == TRUE) 
				printf ("%c ", gameboard [i][j].symbol);
			else {
				switch (gameboard [i][j].symbol) 
				{
					case HIT:   printf ("%c ", HIT);   break;
					case MISS:  printf ("%c ", MISS);  break;
					case WATER: 
					default:    printf ("%c ", WATER); break;
				}
			}
		}

		putchar ('\n');
	}
}

//Function that allows the random ship placement function 
//to put the ship on the game board depending on the parameters.
void place_ship_on_board  (Cell gameBoard[][COLS], WaterCraft ship, 
	                     Coordinate position, int direction) 
{
	int i = ship.length - 1;

	for (i = 0; i < ship.length; i++)
	{
		if (direction == HORIZONTAL) 
			gameBoard [position.row][position.column + i].symbol = ship.symbol;
		else 
			gameBoard [position.row + i][position.column].symbol = ship.symbol;
	}
}

//Function that allows the user to place each of the 5 types of ships on his/her game board.
void manually_place_ships_on_board (Cell gameBoard[][COLS], WaterCraft ship[])
{
	char       stringPosition[11] = "";
	int        i = 0, j = 0;

	Coordinate position[5];
	Boolean    isValid = FALSE;

	fflush (stdin);

	for (i = 0; i < NUM_OF_SHIPS; i++)
	{

		while (TRUE)
		{
			system ("cls");
			display_board (gameBoard, TRUE);
			printf ("Enter the %d cells to place the %s across (no spaces,ie-010203):\n", ship[i].length, ship[i].name);
			printf ("> ");
			scanf ("%s", stringPosition);
			if (convert_string_to_position (position, stringPosition, ship[i].length)) 
			{

				isValid = TRUE;

				for (j = 0; j < ship[i].length; j++) 
				{

					if (gameBoard[position[j].row][position[j].column].symbol == WATER)
					{
						gameBoard[position[j].row][position[j].column].symbol = ship[i].symbol;
					} 
					else
					{
						isValid = FALSE;
						printf ("> Invalid input.\n");

						if (j != 0)
							while (j >= 0)
							{
								gameBoard[position[j].row][position[j].column].symbol = WATER;
								j--;
							}

						break;
					}
				}
			} 
			else
			{
				isValid = FALSE;
				printf ("> Invalid input.\n");
			}

			if (isValid == TRUE) break;
		}

	}
}

//Lets computer randomly place the 5 types of ships on given board.
void randomly_place_ships_on_board (Cell gameBoard[][COLS], WaterCraft ship[]) 
{
	Coordinate position;
	int direction = -1;
	int i = 0;

	for (i = 0; i < NUM_OF_SHIPS; i++) 
	{
		while (TRUE)
		{
			direction = get_random_number  (0, 1); 
			position = generate_position (direction, ship[i].length);

			if (is_valid_location (gameBoard, position, direction, ship[i].length)) break;
		}

		place_ship_on_board  (gameBoard, ship[i], position, direction);
	}
}

//This function updates the game board with hits/misses.
void update_board (Cell gameBoard[][COLS], Coordinate target) 
{
	switch (gameBoard[target.row][target.column].symbol)
	{
		case WATER: 
			gameBoard[target.row][target.column].symbol = MISS;
			break;
		case CARRIER: 
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			gameBoard[target.row][target.column].symbol = HIT;
			break;

		case HIT:
		case MISS:
		default:
			break;
	}	
}

//Checks if direction is within the bounds, updates array with TRUE if in bounds, FALSE if not.
void check_bounds (Boolean bounds[], int bound, int direction) 
{
	switch (direction) 
	{
		case NORTH: 
			if (bound < 0) 
				bounds[0] = FALSE;
			else            
				bounds[0] = TRUE;
			break;

		case SOUTH:
			if (bound > 9) 
				bounds[1] = FALSE;
			else            
				bounds[1] = TRUE;
			break;

		case WEST:
			if (bound < 0)  
				bounds[2] = FALSE;
			else            
				bounds[2] = TRUE;
			break;

		case EAST:
			if (bound > 9)  
				bounds[3] = FALSE;
			else            
				bounds[3] = TRUE;	
			break;
	}
}

//Prints a message and waits for the user to press the enter/return key.
void system_message (char *message) 
{
	char ch = '\0';

	do {
		printf ("%s", message);
	} while ((ch = _getch()) != '\r');
}

//This function determines if the ship was sunk, if sunk it writes to log file.
Boolean check_if_sunk_ship (short sunkShip[][NUM_OF_SHIPS], short player, char shipsymbol, FILE *stream)
{
	Boolean sunked = FALSE;

	switch (shipsymbol) 
	{
		case CARRIER:    
			if (--sunkShip[player][0] == 0)
			{
				printf ("> Player %d's Carrier sank!\n", player + 1);

				// Write to battleship.log
				fprintf (stream, "Player %d's Carrier sank!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case BATTLESHIP: 
			if (--sunkShip[player][1] == 0)
			{
				printf ("> Player %d's Battleship sank!\n", player + 1);

				// Write to battleship.log 
				fprintf (stream, "Player %d's Battleship sank!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case CRUISER:    
			if (--sunkShip[player][2] == 0)
			{
				printf ("> Player %d's Cruiser sank!\n", player + 1);

				// Writes to battleship.log
				fprintf (stream, "Player %d's Cruiser sank!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case SUBMARINE:  
			if (--sunkShip[player][3] == 0) 
			{
				printf ("> Player %d's Submarine sank!\n", player + 1);

				// Writes to battleship.log
				fprintf (stream, "Player %d's Submarine sank!\n", player + 1);

				sunked = TRUE;
			}
			break;

		case DESTROYER:  
			if (--sunkShip[player][4] == 0)
			{
				printf ("> Player %d's Destroyer sank!\n", player + 1);

				// Writes to battleship.log
				fprintf (stream, "Player %d's Destroyer sank!\n", player + 1);

				sunked = TRUE;
			}
			break;
	}

	return sunked;
}


//Function checks to see if the specified position from
//the random ship placement function is valid on gameboard.
Boolean is_valid_location (Cell gameBoard[][COLS], Coordinate position,  int direction, int length) 
{
	int i = length - 1;
	Boolean isValid = TRUE;

	for (i = 0; isValid && i < length; i++)
	{
		if (direction == HORIZONTAL) {
			if (gameBoard [position.row][position.column + i].symbol != WATER &&
				(position.column + i) < COLS)
				isValid = FALSE;
		} 
		else 
		{ 
			if (gameBoard [position.row + i][position.column].symbol != WATER &&
				(position.row + i) < ROWS)
				isValid = FALSE;
		}
	}

	return isValid;
}


//Converts string coordinates to integer coordinates.
Boolean convert_string_to_position (Coordinate position[], char *stringPosition, int length) 
{
	Boolean flag = TRUE;
	char temp = '\0';
	int i = 0, j = 0, k = 1;

	// checks if length of input is good 
	if (strlen (stringPosition)/2 == length) 
	{
		// loops through the length of the ship 
		for (i = 0; i < length && flag; i++) 
		{
			if (isdigit (stringPosition[j]) && isdigit (stringPosition[k])) 
			{
				position[i].row    = stringPosition[j] - '0';
				position[i].column = stringPosition[k] - '0'; 

				j += 2;
				k += 2;
			} 
			else 
			{
				flag = FALSE;
			}
		}
	} 
	else 
	{
		flag = FALSE;
	}

	return flag;
}

//This function determines if there is a winner based on if all 17 pegs are hit.
Boolean is_winner (Stats players[], int player)
{
	Boolean isWin = FALSE;

	if (players[player].numHits == 17) 
		isWin = TRUE;

	return isWin;
}

//Generates position for ships based on direction 
//and length for the random placement selection.
Coordinate generate_position (int direction, int length)
{
	Coordinate position;

	if (direction == HORIZONTAL)
	{
		position.row    = get_random_number (0, ROWS);
		position.column = get_random_number (0, COLS - length);
	} 
	else 
	{ 
		position.row    = get_random_number (0, ROWS - length);
		position.column = get_random_number (0, COLS);
	}

	return position;
}

//Reads in row and column values input by user.
Coordinate get_target (void)
{
	Coordinate target;

	fflush (stdin);

	printf ("Enter Target (ie 0 1):\n");
	printf ("> ");
	scanf ("%d %d", &target.row, &target.column);

	return target;
}

//Determines if the shot taken was a hit or a miss.
int check_shot (Cell gameBoard[][COLS], Coordinate target)
{
	int hit = -2;

	switch (gameBoard[target.row][target.column].symbol) 
	{
		//miss
		case WATER: 
			hit = 0;
			break;

		//hit
		case CARRIER: 
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			hit = 1;
			break;

		case HIT:
		case MISS:
		default:
			hit = -1;
			break;
	}	

	return hit;
}


//Returns a random number from the parameters to help
//decide who goes first and decide the computer's shots.
int get_random_number (int lowest, int highest) 
{
	if (lowest == 0)
	{
		return rand () % ++highest;
	}
	if (lowest > 0)
	{
		return rand () % ++highest + lowest;
	}
}
