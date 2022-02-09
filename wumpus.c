#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void clear_keyboard_buffer(void);
void mapGen(int* hazardSpots);
void adjacentRooms(int currentRoom, int* adjacents);
void playerAction(int* currentRoom, int adj1, int adj2, int adj3, int* arrowsLeft, int* hazardSpots, int* alive, int* wumpusAlive);
void movePlayer(int* currentRoom, int adj1, int adj2, int adj3);
void fireArrow(int currentRoom, int* arrowsLeft, int* hazardSpots, int* alive, int* wumpusAlive);
void arrowTrajectory(int currentRoom, int* alive, int* hazardSpots, int* wumpusAlive, int arrowDistance);
void arrowMovement(int* arrowRoom, int* alive, int* targets, int* hazardSpots, int* wumpusAlive, int originRoom, int arrowDistance);
void oocArrow(int* alive, int* wumpusAlive, int remainingMovement, int* hazardSpots, int originRoom, int playerRoom);
int roomChoiceGet(int adj1, int adj2, int adj3);
void batMove(int* targetRoom, int* hazardSpots, int safeWarp);
int wumpusReaction(int* hazardSpots, int currentRoom, int* alive);
int randomMovementChoice(int adj1, int adj2, int adj3, int isWumpus);
int playAgain(void);
void startingText(int* safeWarp);
void safeWarpChoice(int* safeWarp);
void wait(int delay);

//flip this constant to 1 to enable debug mode
const int debug = 0;

int main(int argc, char* argv[]) {
	time_t t;
	int adjacents[3];
	int alive = 1;
	int currentRoom;
	int hazardSpots[5];
	int arrowsLeft = 5;
	int wumpusAlive = 1;
	int safeWarp = 1;
	srand(time(&t));
	startingText(&safeWarp);
	do {
		alive = 1;
		arrowsLeft = 5;
		wumpusAlive = 1;
		mapGen(hazardSpots);
		do {
			currentRoom = rand() % 20 + 1;
		} while (currentRoom == hazardSpots[0] || currentRoom == hazardSpots[1] || currentRoom == hazardSpots[2] || currentRoom == hazardSpots[3] || currentRoom == hazardSpots[4]);
		do {
			adjacentRooms(currentRoom, adjacents);
			printf("You are in room %d. Adjacent rooms: %d, %d, and %d.\n", currentRoom, adjacents[0], adjacents[1], adjacents[2]);
			printf("You have %d arrows remaining.\n", arrowsLeft);
			if (adjacents[0] == hazardSpots[0] || adjacents[0] == hazardSpots[1] || adjacents[1] == hazardSpots[0] || adjacents[1] == hazardSpots[1] || adjacents[2] == hazardSpots[0] || adjacents[2] == hazardSpots[1]) {
				printf("You feel a draft.\n");
			}
			if (adjacents[0] == hazardSpots[2] || adjacents[0] == hazardSpots[3] || adjacents[1] == hazardSpots[2] || adjacents[1] == hazardSpots[3] || adjacents[2] == hazardSpots[2] || adjacents[2] == hazardSpots[3]) {
				printf("Bats nearby.\n");
			}
			if (adjacents[0] == hazardSpots[4] || adjacents[1] == hazardSpots[4] || adjacents[2] == hazardSpots[4]) {
				printf("You sense the Wumpus.\n");
			}
			if (arrowsLeft == 0 && wumpusAlive == 1) {
				alive = 0;
				printf("You are out of arrows.\nYou lose.\n");
			}
			else if (currentRoom == hazardSpots[4] && alive == 1 && wumpusAlive == 1) {
				alive = wumpusReaction(hazardSpots, currentRoom, &alive);
				if (alive != 0 && wumpusAlive == 1) {
					playerAction(&currentRoom, adjacents[0], adjacents[1], adjacents[2], &arrowsLeft, hazardSpots, &alive, &wumpusAlive);
				}
			}
			else if (currentRoom == hazardSpots[0] || currentRoom == hazardSpots[1] && alive == 1 && wumpusAlive == 1) {
				printf("You have fallen into a hole.\nYou are dead.");
				alive = 0;
			}
			else if (currentRoom == hazardSpots[2] || currentRoom == hazardSpots[3] && alive == 1 && wumpusAlive == 1) {
				printf("You have been picked up by a bat.\n");
				batMove(&currentRoom, hazardSpots, safeWarp);
			}
			else if (alive == 1 && wumpusAlive == 1) {
				playerAction(&currentRoom, adjacents[0], adjacents[1], adjacents[2], &arrowsLeft, hazardSpots, &alive, &wumpusAlive);
			}
			else if (alive == 1 && wumpusAlive == 0) {
				printf("You have slain the wumpus!\nYou win!");
				alive = 0;
			}
			printf("\n");
		} while (alive != 0);
	} while (playAgain() == 1);
	return 0;
}

