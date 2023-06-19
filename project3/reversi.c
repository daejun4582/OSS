#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <ncurses.h>

#include <arpa/inet.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <linux/socket.h>

#define BOARD_SIZE 8
#define BUFFER_SIZE 256


//------------------socket----------------------

//send first - client
int  sending(int argc, char const ** argv);
int  connect_ipaddr_port (const char * ip, int port);
void chat_send(int conn_fd);

//recv - server
int recving(int argc, char const ** argv);
int listen_at_port (int portnum) ;
void chat_recv(int conn_fd);

//----------------------------------------------

void drawBoard(char board[BOARD_SIZE][BOARD_SIZE]); 
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
int isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char piece); 
void flipPieces(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char piece); 
int countPieces(char board[BOARD_SIZE][BOARD_SIZE], char piece);
int isGameOver(char board[BOARD_SIZE][BOARD_SIZE]); 
int determineWinner(char board[BOARD_SIZE][BOARD_SIZE]); 
void placePiece(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char piece); 

int cmdStarty;
int cmdStartx;
int currentPlayer;
char stones[] = {'X','O'};
// char board[BOARD_SIZE][BOARD_SIZE];

int main(int argc, const char *argv[]) {
    currentPlayer = 1;
    int row, col;

    cmdStarty = LINES / 2;
    cmdStartx = (COLS - 18) / 2;

    // Initialize ncurses
    initscr();
    curs_set(0);
    // raw(); // Enable raw mode to process input character by character

    // // Set up the initial game state
    // initializeBoard(board);

    // Check command-line arguments for server/client mode
    if (argc > 1 && strcmp(argv[1], "-server") == 0) {
        recving(argc,argv);

    } else if (argc > 1 && strcmp(argv[1], "-client") == 0) {
        sending(argc,argv);
    } else {
        // Invalid command-line arguments
        printw("Usage: %s -server <port> or %s -client <ip> <port>\n", argv[0], argv[0]);
        refresh();
        return 1;
    }

    // End ncurses
    endwin();

    return EXIT_SUCCESS;
}

void drawBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    clear();

    int boardWidth = BOARD_SIZE * 4 + 1;
    int boardHeight = BOARD_SIZE * 2 + 1;

    int startX = (COLS - boardWidth) / 2; // Calculate the start position for the board
    int startY = (LINES - boardHeight) / 2; 

    // Draw the column labels
    mvprintw(startY, startX + 2, "    0   1   2   3   4   5   6   7");

    // Draw the horizontal lines and row labels
    for (int row = 0; row < BOARD_SIZE; row++) {
        mvprintw(startY + row * 2 + 1, startX, "%d", row);
        if (row != BOARD_SIZE - 1) {
            mvprintw(startY + row * 2 + 2, startX + 4, "+---+---+---+---+---+---+---+---+");
        }
    }

    // Draw the pieces
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col <= BOARD_SIZE; col++) {
            int x = startX + col * 4 + 4;
            int y = startY + row * 2 + 1;

            if(col != BOARD_SIZE)
                mvprintw(y, x, "| %c ", board[row][col]);
            else
                mvprintw(y, x, "|");
        }
        
    }

    refresh();
}


void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = ' ';
        }
    }
    board[3][3] = board[4][4] = 'O';
    board[3][4] = board[4][3] = 'X';
}

int isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char piece) {
    if (board[row][col] != ' ') {
        return 0; // The position is already occupied
    }

    // Check in all directions
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int newRow = row + dx;
        int newCol = col + dy;

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] != piece && board[newRow][newCol] != ' ') {
            while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                newRow += dx;
                newCol += dy;
                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] == piece) {
                    return 1; // There is a valid move in this direction
                }
                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] == ' ') {
                    break; // Reached an empty position
                }
            }
        }
    }

    return 0; // No valid move found
}

void flipPieces(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char piece) {
    // Check in all directions
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int newRow = row + dx;
        int newCol = col + dy;

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] != piece && board[newRow][newCol] != ' ') {
            while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                newRow += dx;
                newCol += dy;
                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] == piece) {
                    // Flip the pieces between (row, col) and (newRow, newCol)
                    int flipRow = row + dx;
                    int flipCol = col + dy;
                    while (flipRow != newRow || flipCol != newCol) {
                        board[flipRow][flipCol] = piece;
                        flipRow += dx;
                        flipCol += dy;
                    }
                    break;
                }
                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] == ' ') {
                    break; // Reached an empty position
                }
            }
        }
    }
}

