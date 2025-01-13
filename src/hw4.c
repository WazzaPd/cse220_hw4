#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>

#include <arpa/inet.h>
#include <signal.h>

#define PORT1 2201
#define PORT2 2202
#define BUFFER_SIZE 1024

#define SHIP 1
#define HIT_SHIP 2
#define MISS 3

int server_socket1, server_socket2;
int board_width, board_height;
int ***board;

typedef struct {
    int row;
    int col;
    int hit;
} Coord;

typedef struct {
    int set_ship;
    int sunk;
    Coord coord1;
    Coord coord2;
    Coord coord3;
    Coord coord4;
} Ship;

Ship p1[5], p2[5];

void init_ship1(){
    for (int i = 0; i<5; i++){
        p1[i].set_ship = 0;
        p1[i].sunk = 0;
        p1[i].coord1.hit = 0;
        p1[i].coord2.hit = 0;
        p1[i].coord3.hit = 0;
        p1[i].coord4.hit = 0;
    }
}

void init_ship2(){
    for (int i = 0; i<5; i++){
        p2[i].set_ship = 0;
        p2[i].sunk = 0;
        p2[i].coord1.hit = 0;
        p2[i].coord2.hit = 0;
        p2[i].coord3.hit = 0;
        p2[i].coord4.hit = 0;
    }
}

//return # of ships left to players name
int calc_remaining_ships(int player){
    int total = 5;
    for (int i = 0; i<5; i++){
        if(player == 0){
            if(p1[i].sunk) total --;
        } else {
            if(p2[i].sunk) total --;
        }
    }
    return total;
}

void printShip(int player){
    if(player == 0){
        for(int i = 0; i<5; i++){
            printf("SHIP %d, coord 1 row: %d col: %d hit: %d\n", i, p1[i].coord1.row, p1[i].coord1.col, p1[i].coord1.hit);
            printf("SHIP %d, coord 2 row: %d col: %d hit: %d\n", i, p1[i].coord2.row, p1[i].coord2.col, p1[i].coord2.hit);
            printf("SHIP %d, coord 3 row: %d col: %d hit: %d\n", i, p1[i].coord3.row, p1[i].coord3.col, p1[i].coord3.hit);
            printf("SHIP %d, coord 4 row: %d col: %d hit: %d\n", i, p1[i].coord4.row, p1[i].coord4.col, p1[i].coord4.hit);
        }
    } else {
        for(int i = 0; i<5; i++){
            printf("SHIP %d, coord 1 row: %d col: %d hit: %d\n", i, p2[i].coord1.row, p2[i].coord1.col, p2[i].coord1.hit);
            printf("SHIP %d, coord 2 row: %d col: %d hit: %d\n", i, p2[i].coord2.row, p2[i].coord2.col, p2[i].coord2.hit);
            printf("SHIP %d, coord 3 row: %d col: %d hit: %d\n", i, p2[i].coord3.row, p2[i].coord3.col, p2[i].coord3.hit);
            printf("SHIP %d, coord 4 row: %d col: %d hit: %d\n", i, p2[i].coord4.row, p2[i].coord4.col, p2[i].coord4.hit);
        }
    }
}

void update_sunk(Ship * ship){
    //printShip();
    if(ship->coord1.hit){
        if(ship->coord2.hit){
            if(ship->coord3.hit){
                if(ship->coord4.hit){
                    ship->sunk = 1;
                }
            }
        }
    }
}