void mapGen(int* hazardSpots) {
	int hole1;
	int hole2;
	int warp1;
	int warp2;
	int monster;
	hole1 = (rand() % 20) + 1;
	do {
		hole2 = (rand() % 20) + 1;
	} while (hole2 == hole1);
	do {
		warp1 = (rand() % 20) + 1;
	} while (warp1 == hole1 || warp1 == hole2);
	do {
		warp2 = (rand() % 20) + 1;
	} while (warp2 == hole1 || warp2 == hole2 || warp2 == warp1);
	do {
		monster = (rand() % 20) + 1;
	} while (monster == hole1 || monster == hole2 || monster == warp1 || monster == warp2);
	hazardSpots[0] = hole1;
	hazardSpots[1] = hole2;
	hazardSpots[2] = warp1;
	hazardSpots[3] = warp2;
	hazardSpots[4] = monster;
	if (debug) {
		printf("Holes 1 and 2 are in rooms %d and %d. Bats 1 and 2 are in rooms %d and %d. The Wumpus is in room %d.\n\n", hole1, hole2, warp1, warp2, monster);
	}
}

void adjacentRooms(int currentRoom, int* adjacents) {
	switch (currentRoom) {
	case 1:
		adjacents[0] = 2;
		adjacents[1] = 5;
		adjacents[2] = 8;
		break;
	case 2:
		adjacents[0] = 1;
		adjacents[1] = 3;
		adjacents[2] = 10;
		break;
	case 3:
		adjacents[0] = 2;
		adjacents[1] = 4;
		adjacents[2] = 12;
		break;
	case 4:
		adjacents[0] = 3;
		adjacents[1] = 5;
		adjacents[2] = 14;
		break;
	case 5:
		adjacents[0] = 4;
		adjacents[1] = 6;
		adjacents[2] = 1;
		break;
	case 6:
		adjacents[0] = 5;
		adjacents[1] = 7;
		adjacents[2] = 15;
		break;
	case 7:
		adjacents[0] = 6;
		adjacents[1] = 8;
		adjacents[2] = 17;
		break;
	case 8:
		adjacents[0] = 7;
		adjacents[1] = 9;
		adjacents[2] = 1;
		break;
	case 9:
		adjacents[0] = 8;
		adjacents[1] = 10;
		adjacents[2] = 18;
		break;
	case 10:
		adjacents[0] = 9;
		adjacents[1] = 11;
		adjacents[2] = 2;
		break;
	case 11:
		adjacents[0] = 10;
		adjacents[1] = 12;
		adjacents[2] = 19;
		break;
	case 12:
		adjacents[0] = 11;
		adjacents[1] = 13;
		adjacents[2] = 3;
		break;
	case 13:
		adjacents[0] = 12;
		adjacents[1] = 14;
		adjacents[2] = 20;
		break;
	case 14:
		adjacents[0] = 13;
		adjacents[1] = 15;
		adjacents[2] = 4;
		break;
	case 15:
		adjacents[0] = 14;
		adjacents[1] = 16;
		adjacents[2] = 6;
		break;
	case 16:
		adjacents[0] = 15;
		adjacents[1] = 17;
		adjacents[2] = 20;
		break;
	case 17:
		adjacents[0] = 16;
		adjacents[1] = 18;
		adjacents[2] = 7;
		break;
	case 18:
		adjacents[0] = 17;
		adjacents[1] = 19;
		adjacents[2] = 9;
		break;
	case 19:
		adjacents[0] = 18;
		adjacents[1] = 20;
		adjacents[2] = 11;
		break;
	case 20:
		adjacents[0] = 19;
		adjacents[1] = 13;
		adjacents[2] = 16;
		break;
	default:
		printf("This room doesn't exist! Get out!\n");
		adjacents[0] = 1;
		adjacents[1] = 1;
		adjacents[2] = 1;
		break;
	}
}