int countPieces(char board[BOARD_SIZE][BOARD_SIZE], char piece) {
    int count = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == piece) {
                count++;
            }
        }
    }
    return count;
}

int isGameOver(char board[BOARD_SIZE][BOARD_SIZE]) {
    int hasValidMovePlayer1 = 0;
    int hasValidMovePlayer2 = 0;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == ' ') {
                // Check if there is a valid move for Player 1 (X)
                if (isValidMove(board, row, col, 'X')) {
                    hasValidMovePlayer1 = 1;
                }
                // Check if there is a valid move for Player 2 (O)
                if (isValidMove(board, row, col, 'O')) {
                    hasValidMovePlayer2 = 1;
                }
            }
        }
    }

    // The game is over if either player has no valid moves
    return !(hasValidMovePlayer1 || hasValidMovePlayer2);
}

int determineWinner(char board[BOARD_SIZE][BOARD_SIZE]) {
    int xCount = countPieces(board, 'X');
    int oCount = countPieces(board, 'O');

    if (xCount > oCount) {
        return 1; // Player 1 (X) wins
    } else if (xCount < oCount) {
        return 2; // Player 2 (O) wins
    } else {
        return 0; // It's a draw
    }
}

void placePiece(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char piece) {
    board[row][col] = piece;
}

int sending(int argc, char const ** argv)
{
    struct sockaddr_in serv_addr; 
	int sock_fd ;
	int s, len ;
	char buffer[1024] = {0}; 
	char * data ;

	if (argc != 4) {
		fprintf(stderr, "Wrong number of arguments\n") ;
		fprintf(stderr, "Usage: ./chat-sendfirst [IP addr] [Port num]\n") ;
		exit(EXIT_FAILURE) ;
	}

	int conn_fd = connect_ipaddr_port(argv[2], atoi(argv[3])) ;
	
	chat_send(conn_fd) ;

	shutdown(conn_fd, SHUT_RDWR) ;

	return 0;
}

int connect_ipaddr_port (const char * ip, int port)
{
	int sock_fd ;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0) ;
	if (sock_fd <= 0) {
		perror("socket failed : ") ;
		exit(EXIT_FAILURE) ;
	}
	int opt = 2 ;
	if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
		fprintf(stderr, "fail at setsockopt\n") ;
		exit(EXIT_FAILURE) ;
	}

	struct sockaddr_in address ;
	bzero(&address, sizeof(address)) ;
	address.sin_family = AF_INET ; 
	address.sin_port = htons(port) ; 
	if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
		perror("inet_pton failed : ") ; 
		exit(EXIT_FAILURE) ;
	} 

	if (connect(sock_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("connect failed : ") ;
		exit(EXIT_FAILURE) ;
	}
	return sock_fd ;
}

void chat_send(int conn_fd)
{
    char buf[256] ;
    char board[BOARD_SIZE][BOARD_SIZE];
    int row = -1,col = -1;
    initializeBoard(board);

	do {
        drawBoard(board);
        refresh();

        getch();

        mvprintw(cmdStarty - 11, cmdStartx + 4, "[Player %d's turn '%c' ]", currentPlayer,stones[currentPlayer-1]);
        mvprintw(cmdStarty + 9, cmdStartx +2, "Enter row and column: ");
        refresh();


        scanw("%d %d", &row, &col);

        printw("%d %d\n",row, col);

        getch();

		while(true)
        {
            if (!(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || !isValidMove(board, row, col, stones[currentPlayer - 1])))
                break;
            // fgets(buf, 256, stdin) ;
            mvprintw(cmdStarty + 9, cmdStartx +3, "InValid Location. Try Again.\n");
            refresh();
            scanw("%d %d", &row, &col);
            refresh();
            sprintf(buf,"%d %d",row,col);
		    buf[strlen(buf) - 1] = '\0' ;
        }

        getch();

		send(conn_fd, buf, strlen(buf), 0) ;

		int s ;
		while ((s = recv(conn_fd, buf, 1024, 0)) == 0) ;
		if (s == -1)
			break ;
		buf[s] = '\0' ;

        sscanf(buf, "%d %d", &row, &col);

        // Place the client's piece on the board
        placePiece(board, row, col, stones[currentPlayer - 1]);

        // Game over, determine the winner
        drawBoard(board);
        refresh();

        mvprintw(cmdStarty - 11, cmdStartx + 4, "[Player %d's turn '%c' ]", currentPlayer,stones[currentPlayer-1]);
        mvprintw(cmdStarty + 9, cmdStartx +2, "Enter row and column: ");

		
	} while (!isGameOver(board)) ;

    int winner = determineWinner(board);
        // Display the winner or a draw message
        if (winner == 0) {
            printw("It's a draw!\n");
        } else {
            printw("Player %d wins!\n", winner);
        }
        refresh();
    
    return;
}

