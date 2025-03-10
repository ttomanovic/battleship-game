#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 5
#define SHIP_COUNT 5

typedef struct {
	int row;
	int col;
} Ship;

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
		scanf("%d %d", &ships[i].row, &ships[i].col);
	}

 modify_table(my_table, GRID_SIZE, ships, SHIP_COUNT);
 print_table(my_table, GRID_SIZE);


// sad treba da se poveze sa drugim igracom i zapocne se igra
// igra 

	return 0;
}