void update_ship(int player, int row, int col){

    if(row == 0 && col == 1){
        printf("THIS SHOULD MAKE HIT'n");
    }

    for (int i = 0; i<5; i++){
        if(player == 0){
            printf("DID ship %d sink?\n", i, p1[i].sunk);
            if(! p1[i].sunk){
                if(p1[i].coord1.row == row && p1[i].coord1.col == col){
                    p1[i].coord1.hit = 1;
                    update_sunk(&p1[i]);
                    return;
                } 
                else if(p1[i].coord2.row == row && p1[i].coord2.col == col){
                    p1[i].coord2.hit = 1;
                    update_sunk(&p1[i]);
                    return;
                }
                else if(p1[i].coord3.row == row && p1[i].coord3.col == col){
                    p1[i].coord3.hit = 1;
                    update_sunk(&p1[i]);
                    return;
                }
                else if(p1[i].coord4.row == row && p1[i].coord4.col == col){
                    p1[i].coord4.hit = 1;
                    update_sunk(&p1[i]);
                    return;
                }
            }
        } else {
            if(! p2[i].sunk){
                if(p2[i].coord1.row == row && p2[i].coord1.col == col){
                    p2[i].coord1.hit = 1;
                    update_sunk(&p2[i]);
                    return;
                } 
                else if(p2[i].coord2.row == row && p2[i].coord2.col == col){
                    p2[i].coord2.hit = 1;
                    update_sunk(&p2[i]);
                    return;
                }
                else if(p2[i].coord3.row == row && p2[i].coord3.col == col){
                    p2[i].coord3.hit = 1;
                    update_sunk(&p2[i]);
                    return;
                }
                else if(p2[i].coord4.row == row && p2[i].coord4.col == col){
                    p2[i].coord4.hit = 1;
                    update_sunk(&p2[i]);
                    return;
                }
            }
        }
    }
}

void insert_ship(int player, int row1, int col1, int row2, int col2, int row3, int col3, int row4, int col4){
    for(int i = 0; i<5; i++){
        if(player == 0){
            if(p1[i].set_ship == 0){
                p1[i].set_ship = 1;
                p1[i].coord1.row = row1;
                p1[i].coord1.col = col1;
                p1[i].coord2.row = row2;
                p1[i].coord2.col = col2;
                p1[i].coord3.row = row3;
                p1[i].coord3.col = col3;
                p1[i].coord4.row = row4;
                p1[i].coord4.col = col4;
                return;
            }
        } else {
            if(p2[i].set_ship == 0){
                p2[i].set_ship = 1;
                p2[i].coord1.row = row1;
                p2[i].coord1.col = col1;
                p2[i].coord2.row = row2;
                p2[i].coord2.col = col2;
                p2[i].coord3.row = row3;
                p2[i].coord3.col = col3;
                p2[i].coord4.row = row4;
                p2[i].coord4.col = col4;
                return;
            }
        }
    }
}

void printboard(){
    for(int i = 0; i< board_height; i++){
        for(int j = 0; j< board_width; j++){
            printf("%d ", board[0][i][j]);
        }
        printf("\n");
    }
}

void handle_sigint(int sig) {
    printf("\nCaught signal %d, closing server socket...\n", sig);
    close(server_socket1);
    close(server_socket2);
    free(board);
    exit(0);
}

int other_player_turn(int current_turn){
    if(current_turn == 0){
        return 1;
    }
    return 0;
}

