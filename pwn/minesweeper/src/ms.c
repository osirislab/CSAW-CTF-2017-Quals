/*
This is just a vanilla stack overflow on x64 machine.

It will be around a program that is used for a calculator.

Maybe some kind of lookup table that is reliant on a multiplication which is used as an index into the LUT to be used to "fast" computations, but the data size filtration only sanitizes the operands, not the output. Have an off-by-one bug for the operands or something like that.

1) Write calculator program
2) Write Bug

Minesweeper
Creating minesweeper

****************
Minesweeper Lite
****************

Custom Board 4x4
-give board layout, give board string

Play board 4x4

*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include "malloc.h"

extern ssize_t recvlen(int fd, char *buf, size_t n);
extern ssize_t sendstr(int fd, const char *str);
ssize_t sendlen(int fd, const char *buf, size_t n);
extern void* malloca(uint32_t size);
extern void freea(void *ptr);
extern void memseta(void* buf, char c, int cnt);
extern void strcpya(char* buf, char* str);
extern unsigned int strlena(const char* buf);

#define IS_REDGE(P, RL) ((P + 1) % RL == 0)
#define IS_LEDGE(P, RL) (P % RL == 0)
#define IS_BOT(P, RL, CL) (((P / RL) + 1) == CL)
#define IS_TOP(P, RL) ((P / RL) == 0)
#define UP(P, RL) ((IS_TOP(P, RL)) ? (-1) : (P - RL))
#define DOWN(P, RL, CL) ((IS_BOT(P, RL, CL)) ? (-1) : (P + RL))
#define RIGHT(P, RL) (IS_REDGE(P, RL) ? (-1) : (P + 1))
#define LEFT(P, RL) (IS_LEDGE(P, RL) ? (-1) : (P - 1))
#define IS_WSPACE(x) (x == 0x20)
#define IS_VALID(x) (!IS_WSPACE(x) && x)
#define IS_DIGIT(x) (IS_VALID(x) && ((x-'0') >= 0) && ((x-'0') <= 9))
#define MAX_ROW_LENGTH 10000
#define DEFAULT_SIZE 25
#define DEFAULT_X 5
#define DEFAULT_Y 5

void view_board(int fd, char* board, int xs, int ys) {
	char buf[MAX_ROW_LENGTH];
	memseta(buf, 0, MAX_ROW_LENGTH);
	for (int i = 0; i < ys*ys; i++) {
		memcpy(buf, board+(xs * i), xs);
		buf[xs] = '\n';
		sendlen(fd, buf, xs+1);
	}
	return;
}

void new_game(int fd, char* board, int xs, int ys) {
	char *nboard;
	char cmd[16] = {0};
	char default_board[DEFAULT_SIZE] = {0};
	char *pend;
	long int x, y;
	unsigned int rand_int;
	int rd;
	int xsize, ysize;
	int i = 0, tot = 0, pos = 0;
	int recvret;

	if (!board) {
		rd = open("/dev/random", O_RDONLY);
		if (rd == -1) {
			perror("Opening /dev/random failed!");
		}
		if (read(rd, &rand_int, sizeof(rand_int)) <= 0) {
			perror("Error reading /dev/random");
		}
		srand(rand_int);
		// init board
		for (int i = 0; i < sizeof(default_board); i++) {
			default_board[i] = 'O';
		}
		// set the mine
		default_board[rand() % sizeof(default_board)] = 'X';
		nboard = default_board;
		xsize = DEFAULT_X;
		ysize = DEFAULT_Y;
	}
	else {
		nboard = board;
		xsize = xs;
		ysize = ys;
	}

	sendstr(fd, "Welcome. The board has been initialized to have a random *mine*" \
				"placed in the midst. Your job is to uncover it. You can:\n1) View Board (V)\n" \
				"2) Uncover a location (U X Y). Zero indexed.\n3) Quit game (Q)\n");
	while(1) {
		recvret = recvlen(fd, cmd, sizeof(cmd));
		if (recvret == -1) {
			sendstr(fd, "Goodbye!\n");
			return;
		}
		for (i = 0; i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
		}
		// Entire buffer was spaces
		if (i == sizeof(cmd)) {
			sendstr(fd, "Please enter a valid command! V, U, or Q\n");
			continue;
		}
		switch(cmd[i]) {
			case 0x56:
			case 0x76:
				view_board(fd, nboard, xsize, ysize);
				break;
			/* 	If we are looking to uncover a place,
				then find the co-ordinates and then 
				set them accordingly. Open up all positions
				around them. **EASY MODE***
				X is mine
				O is open
				U is uncovered
			*/
			case 0x55:
			case 0x75:
				i++;
				if (i == sizeof(cmd)) {
					sendstr(fd, "Not enough arguments to uncover. U X Y\n");
					break;
				}
				for (;i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
				}
				if (i == sizeof(cmd)) {
					sendstr(fd, "Not enough arguments to uncover. U X Y\n");
					break;
				}

				tot = 0;
				while(i < sizeof(cmd) && IS_DIGIT(cmd[i])) {
					tot = ((tot*10) + (cmd[i++] - '0'));
				}
				if (i == sizeof(cmd)) {
					sendstr(fd, "Not enough arguments to uncover. U X Y\n");
					break;
				}
				for (; i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
				}
				x = tot;
				tot = 0;
				while(i < sizeof(cmd) && IS_DIGIT(cmd[i])) {
					tot = ((tot*10) + (cmd[i++] - '0'));
				}
				y = tot;

				if (y >= ysize) {
					sendstr(fd, "Y parameter is out of range!\n");
					break;
				}
				if (x >= xsize) {
					sendstr(fd, "X parameter is out of range\n");
					break;
				}
				pos = (y * xsize) + x;

				if (nboard[pos] == 'X') {
					sendstr(fd, "Mine found!\n");
					view_board(fd, nboard, xsize, ysize);
					return;
				}
				else {
					nboard[pos] = 'U';
				}

				if (UP(pos, xsize) != -1) {
					if (nboard[UP(pos, xsize)] == 'X') {
						sendstr(fd, "Mine found!\n");
						view_board(fd, nboard, xsize, ysize);
						return;
					}
					else {
						nboard[UP(pos, xsize)] = 'U';
					}
				}

				if (DOWN(pos, xsize, ysize) != -1) {
					if (nboard[DOWN(pos, xsize, ysize)] == 'X') {
						sendstr(fd, "Mine found!\n");
						view_board(fd, nboard, xsize, ysize);
						return;
					}
					else {
						nboard[DOWN(pos, xsize, ysize)] = 'U';
					}
				}
				if (RIGHT(pos, xsize) != -1) {
					if (nboard[RIGHT(pos, xsize)] == 'X') {
						sendstr(fd, "Mine found!\n");
						view_board(fd, nboard, xsize, ysize);
						return;
					}
					else {
						nboard[RIGHT(pos, xsize)] = 'U';
					}
				}
				if (LEFT(pos, xsize) != -1) {
					if (nboard[LEFT(pos, xsize)] == 'X') {
						sendstr(fd, "Mine found!\n");
						view_board(fd, nboard, xsize, ysize);
						return;
					}
					else {
						nboard[LEFT(pos, xsize)] = 'U';
					}
				}
				break;
			case 0x51:
			case 0x71:
				return;
			default:
				sendstr(fd, "Please enter a valid command!\n");
				break;
		}
	}
}

