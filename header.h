/*
 * Programmer: Matt Sawyer
 * Class: CptS 121, Spring 2014
 * Battleship
 * Date: 3/25/2014
 *                                                                    
 *Description: The file contains all of the function prototypes, structs, 
 *              enumerated types, constanct macros and standard libraries,
 *              required to run the game of Battleship.    
 */

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define ROWS 10
#define COLS 10

#define LOG_FILE_NAME "battleship.log"

#define NUM_OF_SHIPS  5

#define HORIZONTAL    0
#define VERTICAL      1

#define PLAYER_ONE    0
#define PLAYER_TWO    1


#define WATER         '-'
#define HIT           '*'
#define MISS          'm'

#define CARRIER       'c'
#define BATTLESHIP    'b'
#define CRUISER       'r'
#define SUBMARINE     's'
#define DESTROYER     'd'

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

typedef enum {
	FALSE, TRUE
} Boolean;

typedef enum {
	CARRIER_L    = 5, 
	BATTLESHIP_L = 4,
	CRUISER_L    = 3, 
	SUBMARINE_L  = 3,
	DESTROYER_L  = 2
} ShipType;

typedef struct watercraft {
	char   symbol;
	short  length;
	char   *name;
} WaterCraft;

typedef struct stats {
	int        numHits;
	int        numMisses;
	int        totalShots;
	double     hitMissRatio;
} Stats;

typedef struct coordinate {
	int        row;
	int        column;
} Coordinate;

typedef struct cell {
	char       symbol; 
	Coordinate position;
} Cell;


void welcome_screen (void);
void initialize_game_board (Cell gameboard[][COLS]);
void display_board (Cell gameboard [][COLS], Boolean showpegs);
void place_ship_on_board (Cell gameboard[][COLS], WaterCraft ship, Coordinate position, int direction);
void manually_place_ships_on_board (Cell gameboard[][COLS], WaterCraft ship[]);
void randomly_place_ships_on_board (Cell gameboard[][COLS], WaterCraft ship[]);
void update_board (Cell gameboard[][COLS], Coordinate target);
void check_bounds (Boolean bounds[], int bound, int direction);
void system_message (char *message);

Boolean check_if_sunk_ship (short sunkship[][NUM_OF_SHIPS], short player, char shipsymbol, FILE *stream);
Boolean is_valid_location (Cell gameboard[][COLS], Coordinate position, int direction, int length);
Boolean convert_string_to_position (Coordinate position[], char *stringposition, int length);
Boolean is_winner (Stats players[], int player);
Coordinate generate_position (int direction, int length);
Coordinate get_target (void);

int check_shot (Cell gameboard[][COLS], Coordinate target);
int get_random_number (int lowest, int highest);

#endif