// 1 for success, 2 for out of range, 3 for overlap
int place_shape1(int rotation, int row, int col, int player){

    printf("THIS IS SHAPE 1 \n\n");
    printf("This is col: %d\n", col);
    printf("This is row: %d\n", row);
    // row, col out of bounds
    if(row < 0 || col < 0 || row+1 >= board_height || col+1 >= board_width){
        return 2;
    }

    // Overlapping ship
    if(board[player][row][col] || board[player][row][col+1] || board[player][row+1][col] || board[player][row+1][col+1]){
        return 3;
    }
    //Valid
    board[player][row][col] = SHIP;
    board[player][row][col+1] = SHIP;
    board[player][row+1][col] = SHIP;
    board[player][row+1][col+1] = SHIP;

    insert_ship(player, row, col, row, col+1, row+1, col, row+1, col+1);
    return 1;
}
// 1 for success, 2 for out of range, 3 for overlap
int place_shape2(int rotation, int row, int col, int player){
    // rotation 1 and 3
    if(rotation == 1 || rotation == 3){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+3 >= board_height || col >= board_width){
            return 2;
        }

        //Overlapping ship
        if(board[player][row][col] || board[player][row+1][col] || board[player][row+2][col] || board[player][row+3][col]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row+2][col] = SHIP;
        board[player][row+3][col] = SHIP;

        insert_ship(player, row, col, row+1, col, row+2, col, row+3, col);
        return 1;
    }
    // rotation 2 and 4 
    else{
        // row, col out of bounds
        if(row < 0 || col < 0 || row >= board_height || col+3 >= board_width){
            return 2;
        }
        //Overlapping ship
        if(board[player][row][col] || board[player][row][col+1] || board[player][row][col+2] || board[player][row][col+3]) {
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row][col+2] = SHIP;
        board[player][row][col+3] = SHIP;

        insert_ship(player, row, col, row, col+1, row, col+2, row, col+3);
        return 1;
    }
}
// 1 for success, 2 for out of range, 3 for overlap
int place_shape3(int rotation, int row, int col, int player){
    // rotation 1 and 3
    if(rotation == 1 || rotation == 3){
        // row, col out of bounds
        if(row-1 < 0 || col < 0 || row >= board_height || col+2 >= board_width){
            return 2;
        }

        //Overlapping ship
        if(board[player][row][col] || board[player][row][col+1] || board[player][row-1][col+1] || board[player][row-1][col+2]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row-1][col+1] = SHIP;
        board[player][row-1][col+2] = SHIP;
        insert_ship(player, row, col, row, col+1, row-1, col+1, row-1, col+2);
        return 1;
    }
    // rotation 2 and 4 
    else{
        // row, col out of bounds
        if(row < 0 || col < 0 || row+2 >= board_height || col+1 >= board_width){
            return 2;
        }
        //Overlapping ship
        if(board[player][row][col] || board[player][row+1][col] || board[player][row+1][col+1] || board[player][row+2][col+1]){
            return 3;
        } 

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row+1][col+1] = SHIP;
        board[player][row+2][col+1] = SHIP;

        insert_ship(player, row, col, row+1, col, row+1, col+1, row+2, col+1);
        return 1;
    }
}  
// 1 for success, 2 for out of range, 3 for overlap
int place_shape4(int rotation, int row, int col, int player){
    // rotation 1
    if(rotation == 1){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+2 >= board_height || col+1 >= board_width){
            return 2;
        }

        //Overlapping ship
        if( board[player][row][col] || board[player][row+1][col] || board[player][row+2][col] || board[player][row+2][col+1]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row+2][col] = SHIP;
        board[player][row+2][col+1] = SHIP;

        insert_ship(player, row, col, row+1, col, row+2, col, row+2, col+1);
        return 1;
    }
    // rotation 2
    else if( rotation == 2){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+1 >= board_height || col+2 >= board_width){
            return 2;
        }
        //Overlapping ship
        if( board[player][row][col] || board[player][row+1][col] || board[player][row][col+1] || board[player][row][col+2]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row][col+2] = SHIP;

        insert_ship(player, row, col, row+1, col, row, col+1, row, col+2);
        return 1;
    }
    // rotation 3
    else if(rotation == 3){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+2 >= board_height || col+1 >= board_width){
            return 2;
        }

        //Overlapping ship
        if( board[player][row][col] || board[player][row][col+1] || board[player][row+1][col+1] || board[player][row+2][col+1]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row+1][col+1] = SHIP;
        board[player][row+2][col+1] = SHIP;

        insert_ship(player, row, col, row, col+1, row+1, col+1, row+2, col+1);
        return 1;
    }
    // rotation 4 
    else{
        // row, col out of bounds
        if(row-1 < 0 || col < 0 || row >= board_height || col+2 >= board_width){
            return 2;
        }
        //Overlapping ship
        if( board[player][row][col] || board[player][row][col+1] || board[player][row][col+2] || board[player][row-1][col+2]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row][col+2] = SHIP;
        board[player][row-1][col+2] = SHIP;

        insert_ship(player, row, col, row, col+1, row, col+2, row-1, col+2);
        return 1;
    }
}
// 1 for success, 2 for out of range, 3 for overlap
int place_shape5(int rotation, int row, int col, int player){
    // rotation 1 and 3
    if(rotation == 1 || rotation == 3){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+1 >= board_height || col+2 >= board_width){
            return 2;
        }

        //Overlapping ship
        if( board[player][row][col] || board[player][row][col+1] || board[player][row+1][col+1] || board[player][row+1][col+2]) {
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row+1][col+1] = SHIP;
        board[player][row+1][col+2] = SHIP;

        insert_ship(player, row, col, row, col+1, row+1, col+1, row+1, col+2);
        return 1;
    }
    // rotation 2 and 4 
    else{
        // row, col out of bounds
        if(row-1 < 0 || col < 0 || row+1 >= board_height || col+1 >= board_width){
            return 2;
        }

        //Overlapping ship
        if(board[player][row][col] || board[player][row+1][col] || board[player][row-1][col+1] || board[player][row][col+1]) {
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row-1][col+1] = SHIP;
        board[player][row][col+1] = SHIP;

        insert_ship(player, row, col, row+1, col, row-1, col+1, row, col+1);
        return 1;
    }
}
// 1 for success, 2 for out of range, 3 for overlap
int place_shape6(int rotation, int row, int col, int player){
    // rotation 1
    if(rotation == 1){
        // row, col out of bounds
        if(row-2 < 0 || col < 0 || row >= board_height || col+1 >= board_width){
            return 2;
        }

        //Overlapping ship
        if(board[player][row][col] || board[player][row][col+1] || board[player][row-1][col+1] || board[player][row-2][col+1]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row-1][col+1] = SHIP;
        board[player][row-2][col+1] = SHIP;

        insert_ship(player, row, col, row, col+1, row-1, col+1, row-2, col+1);
        return 1;
    }
    // rotation 2
    else if( rotation == 2){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+1 >= board_height || col+2 >= board_width){
            return 2;
        }
        //Overlapping ship
        if( board[player][row][col] || board[player][row+1][col] || board[player][row+1][col+1] || board[player][row+1][col+2]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row+1][col+1] = SHIP;
        board[player][row+1][col+2] = SHIP;

        insert_ship(player, row, col, row+1, col, row+1, col+1, row+1, col+2);
        return 1;
    }
    // rotation 3
    else if(rotation == 3){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+2 >= board_height || col+1 >= board_width){
            return 2;
        }

        //Overlapping ship
        if( board[player][row][col] || board[player][row][col+1] || board[player][row+1][col] || board[player][row+2][col] ){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row+2][col] = SHIP;

        insert_ship(player, row, col, row, col+1, row+1, col, row+2, col);
        return 1;
    }
    // rotation 4 
    else{
        // row, col out of bounds
        if(row < 0 || col < 0 || row+1 >= board_height || col+2 >= board_width){
            return 2;
        }
        //Overlapping ship
        if( board[player][row][col] || board[player][row][col+1] || board[player][row][col+2] || board[player][row+1][col+2]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row][col+2] = SHIP;
        board[player][row+1][col+2] = SHIP;

        insert_ship(player, row, col, row, col+1, row, col+2, row+1, col+2);
        return 1;
    }
}
// 1 for success, 2 for out of range, 3 for overlap
int place_shape7(int rotation, int row, int col, int player){
    // rotation 1
    if(rotation == 1){
        // row, col out of bounds
        if(row < 0 || col < 0 || row+1 >= board_height || col+2 >= board_width){
            return 2;
        }

        //Overlapping ship
        if(board[player][row][col] || board[player][row][col+1] || board[player][row+1][col+1] || board[player][row][col+2]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row+1][col+1] = SHIP;
        board[player][row][col+2] = SHIP;

        insert_ship(player, row, col, row, col+1, row+1, col+1, row, col+2);
        return 1;
    }
    // rotation 2
    else if( rotation == 2){
        // row, col out of bounds
        if(row-1 < 0 || col < 0 || row+1 >= board_height || col+1 >= board_width){
            return 2;
        }
        //Overlapping ship
        if( board[player][row][col] || board[player][row-1][col+1] || board[player][row][col+1] || board[player][row+1][col+1]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row-1][col+1] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row+1][col+1] = SHIP;

        insert_ship(player, row, col, row-1, col+1, row, col+1, row+1, col+1);
        return 1;
    }
    // rotation 3
    else if(rotation == 3){
        // row, col out of bounds
        if(row-1 < 0 || col < 0 || row >= board_height || col+2 >= board_width){
            return 2;
        }

        //Overlapping ship
        if( board[player][row][col] || board[player][row][col+1] || board[player][row-1][col+1] || board[player][row][col+2] ){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row][col+1] = SHIP;
        board[player][row-1][col+1] = SHIP;
        board[player][row][col+2] = SHIP;

        insert_ship(player, row, col, row, col+1, row-1, col+1, row, col+2);
        return 1;
    }
    // rotation 4 
    else{
        // row, col out of bounds
        if(row < 0 || col < 0 || row+2 >= board_height || col+1 >= board_width){
            return 2;
        }
        //Overlapping ship
        if( board[player][row][col] || board[player][row+1][col] || board[player][row+1][col+1] || board[player][row+2][col+0]){
            return 3;
        }

        //Valid
        board[player][row][col] = SHIP;
        board[player][row+1][col] = SHIP;
        board[player][row+1][col+1] = SHIP;
        board[player][row+2][col] = SHIP;

        insert_ship(player, row, col, row+1, col, row+1, col+1, row+2, col);
        return 1;
    }
}