void playerAction(int* currentRoom, int adj1, int adj2, int adj3, int* arrowsLeft, int* hazardSpots, int* alive, int* wumpusAlive) {
	int noc;
	char c;
	printf("Shoot or move? (S/M)\n");
	do {
		noc = scanf(" %c", &c);
		clear_keyboard_buffer();
		if (noc == 0 || !(c == 's' || c == 'S' || c == 'm' || c == 'M')) {
			printf("Invalid input. Please enter S or M.\n");
			noc = 0;
		}
	} while (noc == 0);
	switch (c) {
	case 'm':
	case 'M':
		movePlayer(currentRoom, adj1, adj2, adj3);
		break;
	case 's':
	case 'S':
		fireArrow(*currentRoom, arrowsLeft, hazardSpots, alive, wumpusAlive);
		break;
	}
}

void movePlayer(int* currentRoom, int adj1, int adj2, int adj3) {
	printf("Your movement options are rooms %d, %d, and %d.\n", adj1, adj2, adj3);
	printf("Please enter your choice. \n");
	*currentRoom = roomChoiceGet(adj1, adj2, adj3);
}

void fireArrow(int currentRoom, int* arrowsLeft, int* hazardSpots, int* alive, int* wumpusAlive) {
	int noc;
	int arrowDistance = 0;
	printf("How far would you like the arrow to fly? (0-5, 0 cancels firing)\n");
	do {
		noc = scanf(" %d", &arrowDistance);
		clear_keyboard_buffer();
		if (noc == 0 || arrowDistance < 0 || arrowDistance > 5) {
			printf("Invalid entry. Please enter between 0 and 5.\n");
			noc = 0;
		}
	} while (noc == 0);
	if (*arrowsLeft > 0 && arrowDistance > 0) {
		//printf("Fired arrow from room %d\n", currentRoom);
		printf("Which rooms would you like the arrow to fly through?\n");
		if (*wumpusAlive == 1) {
			arrowTrajectory(currentRoom, alive, hazardSpots, wumpusAlive, arrowDistance);
			wumpusReaction(hazardSpots, currentRoom, alive);
			*arrowsLeft = *arrowsLeft - 1;
		}
	}
	else if(*arrowsLeft <= 0){
		printf("You have no arrows left to fire.");
	}
	else if (arrowDistance == 0) {
		if (debug)
			printf("arrow distance 0\n");
	}
	//printf("%d\n", *arrowsLeft);
}

void arrowTrajectory(int currentRoom, int* alive, int* hazardSpots, int* wumpusAlive, int arrowDistance) {
	int noc;
	int targets[5] = { 0, 0, 0, 0, 0 };
	int arrowRoom = currentRoom;
	int i;
	
	printf("Please enter %d room(s) for the arrow to fly through. Please hit enter after every room.\n", arrowDistance);
	for (i = 0; i < arrowDistance; i++) {
		do {
			printf("Target %d: ", i + 1);
			noc = scanf(" %d", &targets[i]);
			clear_keyboard_buffer();
			if (noc == 0 || targets[i] < 1 || targets[i] > 20) {
				printf("Invalid room number. Please enter between 1 and 20.\n");
				noc = 0;
			}
		} while (noc == 0);
	}
	arrowMovement(&arrowRoom, alive, targets, hazardSpots, wumpusAlive, currentRoom, arrowDistance);
}

