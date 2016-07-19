/*
 * Programmer: Matt Sawyer
 * Class: CptS 121, Spring 2014
 * Battleship
 * Date: 3/25/2014
 *                                                                    
 *Description: The game of Battleship.                      
 */

#include "header.h"

int main (void)
{
	Stats players[2] = {{0, 0, 0, 0.0}, {0, 0, 0, 0.0}};

	Cell player_one_gameboard[ROWS][COLS];    
	Cell player_two_gameboard[ROWS][COLS];     

	Coordinate target;             // x, y value of a target, target with temporary value, target origin, and AI targeting.
	Coordinate targetTemp;       
	Coordinate targetOrigin;      
	Coordinate targetAI;           

	WaterCraft ship[NUM_OF_SHIPS] = {{'c', 5, "Carrier"}, {'b', 4, "Battleship"}, {'r', 3, "Cruiser"}, {'s', 3, "Submarine"}, {'d', 2, "Destroyer"}};

	Boolean    huntMode       = TRUE;                     //mode of randomly selecting a target
	Boolean    targetMode     = FALSE;                    //mode when there is a hit
	Boolean    flipper        = TRUE;	                  //swaps boolean value
	Boolean    bounds[4]   = {TRUE, TRUE, TRUE, TRUE};
	Boolean    hasAShipSunked = FALSE;                  


	short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2},    
	                                   {5, 4, 3, 3, 2}};

	short player  = 0;	           // 0 is player1, 1 is player2 
	short shot    = 0;             // holds temp value if ship has been shot 
	int   option  = 0;             // option for player to place ship manually or randomly 
	int   north   = 0,             // holds change of value when going north 
		  south   = 0,             // holds change of value when going south 
		  east    = 0,             // holds change of value when going east 
		  west    = 0;             // holds change of value when going west 
	int   i       = 0,             
		  counter = 1;             // i and counter are used as counters

	char  shipsymbol = '\0';       // temporary value to save character symbol of the ship 

	FILE *outStream = NULL;        // stream to output file battleship.log 

	outStream = fopen (LOG_FILE_NAME, "w");

	srand ((unsigned int) time (NULL));

	// Prints welcome screen
	welcome_screen ();
	system_message ("                            Press <ENTER> to continue.\n");
	system ("cls");

	initialize_game_board (player_one_gameboard);
	initialize_game_board (player_two_gameboard);

	printf ("~ Please select an option for placing ships:\n");
	printf ("> [1] Manually place ships\n");
	printf ("> [2] Randomly have ships placed for you.\n");
	printf ("~ Enter Option: ");
	scanf ("%d", &option);

	switch (option) {
		case 1: manually_place_ships_on_board (player_one_gameboard, ship);
	            break;
		case 2: randomly_place_ships_on_board (player_one_gameboard, ship);
				break;
	}

	randomly_place_ships_on_board (player_two_gameboard, ship);
	printf ("> Player2 (Computer's) board has been generated.\n");

	//This program randomly selects player1 or player2 to go first.
	player = get_random_number (0, 1);
	printf ("> Player%d has been randomly selected to go first.\n", player + 1);
	system_message ("> Press <ENTER> to continue.\n");
	system ("cls");

	while (TRUE) {

		//Writes to battleship.log
		fprintf (outStream, "Player%d's turn.\n", player + 1);
		//switches back and forth between player1 and player2
		switch (player)
		{
			case PLAYER_ONE: 
				//Prints player2's game board
				printf ("> Player 2's Board:\n");
				display_board (player_two_gameboard, FALSE);
				printf ("> PLAYER 1'S TURN\n");
				do {
					target = get_target (); 
					shot = check_shot (player_two_gameboard, target);
					if (shot == -1) 
						printf ("> Nope, try inputting another target.\n");
				} while (shot == -1);
				shipsymbol = player_two_gameboard[target.row][target.column].symbol;
				break;
			case PLAYER_TWO: 
				printf ("> Player 1's Board:\n");
				display_board (player_one_gameboard, TRUE);
				printf ("> COMPUTER'S TURN\n");
				if (hasAShipSunked) {
					hasAShipSunked = FALSE;
					targetMode = FALSE;
					huntMode = TRUE;
				}

				if (targetMode) {
					//This mode randomly selects a target, sets mode to true if hit
					target = targetAI;

					do {
						if (bounds[NORTH]) 
						{       
							target.row = north;
						} 
						else if (bounds[SOUTH]) 
						{ 
							target.row = south;
						} 
						else if (bounds[WEST]) 
						{ 
							target.column = west;
						}
						else if (bounds[EAST]) 
						{
							target.column = east;
						}
						else if (!bounds[NORTH] && !bounds[SOUTH] && 
						           !bounds[WEST]  && !bounds[EAST]  && 
								   !hasAShipSunked) 
						{
							target = targetOrigin;
							targetTemp = target;
							north = target.row - counter;
							targetTemp.row = north;
							if (check_shot (player_one_gameboard, targetTemp) != -1 && north >= 0)
							{
								bounds[NORTH] = TRUE;//Checks if NORTH is a hit, does this for NORTH, SOUTH, WEST, and EAST
							}
							targetTemp = target;
							south = target.row + counter;
							targetTemp.row = south;
							if (check_shot (player_one_gameboard, targetTemp) != -1 && south <= 9) 
							{
								bounds[SOUTH] = TRUE;
							}
							targetTemp = target;
							west = target.column - counter;
							targetTemp.column = west;
							if (check_shot (player_one_gameboard, targetTemp) != -1 && west >= 0) 
							{
								bounds[WEST] = TRUE;
							}
							targetTemp = target;
							east = target.column + counter;
							targetTemp.column = east;
							if (check_shot (player_one_gameboard, targetTemp) != -1 && east <= 9) 
							{
								bounds[EAST] = TRUE;
							}
							counter++;
						} 
						else 
						{
							targetMode = FALSE;
							huntMode = TRUE;
							break;
						}
						shot = check_shot (player_one_gameboard, target);
					}
					while (shot == -1 && targetMode == TRUE);
					if (shot == 1 && huntMode == FALSE) 
					{
						for (i = 0; i < 4; i++) 
						{
							if (flipper == FALSE)
								bounds[i] = FALSE;

							if (bounds[i] == flipper) 
								flipper = FALSE;
						}
					}
					else 
					{
						for (i = 0; i < 4; i++) 
						{
							if (flipper == TRUE && bounds[i] != FALSE) 
							{
								bounds[i] = FALSE;
								break;
							}
						}
					}

					flipper = TRUE;
				}
				//Starting mode of player2, randomly selects cells and as targets, when cell is a hit, HUNT set to false and TARGET set to true.
				if (huntMode)
				{	
					counter = 1;
					flipper = TRUE;
					for (i = 0; i < 4; i++)
						bounds[i] = TRUE;
					do 
					{
						target.row = get_random_number (0, 9);
						target.column = get_random_number (0, 9);
						shot = check_shot (player_one_gameboard, target);
					} while (shot == -1);
					if (shot == 1) targetOrigin = target;
				}
				 //1 signals a hit, 0 miss, -1 already shot there.
				if (shot == 1) 
				{
					if (!bounds[NORTH] && !bounds[SOUTH] && 
						!bounds[WEST]  && !bounds[EAST]  && 
						!hasAShipSunked) { target = targetOrigin; }

					huntMode = FALSE;
					targetMode = TRUE;
					targetAI = target;
					if (bounds[NORTH] == TRUE) 
					{
						north = (targetAI.row - 1);
						check_bounds (bounds, north, NORTH);
						targetTemp = target;
						targetTemp.row = north;
						if (check_shot (player_one_gameboard, targetTemp) == -1)
							bounds[NORTH] = FALSE;
					}

					if (bounds[SOUTH] == TRUE) 
					{
						south = targetAI.row + 1;
						check_bounds (bounds, south, SOUTH);
						targetTemp = target;
						targetTemp.row = south;
						if (check_shot (player_one_gameboard, targetTemp) == -1)
							bounds[SOUTH] = FALSE;
					}

					if (bounds[WEST] == TRUE) 
					{
						west  = targetAI.column - 1;
						check_bounds (bounds, west, WEST);
						targetTemp = target;
						targetTemp.column = west;
						if (check_shot (player_one_gameboard, targetTemp) == -1)
							bounds[WEST] = FALSE;
					}

					if (bounds[EAST] == TRUE) 
					{  
						east  = targetAI.column + 1;
						check_bounds (bounds, east, EAST);
						targetTemp = target;
						targetTemp.column = east;
						if (check_shot (player_one_gameboard, targetTemp) == -1)
							bounds[EAST] = FALSE;
					}
				}
				shipsymbol = player_one_gameboard[target.row][target.column].symbol;
				break;
		}

		if (shot == 1) 
		{ // Registers a hit
			printf ("> %d, %d is a hit!\n", target.row, target.column);

			//Writes to battleship.log 
			fprintf (outStream, "%d, %d is a hit!\n", target.row, target.column);
			players[player].numHits++;
			if (player == 1)  
				hasAShipSunked = check_if_sunk_ship (sunkShip, !player, shipsymbol, outStream);
			else
				check_if_sunk_ship (sunkShip, !player, shipsymbol, outStream);

		}
		else
		{ //For a miss
			printf ("> %d, %d is a miss!\n", target.row, target.column);
			fprintf (outStream, "%d, %d is a miss!\n", target.row, target.column);
			players[player].numMisses++;
		}

		if (player == 0) 
			update_board (player_two_gameboard, target);
		else            
			update_board (player_one_gameboard, target);
		if (is_winner (players, player)) {
			printf ("\n> Player%d wins!\n", player + 1);
			fprintf (outStream, "\n~~~~ Player%d wins! ~~~~\n", player + 1);
			break;
		}

		system_message ("> Press <ENTER> to continue.\n");
		player = !player;	
		system ("cls");
	}

	players[0].totalShots = players[0].numHits + players[0].numMisses;
	players[0].hitMissRatio = ((double) players[0].numHits/(double) players[0].numMisses) * 100;
	players[1].totalShots = players[1].numHits + players[1].numMisses;
	players[1].hitMissRatio = ((double) players[1].numHits/(double) players[1].numMisses) * 100;
	fprintf (outStream, "                   STATS                          \n");
	fprintf (outStream, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	fprintf (outStream, " PLAYER1 : %d hits                                \n", players[0].numHits);
	fprintf (outStream, "           %d misses                              \n", players[0].numMisses);
	fprintf (outStream, "           %d total shots                         \n", players[0].totalShots);
	fprintf (outStream, "           %.2lf%% hit/miss ratio                 \n", players[0].hitMissRatio);
	fprintf (outStream, " PLAYER2 : %d hits                                \n", players[1].numHits);
	fprintf (outStream, "           %d misses                              \n", players[1].numMisses);
	fprintf (outStream, "           %d total shots                         \n", players[1].totalShots);
	fprintf (outStream, "           %.2lf%% hit/miss ratio                 \n", players[1].hitMissRatio);
	fprintf (outStream, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	fclose (outStream);
	return 0;
}