int recving(int argc, char const ** argv)
{
    if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments") ;
		exit(EXIT_FAILURE) ;
	}

	int conn_fd = listen_at_port(atoi(argv[2])) ;

	chat_recv(conn_fd) ;

	shutdown(conn_fd, SHUT_RDWR) ;

    return 0;
}

int listen_at_port (int portnum) 
{
	int sock_fd = socket(AF_INET /*IPv4*/, SOCK_STREAM /*TCP*/, 0 /*IP*/) ;
	if (sock_fd == 0)  { 
		perror("socket failed : "); 
		exit(EXIT_FAILURE); 
	}
	int opt = 2 ;
	if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
		fprintf(stderr, "fail at setsockopt\n") ;
		exit(EXIT_FAILURE) ;
	}

	struct sockaddr_in address ; 
	bzero(&address, sizeof(address)) ; 	
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY /* localhost */ ; 
	address.sin_port = htons(portnum); 

	if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed: "); 
		exit(EXIT_FAILURE); 
	} 

	if (listen(sock_fd, 16 /* the size of waiting queue*/) < 0) { 
		perror("listen failed : "); 
		exit(EXIT_FAILURE); 
	} 

	int addrlen = sizeof(address); 
	int conn_fd = accept(sock_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen) ;
	if (conn_fd < 0) {
		perror("accept failed: "); 
		exit(EXIT_FAILURE); 
	}
	return conn_fd ;
}

void chat_recv(int conn_fd)
{
    char buf[256] ;
    int row,col;

    char board[BOARD_SIZE][BOARD_SIZE];

    initializeBoard(board);

	do {
		int s ;

        drawBoard(board);
        refresh();

        
		
		while ( (s = recv(conn_fd, buf, 255, 0)) == 0 ) ;
		if (s == -1)
			break ;
        
        buf[s] = '\0' ;
		// printf(">%s\n", buf) ;

        // Parse the move
        sscanf(buf, "%d %d", &row, &col);

        // Place the client's piece on the board
        placePiece(board, row, col, stones[currentPlayer - 1]);

        // Game over, determine the winner
        drawBoard(board);
        refresh();

        mvprintw(cmdStarty - 11, cmdStartx + 4, "[Player %d's turn '%c' ]", currentPlayer,stones[currentPlayer-1]);
        mvprintw(cmdStarty + 9, cmdStartx +2, "Enter row and column: ");


		while(true)
        {
            if (!(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || !isValidMove(board, row, col, stones[currentPlayer - 1])))
                break;
            mvprintw(cmdStarty + 9, cmdStartx +3, "InValid Location. Try Again.\n");
            // fgets(buf, 256, stdin) ;
            scanw("%d %d", &row, &col);
            refresh();
            sprintf(buf,"%d %d",row,col);
		    buf[strlen(buf) - 1] = '\0' ;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1;
		
		send(conn_fd, buf, strlen(buf), 0) ;

	} while (!isGameOver(board)) ;

    int winner = determineWinner(board);
        // Display the winner or a draw message
        if (winner == 0) {
            printw("It's a draw!\n");
        } else {
            printw("Player %d wins!\n", winner);
        }
        refresh();
    
    return;
}