void arrowMovement(int* arrowRoom, int* alive, int* targets, int* hazardSpots, int* wumpusAlive, int originRoom, int arrowDistance) {
	int currentRoom = *arrowRoom;
	int target1adj[3] = { 0, 0, 0 };
	int target2adj[3] = { 0, 0, 0 };
	int target3adj[3] = { 0, 0, 0 };
	int target4adj[3] = { 0, 0, 0 };
	int target5adj[3] = { 0, 0, 0 };
	int inControl[5] = { 1, 1, 1, 1, 1 };
	int oocCalled = 0;
	int n = 0;
	int i;
	adjacentRooms(targets[0], target1adj);
	if(arrowDistance > 1)
	adjacentRooms(targets[1], target2adj);
	if (arrowDistance > 2)
	adjacentRooms(targets[2], target3adj);
	if (arrowDistance > 3)
	adjacentRooms(targets[3], target4adj);
	if (arrowDistance > 4)
	adjacentRooms(targets[4], target5adj);
	if (!(currentRoom == target1adj[0] || currentRoom == target1adj[1] || currentRoom == target1adj[2]) && n < arrowDistance) {
		for (i = 0; i < 5; i++) {
			inControl[i] = 0;
		}
		if (oocCalled == 0) {
			oocArrow(alive, wumpusAlive, arrowDistance, hazardSpots, currentRoom, currentRoom);
			oocCalled = 1;
		}
		if (debug) printf("OOC1\n"); 
	}
	n++;
	if (!(targets[0] == target2adj[0] || targets[0] == target2adj[1] || targets[0] == target2adj[2]) && n < arrowDistance) {
		for (i = 1; i < 5; i++) {
			inControl[i] = 0;
		}
		if (oocCalled == 0) {
			oocArrow(alive, wumpusAlive, arrowDistance - 1, hazardSpots, targets[0], currentRoom);
			oocCalled = 1;
		}
		if (debug) printf("OOC2\n");
	}
	n++;
	if (!(targets[1] == target3adj[0] || targets[1] == target3adj[1] || targets[1] == target3adj[2]) && n < arrowDistance) {
		for (i = 2; i < 5; i++) {
			inControl[i] = 0;
		}
		if (oocCalled == 0) {
			oocArrow(alive, wumpusAlive, arrowDistance - 2, hazardSpots, targets[1], currentRoom);
			oocCalled = 1;
		}
		if (debug) printf("OOC3\n");
	}
	n++;
	if (!(targets[2] == target4adj[0] || targets[2] == target4adj[1] || targets[2] == target4adj[2]) && n < arrowDistance) {
		for (i = 3; i < 5; i++) {
			inControl[i] = 0;
		}
		if (oocCalled == 0) {
			oocArrow(alive, wumpusAlive, arrowDistance - 3, hazardSpots, targets[2], currentRoom);
			oocCalled = 1;
		}
		if (debug) printf("OOC4\n");
	}
	n++;
	if (!(targets[3] == target5adj[0] || targets[3] == target5adj[1] || targets[3] == target5adj[2]) && n < arrowDistance) {
		for (i = 4; i < 5; i++) {
			inControl[i] = 0;
		}
		if (oocCalled == 0) {
			oocArrow(alive, wumpusAlive, arrowDistance - 4, hazardSpots, targets[3], currentRoom);
			oocCalled = 1;
		}
		if (debug) printf("OOC5\n");
	}
	n++;

	if (inControl[0] != 0 && targets[0] == originRoom) {
		*alive = 0;
	}
	else if (inControl[0] != 0 && targets[0] == hazardSpots[4] && *alive == 1) {
		*wumpusAlive = 0;
		*alive = 0;
	}
	else if (inControl[1] != 0 && targets[1] == originRoom) {
		*alive = 0;
	}
	else if (inControl[1] != 0 && targets[1] == hazardSpots[4] && *alive == 1) {
		*wumpusAlive = 0;
		*alive = 0;
	}
	else if (inControl[2] != 0 && targets[2] == originRoom) {
		*alive = 0;
	}
	else if (inControl[2] != 0 && targets[2] == hazardSpots[4] && *alive == 1) {
		*wumpusAlive = 0;
		*alive = 0;
	}
	else if (inControl[3] != 0 && targets[3] == originRoom) {
		*alive = 0;
	}
	else if (inControl[3] != 0 && targets[3] == hazardSpots[4] && *alive == 1) {
		*wumpusAlive = 0;
		*alive = 0;
	}
	else if (inControl[4] != 0 && targets[4] == originRoom) {
		*alive = 0;
	}
	else if (inControl[4] != 0 && targets[4] == hazardSpots[4] && *alive == 1) {
		*wumpusAlive = 0;
		*alive = 0;
	}

	if (*wumpusAlive == 1 && *alive == 0) {
		printf("You have shot yourself.\nYou are dead.\n");
	}
	if (*wumpusAlive == 0 && *alive == 0) {
		printf("You have slain the Wumpus!\nYou win!\n");
	}
	if (debug) {
		printf("wumpusAlive == %d\n", *wumpusAlive);
	}

}

