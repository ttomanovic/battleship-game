#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <unistd.h>

#define GRID_SIZE 5
#define SHIP_COUNT 5

typedef struct {
	int row;
	int col;
} Ship;


#define check_error(cond,msg)\
  do {\
    if (!(cond)) {\
      perror(msg);\
      fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
      exit(EXIT_FAILURE);\
    }\
  } while (0)


void modify_table(int t[][GRID_SIZE], int table_size, Ship ships[], int ship_count){
	for(int i=0; i<ship_count; i++)
		t[ships[i].row][ships[i].col]=1;
}

void print_table(int t[][GRID_SIZE]){
	for(int i=0;i<GRID_SIZE;i++){
		for(int j=0;j<GRID_SIZE;j++)
			fprintf(stdout, "%d ",t[i][j]);
		fprintf(stdout, "\n");
	}
	printf("Successfully printed the table!\n");
}

// send_attack bi trebalo da ocita response i ako kaze hit, onda vrati 1, else -1
int send_attack(int fd1, int fd2, int x, int y) {
	char buf[] = {x,y};

	int size = write(fd1, buf, 2);
	check_error(size==2, "failed to write coordinates to a fifo\n");
	char response[10] = {0};
	int read_size = read(fd2, response, 10);
	check_error(read_size>0, "Failed to read the response from fifo\n");
	response[read_size] = '\0';
	if(strcmp(response,"hit")==0)
		return 1;
	else
		return -1;
}

// attack vraca result -1 ako je promasen i 1 ako je pogođen
int attack(int fd_attack, int fd_respond, int opponents_table[][GRID_SIZE], int *ships_sunk) {
    printf("Insert attack coordinates (row col): ");
    int x, y;
    scanf("%d %d", &x, &y);

    int result = send_attack(fd_attack, fd_respond, x, y);

    if (result == -1) {
        opponents_table[x][y] = -1;
printf("Waiting for player 1 to start... \n");
int fd_respond = open("p1_writes", O_RDONLY);
check_error(fd_respond!=-1, "open p1_writes failed");

        printf("Miss!\n");
    } else {
        opponents_table[x][y] = 1;
        printf("Hit!\n");
        (*ships_sunk)++;
    }

    print_table(opponents_table);
    return result;
}


int opponent_attacks(int fd_attack, int fd_respond, int my_table[][GRID_SIZE], int *my_ships_sunk) {
    char buf[2] = {0};
    int read_size = read(fd_respond, buf, 2);
	printf("%d\n",read_size);
    check_error(read_size == 2, "failed to read opponent attack");

    int x = buf[0], y = buf[1];
	int result = 0;

    char response[10];
    if (my_table[x][y] == 1) {
        (*my_ships_sunk)++;
        strcpy(response, "hit");
		result = 1;
    } else {
        strcpy(response, "miss");
		result = -1;
    }

    int write_size = write(fd_attack, response, strlen(response) + 1);
    check_error(write_size > 0, "failed to write response to FIFO");

    return result;
}

//neka ovaj igrac zapocinje igru

int main(){

	Ship ships[SHIP_COUNT]={{0,0}, {1,3}, {3,3}, {2,4}, {2,2}};

	/* printf("Unesi pozicije brodova: \n");

	for(int i=0; i<SHIP_COUNT; i++){
		// ubaci proveru da li su pravilno uneseni brodovi,
		// ne moze brod da bude na koordinati 6 7 lupam
		scanf("%d %d", &ships[i].col, &ships[i].row);
	}*/
 int my_table[GRID_SIZE][GRID_SIZE] = {0};
 int opponents_table[GRID_SIZE][GRID_SIZE] = {0};
 // neka je pogodak 1, promašaj -1, i 0 da nisam pokušavala


 modify_table(my_table, GRID_SIZE, ships, SHIP_COUNT);
 print_table(my_table);

 int res = mkfifo("p1_writes",0600);
 printf("after mkfifo\n");
 check_error(res!=-1, "mkfifo failed");
// ubaci proveru neki check error za ovaj res, da li je fifo fajl uspesno kreiran

printf("does it block at fd_attack?");
int fd_attack = open("p1_writes", O_WRONLY);
printf("does it block at respond?");
check_error(fd_attack!=-1, "open p1_writes failed");
int fd_respond = open("p2_writes", O_RDONLY);
check_error(fd_respond!=-1, "open p2_writes failed");


 // napad izgleda ovako: do{
 //   salji koordinate u fajl p1_writes
 //   primi odgovor iz p2_writes i upisi -1 ili 1 na odgovarajucu koordinat
 //   ispisi trenutni opponents table
 //}while(pogodjen && pogodjenih<5)
//int turn = 1;
int ships_sunk = 0;
int my_ships_sunk = 0;


//igra se igra dok makar jedan ne pogodi sve brodove
while (ships_sunk < SHIP_COUNT && my_ships_sunk < SHIP_COUNT){
	int result;
	        do {
	            result = attack(fd_attack, fd_respond, opponents_table, &ships_sunk);
	        } while (result == 1 && ships_sunk < SHIP_COUNT);
			 
			//proveravamo da li smo iz petlje izasli jer su se potopili svi brodovi
	        if (ships_sunk >= SHIP_COUNT) {
	            printf("You win!\n");
	            break;
	        }

	        // Player 2 attacks until a miss
	        int opponent_result;
	        do {
	            opponent_result = opponent_attacks(fd_attack, fd_respond, my_table, &my_ships_sunk);
	        } while (opponent_result == 1 && my_ships_sunk < SHIP_COUNT);

	        if (my_ships_sunk >= SHIP_COUNT) {
	            printf("You lose!\n");
	            break;
	        }
}


    close(fd_attack);
    close(fd_respond);
    unlink("p1_writes");

	return 0;
}