char* init_game(int fd, int* xs, int* ys) {
	char cmd[16];
	char *board = NULL;
	char *buf;
	int x = 0, y = 0, tot, i;
	int rlen, recvret;
	sendstr(fd, "Please enter in the dimensions of the board you would like to set in this format: B X Y\n");
	recvret = recvlen(fd, cmd, sizeof(cmd));
	if (recvret == -1) {
		sendstr(fd, "Goodbye!\n");
		return 0;
	}
	// malloc small buffer
	buf = malloca(11);
	memseta(buf, 0, 11);
	memcpy(buf, "HI THERE!!\n", 11);
	sendstr(fd, buf);
	freea(buf);
	// malloc large buffer
	buf = malloca(1000);
	memseta(buf, 0, 1000);
	memcpy(buf,	"  +---------------------------+---------------------------+\n" \
				"  |      __________________   |                           |\n" \
				"  |  ==c(______(o(______(_()  | |''''''''''''|======[***  |\n" \
				"  |             )=\\           | |  EXPLOIT   \\            |\n" \
				"  |            / \\            | |_____________\\_______    |\n" \
				"  |           /   \\           | |==[--- >]============\\   |\n" \
				"  |          /     \\          | |______________________\\  |\n" \
				"  |         / RECON \\         | \\(@)(@)(@)(@)(@)(@)(@)/   |\n" \
				"  |        /         \\        |  *********************    |\n" \
				"  +---------------------------+---------------------------+\n" \
				"                                                           \n" \
				"IIIIII    dTb.dTb        _.---._       \n" \
				"  II     4'  v  'B   .\"\"\"\" /|\\`.\"\"\"\". \n" \
				"  II     6.     .P  :  .' / | \\ `.  : \n" \
				"  II     'T;. .;P'  '.'  /  |  \\  `.' \n" \
				"  II      'T; ;P'    `. /   |   \\ .'  \n" \
				"IIIIII     'YvP'       `-.__|__.-'     \n" \
				"-msf                                   \n" , 1000);
	sendstr(fd, buf);
	freea(buf);
	// at this point there should be two nodes on the heap.

	//bug between the index of the array vs the size of the array
	//allocate an array using indices and then the user expects to enter in a size of the full array
	//X - 1
	//Y - 1
	for (i = 0; i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
	}
	if (i == sizeof(cmd)) {
		sendstr(fd, "Please send valid command! B X Y\n");
		return 0;
	}
	switch(cmd[i]) {
		case 0x42:
		case 0x62:
			i++;
			if (i == sizeof(cmd)) {
				sendstr(fd, "Not enough arguments to set board. B X Y\n");
				return 0;
			}

			for (;i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
			}
			if (i == sizeof(cmd)) {
				sendstr(fd, "Not enough arguments to uncover. U X Y\n");
				return 0;
			}

			tot = 0;
			while(i < sizeof(cmd) && IS_DIGIT(cmd[i])) {
				tot = ((tot*10) + (cmd[i++] - '0'));
			}
			if (i == sizeof(cmd)) {
				sendstr(fd, "Not enough arguments to uncover. U X Y\n");
				return 0;
			}
			x = tot;
			for (; i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
			}

			tot = 0;
			while(i < sizeof(cmd) && IS_DIGIT(cmd[i])) {
				tot = ((tot*10) + (cmd[i++] - '0'));
			}
			y = tot;

			if (x >= MAX_ROW_LENGTH || y >= MAX_ROW_LENGTH) {
				sendstr(fd, "Dimension being set is too large\n");
				return 0;
			}
			// take the top 1000 byte node, cut it a bit
			// return the top node, and then make a new node from the bottom one
			// this will take the large node and cut it.
			// but this will only occur if the user allocates 
			// a buffer bigger than 11. the heap sorts blocks
			// this will produce a N1 -> N2 == N1 -> CURR -> N3
			// then N1 -> N3 as CURR is delinked

			board = malloca((x - 1) * (y - 1));
			if ( ((x - 1) * (y-1)) >= 0x1000) {
				sendstr(fd, "Cannot allocate such a large board\n");
				return 0;
			}
			memset(board, 0, (x-1) * (y-1));
			fprintf(stderr, "Allocated buffer of size: %d", ((x-1) * (y-1)));
			break;
		default:
			sendstr(fd, "Please send a valid command! B X Y\n");
			return 0;
	}
	while (1) {
		sendstr(fd, "Please send the string used to initialize the board. Please send X * Y bytes follow by a newline"\
				"Have atleast 1 mine placed in your board, marked by the character X\n");
		// overflow into the bottom node
		// This should overflow CURR and into N3
		rlen = recvlen(fd, board, (x * y)+1);
		if (rlen == -1) {
			sendstr(fd, "Goodbye!\n");
			return 0;
		}
		if (strstr(board, "X") && rlen == (x*y)+1) {
			break;
		}
	}
	// TODO: This triggers the free() and the overwrite
	// Now, at this stage
	// N1 -> N3 but N3 has a corrupted header
	// since 200 will probably fit in not the 11 byte block
	// only other byte block it would fit in is this one
	// unless the user is putting in low bytes into this size header
	// When the top block becomes allocated, this will crash in delink
	buf = malloca(200);
	memset(buf, 0, 200);
	memcpy(buf, \
				"____________\n" \
				"< cowsay <3 minesweeper >\n" \
				" ------------          \n" \
				"       \\   ,__,        \n" \
				"        \\  (oo)____    \n" \
				"           (__)    )\\  \n" \
				"              ||--|| * \n" , 160);
	sendstr(fd, buf);
	freea(buf);
	*ys = y;
	*xs = x;
	return board;
}

/* Minesweeper */
int ms(int fd) {
	char cmd[16] = {0};
	char *curr_board = 0;
	int xs = 0, ys = 0, i;
	int recvret;

	while(1) {
		sendstr(fd, "\nHi. Welcome to Minesweeper. Please select an option:" \
			        "\n1) N (New Game)\n2) Initialize Game(I)\n3) Q (Quit)\n");
		recvret = recvlen(fd, cmd, sizeof(cmd));
		if (recvret == -1) {
			sendstr(fd, "Goodbye!\n");
			return 0;
		}
		for (i = 0; i < sizeof(cmd) && !IS_VALID(cmd[i]); i++) {
		}
		if (i == sizeof(cmd)) {
			sendstr(fd, "No command string entered! N, I, or Q please!\n");
			continue;
		}
		switch(cmd[i]) {
			case 0x4e:
			case 0x6e:
				new_game(fd, curr_board, xs, ys);
				break;

			case 0x49:
			case 0x69:
				curr_board = init_game(fd, &xs, &ys);
				break;

			case 0x51:
			case 0x71:
				sendstr(fd, "Goodbye!\n");
				return 0;

			default:
				sendstr(fd, "Invalid option, please try again N, I, or Q please!\n");
		}
	}
	return 0;
}