void oocArrow(int* alive, int* wumpusAlive, int remainingMovement, int* hazardSpots, int currentRoom, int playerRoom) {
	int i;
	int adjacents[3];
	for (i = 0; i < remainingMovement; i++) {
		adjacentRooms(currentRoom, adjacents);
		currentRoom = randomMovementChoice(adjacents[0], adjacents[1], adjacents[2], 0);
		if (currentRoom == hazardSpots[4]) {
			*wumpusAlive = 0;
		}
		if (currentRoom == playerRoom) {
			*alive = 0;
		}
		if ((*alive == 1) && (*wumpusAlive == 0)) {
			//printf("Your out-of-control arrow has hit the wumpus!\nYou Win!\n\n");
			i = remainingMovement;
		}
		if ((*alive == 0) && (*wumpusAlive == 1)) {
			//printf("Your out-of-control arrow has hit you!\nYou are dead!\n\n");
			i = remainingMovement;
		}
	}
}

int roomChoiceGet(int adj1, int adj2, int adj3) {
	int output;
	int noc;
	do {
		noc = scanf("%d", &output);
		clear_keyboard_buffer();
		if (noc == 0 || !(output == adj1 || output == adj2 || output == adj3)) {
			printf("Invalid input. Please enter %d, %d, or %d.\n", adj1, adj2, adj3);
			noc = 0;
		}
	} while (noc == 0);

	return output;
}

int randomMovementChoice(int adj1, int adj2, int adj3, int isWumpus) {
	int output;
	int i;
	i = rand() % 3;
	switch (i) {
	case 0:
		output = adj1;
		break;
	case 1:
		output = adj2;
		break;
	case 2:
		output = adj3;
		break;
	default:
		exit(1);
	}
	if (debug && isWumpus == 1) {
		printf("Wumpus moves to %d.\n", output);
	}
	else if (debug) {
		printf("Arrow moves to %d.\n", output);
	}
	return output;
}

void batMove(int* targetRoom, int* hazardSpots, int safeWarp) {
	if (safeWarp) {
		do {
			*targetRoom = rand() % 20 + 1;
		} while (*targetRoom == hazardSpots[0] || *targetRoom == hazardSpots[1] || *targetRoom == hazardSpots[2] || *targetRoom == hazardSpots[3] || *targetRoom == hazardSpots[4]);
	}
	else {
		*targetRoom = rand() % 20 + 1;
	}
}

