#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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

static int my_table[GRID_SIZE][GRID_SIZE] = {0};

void modify_table(int t[][GRID_SIZE], int table_size, Ship ships[], int ship_count){
	for(int i=0; i<ship_count; i++)
		t[ships[i].row][ships[i].col]=1;
}

void print_table(int t[][GRID_SIZE], int size){
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++)
			fprintf(stdout, "%d ",t[i][j]);
		fprintf(stdout, "\n");
	}
}

int main(){

	printf("Unesi pozicije brodova: \n");

	Ship ships[SHIP_COUNT];
	for(int i=0; i<SHIP_COUNT; i++){
		// ubaci proveru da li su pravilno uneseni brodovi,
		//ne moze brod da bude na koordinati 6 7 lupam
		scanf("%d %d", &ships[i].col, &ships[i].row);
	}

 modify_table(my_table, GRID_SIZE, ships, SHIP_COUNT);
 print_table(my_table, GRID_SIZE);

 int res = mkfifo("p2_writes",0600);
 check_error(res!=-1, "mkfifo failed");
// ubaci proveru neki check error za ovaj res, da li je fifo fajl uspesno kreiran

int fd_attack = open("p2_writes", O_WRONLY);
check_error(fd_attack!=-1, "open p2_writes failed");
int fd_respond = open("p1_writes", O_RDONLY);
check_error(fd_respond!=-1, "open p1_writes failed");


// sada mi treba tabela u koju ću da zapisujem da li sam pogodila brod ili ne
 // neka je pogodak 1, promašaj -1, i 0 da nisam pokušavala

 int opponents_table[GRID_SIZE][GRID_SIZE] = {0};

 // napad izgleda ovako: do{
 //   salji koordinate u fajl
 //   primi odgovor i upisi -1 ili 1 na odgovarajucu koordinat
 //   ispisi trenutni opponents table
 //}while(pogodjen && pogodjenih<5)

 // smenjuju se napadi sve dok jedan ne pobedi

 // napada se sve dok se ne pogode svi brodove, tako da mogu da
 // cuvam broj brodova koje sam pogodila


// sad treba da se poveze sa drugim igracom i zapocne se igra
// igra

	return 0;
}