// 1 for success, 2 for out of range, 3 for overlap
int handle_ship_placement(int type, int rotation, int row, int col, int player_turn){
    switch (type) {
        case 1:
            return place_shape1(rotation, row, col, player_turn);
            break;
        case 2:
            return place_shape2(rotation, row, col, player_turn);
            break;
        case 3:
            return place_shape3(rotation, row, col, player_turn);
            break;
        case 4:
            return place_shape4(rotation, row, col, player_turn);
            break;
        case 5:
            return place_shape5(rotation, row, col, player_turn);
            break;
        case 6:
            return place_shape6(rotation, row, col, player_turn);
            break;
        case 7:
            return place_shape7(rotation, row, col, player_turn);
            break;
    }
}

// Will return a message reponse
char * handleInitialize(char * initializationMessage, int player_turn){
    //ps reffers to peice rotation
    int pt[5], ps[5], pc[5], pr[5], extra;

    char init_char;
    char *message;

    if (sscanf(initializationMessage, "%c %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
    &init_char, &pt[0], &ps[0], &pc[0], &pr[0], &pt[1], &ps[1], &pc[1], &pr[1], &pt[2], &ps[2], &pc[2], &pr[2], &pt[3], &ps[3], &pc[3], &pr[3], &pt[4], 
    &ps[4], &pc[4], &pr[4], &extra) != 21){
        message = malloc(6* sizeof(char));
        strcpy(message, "E 201");
        return message;
    }

    //Check for shape error 
    for(int i = 0; i<5; i++){
        if(pt[i] < 1 || pt[i] > 7){
            message = malloc(6* sizeof(char));
            strcpy(message, "E 300");
            return message;
        }
    }
    //check for rotation error
    for(int i = 0; i<5; i++){
        if(ps[i] < 1 || ps[i] > 4){
            message = malloc(6* sizeof(char));
            strcpy(message, "E 301");
            return message;
        }
    }

    int placement_status[5] = {0};

    for(int i = 0; i<5; i++){
        // place ship on board
        printf("THIS IS i: %d", i);
        placement_status[i] = handle_ship_placement(pt[i], ps[i], pr[i], pc[i], player_turn);
    }
    for(int i = 0; i< 5; i++){
        if(placement_status[i] == 2){
            message = malloc(6* sizeof(char));
            strcpy(message, "E 302");
            return message;
        }
    }
    for(int i = 0; i< 5; i++){
        if(placement_status[i] == 3){
            message = malloc(6* sizeof(char));
            strcpy(message, "E 303");
            return message;
        }
    }

    message = malloc(2* sizeof(char));
    strcpy(message, "A");
    return message;
}