void startingText(int* safeWarp) {
	printf("The original version of this game was created in 1973 by Gregory Yob.\nCertain elements were inspired by the version on ifiction.org.\n\n");
	wait(2);
	printf("This game takes place in a system of twenty caves which correspond to the vertices of a dodecahedron.\nAs such, each cave is connected to three other caves.\nIn these caves are two holes, two colonies of super bats, you, and the Wumpus.\n\n");
	printf("The objective of the game is to kill the Wumpus by shooting it with an arrow.\nYou lose if you run out of arrows (you start with five), get eaten by the Wumpus, fall in a hole,\n or shoot yourself with an arrow.\n\n");
	wait(2);
	printf("You start in a randomly-selected room. The starting room will never contain a hazard.\n\n");
	printf("On your turn, you have the choice to move to an adjacent room, or to fire an arrow.\nIf you move into a room containing a hole, you fall in and die.\nIf you move into a room containing super bats, they will move you to a random room.\n");
	printf("If you move into the room which the Wumpus is in, you startle it.\nIt will either move to an adjacent room (75%% chance) or eat you (25%% chance).\n");
	printf("The Wumpus is immune to other hazards, as it can cling to walls and the bats cannot get a grip on its shell.\n\n");
	wait(2);
	printf("If you choose to fire an arrow rather than to move, you will be prompted to enter up to five room numbers.\nThe arrow will attempt to pass through these rooms in the order in which you entered them.\nIf it attempts to enter a room which it is not adjacent to, it begins moving randomly.\n");
	printf("If the arrow enters the room containing the Wumpus, it hits it, and you win.\n");
	printf("If the arrow enters your room (regardless of whether you told it to or if it went out of control),\n it hits you, and you die.\n");
	printf("If the Wumpus is still alive after you fire, it gets startled.\nIt will either move to an adjacent room (75%% chance) or stay where it is (25%% chance).\nIf it moves to the room you are in, it eats you.\n\n");
	wait(2);
	printf("If you are adjacent to a cave containing a hazard, you recieve the following warnings:\nHole: \"You feel a draft.\"\nBats: \"Bats nearby.\"\nWumpus: \"You sense the Wumpus.\"\n\n");
	if (debug) {
		printf("You are playing in debug mode. Please note that while I have intentionally left it in the release version,\n I do consider it to be cheating. It is also not designed to be user-friendly.\n\n");
	}
	safeWarpChoice(safeWarp);
}

void safeWarpChoice(int* safeWarp) {
	int noc;
	char c;
	printf("This version of the game allows the player to disable the possibility of the bats dropping you in a hazardous location.\nWould you like to enable safe bat-warping? (Y/N)\n");
	do {
		noc = scanf(" %c", &c);
		clear_keyboard_buffer();
		if (noc == 0 || !(c == 'y' || c == 'n' || c == 'Y' || c == 'N')) {
			printf("Invalid input. Please enter Y or N.\n");
			noc = 0;
		}
	} while (noc == 0);
	switch (c) {
	case 'y':
	case 'Y':
		*safeWarp = 1;
		break;
	case 'n':
	case 'N':
		*safeWarp = 0;
		break;
	}
	if (debug) {
		printf("Safewarp == %d\n", *safeWarp);
	}
}

int wumpusReaction(int* hazardSpots, int currentRoom, int* alive) {
	int wumpusAdjacents[3];
	if (rand() % 4 + 1 == 1) {
	//if(1){
		if (hazardSpots[4] == currentRoom && *alive == 1) {
			printf("You have been eaten by the wumpus.\nYou are dead.");
			*alive = 0;
		}
		return 0;
	}
	else {
		adjacentRooms(hazardSpots[4], wumpusAdjacents);
		if (debug) {
			printf("Wumpus-adjacent rooms are %d, %d, and %d.\n", wumpusAdjacents[0], wumpusAdjacents[1], wumpusAdjacents[2]);
		}
		hazardSpots[4] = randomMovementChoice(wumpusAdjacents[0], wumpusAdjacents[1], wumpusAdjacents[2], 1);
		if (hazardSpots[4] == currentRoom && *alive == 1) {
			printf("You have been eaten by the wumpus.\nYou are dead.");
			*alive = 0;
			return 0;
		}
		else{
			return 1;
		}
	}
}

int playAgain(void) {
	int noc;
	char c;
	printf("Would you like to play again? (y/n)\n");
	do {
		noc = scanf(" %c", &c);
		clear_keyboard_buffer();
		if (noc == 0 || !(c == 'y' || c == 'Y' || c == 'n' || c == 'C')) {
			printf("Invalid input. Please enter y or n.");
			noc = 0;
		}
	} while (noc == 0);
		switch (c) {
		case 'y':
		case 'Y':
			return 1;
			break;
		case 'n':
		case 'N':
			return 0;
			break;
		default: 
			return 0;
		}
	}

void wait(int delay) {
	time_t t;
	time_t startTime;
	time_t currentTime;
	startTime = time(&t);
	currentTime = startTime;
	while (currentTime != (startTime + delay)) {
		currentTime = time(&t);
	}
}

void clear_keyboard_buffer(void) {
	char c;
	int noc;
	noc = scanf("%c", &c);
	while (c != '\n') {
		noc = scanf("%c", &c);
	}
}