//Will return message
char * handleShoot(char * shootMessage, int player_turn){
    
    int row, col, extra;
    char init_char;
    char *message;

    // Invalid parameters error
    if(sscanf(shootMessage, "%c %d %d %d", &init_char, &row, &col, &extra) != 3){
        message = malloc(6* sizeof(char));
        strcpy(message, "E 202");
        return message;
    }

    // invalid shot error Out of bounds
    if(row < 0 || row >= board_height || col < 0 || col >= board_width){
        message = malloc(6* sizeof(char));
        strcpy(message, "E 400");
        return message;
    }

    int current_tile_status = board[other_player_turn(player_turn)][row][col];

    //invalid shot error already guessed
    if(current_tile_status == HIT_SHIP || current_tile_status == MISS){
        message = malloc(6* sizeof(char));
        strcpy(message, "E 401");
        return message;
    }

    //Hit or MISS
    if(current_tile_status == SHIP){
        board[other_player_turn(player_turn)][row][col] = HIT_SHIP;

        update_ship(other_player_turn(player_turn), row, col);
        message = malloc(6* sizeof(char));

        int remaining = calc_remaining_ships(other_player_turn(player_turn));
        sprintf(message, "R %d H", remaining);
        return message;
    }
    if(current_tile_status == 0){
        board[other_player_turn(player_turn)][row][col] = MISS;
        message = malloc(6* sizeof(char));
        int remaining = calc_remaining_ships(other_player_turn(player_turn));
        sprintf(message, "R %d M", remaining);
        return message;
    }
}

//Will return a message for a hits and misses on players board
char * allHitsandMisses(int player){
    char message[1000] = "";
    for(int i = 0; i<board_height; i++){
        for(int j = 0; j<board_width; j++){
            if(board[player][i][j] == HIT_SHIP) sprintf(message + strlen(message), " H %d %d", i, j);
            if(board[player][i][j] == MISS) sprintf(message + strlen(message), " M %d %d", i, j);
        }
    }

    char * mallocedMessage = malloc((strlen(message)+1) * sizeof(char));
    strcpy(mallocedMessage, message);
    return mallocedMessage;
}

//Will return message
char * handleQuery(int player_turn){
    char * message;
    int remaining = calc_remaining_ships(other_player_turn(player_turn));

    char * result = allHitsandMisses(other_player_turn(player_turn));

    printf("this is result: %s\n", result);

    message = malloc((strlen(result)+5) * sizeof(char));
    sprintf(message, "G %d%s", remaining, result);
    
    printf("this is size of result: %d\n", strlen(result));
    printf("this is message: %s\n", message);
    printf("this is size of message %d\n", strlen(message));

    free(result);
    return message;
}

int main (){

    init_ship1();
    init_ship2();

    char server_message[256] = "You have reached the server\n";

    signal(SIGINT, handle_sigint);
    
    // create the server socket
    server_socket1 = socket(AF_INET, SOCK_STREAM, 0);
    server_socket2 = socket(AF_INET, SOCK_STREAM, 0);

    //define the server address for player 1
    struct sockaddr_in server_address1;
    server_address1.sin_family = AF_INET;
    server_address1.sin_port = htons(PORT1);
    server_address1.sin_addr.s_addr = INADDR_ANY;

    //define the server address for player 2
    struct sockaddr_in server_address2;
    server_address2.sin_family = AF_INET;
    server_address2.sin_port = htons(PORT2);
    server_address2.sin_addr.s_addr = INADDR_ANY;

    //bind the sockets to a specified IP and port
    int opt = 1;
    setsockopt(server_socket1, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(server_socket2, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(server_socket1, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    setsockopt(server_socket2, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    
    bind(server_socket1, (struct sockaddr*) &server_address1, sizeof(server_address1));
    bind(server_socket2, (struct sockaddr*) &server_address2, sizeof(server_address2));

    listen(server_socket1, 2);
    listen(server_socket2, 2);

    int player1, player2;
    char buffer[BUFFER_SIZE] = {0};

    int board_set = 0;
    int initialize_set = 0;
    
    player1 = accept(server_socket1, NULL, NULL);
    if(player1 < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    player2 = accept(server_socket2, NULL, NULL);
    if(player2 < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("accepted both\n");
    int player[2] = {player1, player2};
    int player_turn = 0;


    while(1){
        memset(buffer, 0, BUFFER_SIZE);

        //Get Player message
        printf("reading message from player %d\n", player_turn+1);
        int nbytes_from_player = read(player[player_turn], buffer, BUFFER_SIZE);
        if (nbytes_from_player <= 0)
        {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("read message from p%d: %s\n", player_turn+1, buffer);

        //Get packet type
        char action = ' ';
        int action_was_valid = 0;
        sscanf(buffer, "%c ", &action);

        //Interpret Player message

        //Handle Forfeit
        if(action == 'F'){
            send(player[player_turn], "H 0", 3, 0);
            send(player[other_player_turn(player_turn)], "H 1", 4, 0);

            break;
        }

        //Handle Begin Packet
        else if((! board_set) && action != 'B') send(player[player_turn], "E 100", 6, 0);
        else if ((! board_set) && action == 'B'){

            //Player 1
            if(player_turn == 0){
                int width, height, extra;

                if(sscanf(buffer, "%c %d %d %d", &action, &width, &height, &extra) != 3) send(player[player_turn], "E 200", 6, 0);
                else {
                    printf("width: %d, height %d", width, height);

                    if(width < 10 || height < 10){
                        send(player[player_turn], "E 200", 6, 0);
                    } 
                    // Successful Begin packet
                    else {
                        board_width = width;
                        board_height = height;
                        board = malloc(2 * sizeof(int**));
                        board[0] = malloc((height * sizeof(int*)));
                        board[1] = malloc((height * sizeof(int*)));
                        for (int i = 0; i < height; i++) {
                            board[0][i] = malloc(width * sizeof(int));
                            memset(board[0][i], 0, width * sizeof(int));
                        }
                        //printf("up to here\n");
                        for (int i = 0; i < height; i++) {
                            board[1][i] = malloc(width * sizeof(int));
                            memset(board[1][i], 0, width * sizeof(int));
                        }
                        send(player[player_turn], "A", 2, 0);
                        action_was_valid = 1;
                    }
                }

            }
            //Player 2
            else {
                int extra;
                if(sscanf(buffer, "%c %d", &action, &extra) != 1) send(player[player_turn], "E 200", 6, 0);
                else {
                    send(player[player_turn], "A", 2, 0);
                    board_set = 1;
                    action_was_valid = 1;
                }
            }
        }

        //handle Initialize
        else if((! initialize_set) && action != 'I') send(player[player_turn], "E 101", 6, 0);
        else if((! initialize_set) && action == 'I'){
            char * message = handleInitialize(buffer, player_turn);
            send(player[player_turn], message, sizeof(message), 0);
            if(message[0] == 'A'){
                action_was_valid = 1;
                
                // Player 2 successful initialization
                if(player_turn == 1){
                    initialize_set = 1;
                }
            }
            // error message obtained 
            else {
                printboard();
                // restart board
                if(player_turn == 0){
                    init_ship1();
                    for (int i = 0; i < board_height; i++) {
                        memset(board[0][i], 0, board_width * sizeof(int));
                    }
                } else {
                    init_ship2();
                    for (int i = 0; i < board_height; i++) {
                        memset(board[1][i], 0, board_width * sizeof(int));
                    }
                }
            }
            free(message);
        } 
        
        //handle shoot packet
        else if(action == 'S'){
            char * message = handleShoot(buffer, player_turn);
            send(player[player_turn], message, strlen(message), 0);
            if(message[0] == 'R'){
                printf("This is message: %s\n", message);
                action_was_valid = 1;

                // GAME OVER
                if(message[2] == '0'){
                    send(player[other_player_turn(player_turn)], "H 0", 4, 0);
                    send(player[player_turn], "H 1", 4, 0);
                }
            }
            free(message);
        } 
        
        // handle query packet
        else if(action == 'Q'){
            char * message = handleQuery(player_turn);
            send(player[player_turn], message, strlen(message), 0);
            free(message);
        }

        else {
            printf("not happening");
            send(player[player_turn], "E 102", 6, 0);
        }

        //Next players turn
        if(action_was_valid){
            player_turn = other_player_turn(player_turn);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < board_height; j++) {
            free(board[i][j]);  // Free each row
        }
        free(board[i]);  // Free each layer
    }
    free(board);  // Free the main array pointer
    close(server_socket1);
    close(server_socket2);

    return 0;
}