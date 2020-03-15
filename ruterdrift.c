#include "shared.h"

struct router{
  unsigned int id;
  unsigned char flag;
  unsigned char mLenght;
  char model[248];
  int *n; // Naboer
  int nCounter; // Antall naboer
};

struct router** routerPointer;

int main(int argc, char const *argv[]) {

  FILE *myFile = fopen(argv[1], "rb");
  if (myFile == NULL){
    perror("Nej");
    return EXIT_FAILURE;
  }
  int routers;
  fread(&routers, sizeof(int), 1, myFile);
  printf("Number of routers: %d\n", routers);

  routerPointer = malloc((sizeof(struct router*)) * routers); //8 * routers byte

  for (int i = 0; i < routers; i++){
    read(myFile, i, routerPointer);
  }


  // Tester innlesing av øvrig infoblokker
  read_connections(myFile);

  fclose(myFile);

  // Start reading argv[2]

  myFile = fopen(argv[2], "r");
  if (myFile == NULL){
    perror("Nej");
    return EXIT_FAILURE;
  }
  printf("File: %s\n", argv[2]);

  //Print, set flag, set model, add connection, delete, existing

  fclose(myFile);
}

// Funker nå som den skal, liten utfordring med print av modell
void read(FILE *myFile, int n, struct router** routerPointer){
  // Hjelpepekere
  unsigned int id;
  unsigned char flag;
  unsigned char mLenght;
  unsigned int lastBit = 1;

  fread(&id, sizeof(int), 1, myFile);
  fread(&flag, sizeof(char), 1, myFile);
  fread(&mLenght, sizeof(char), 1, myFile);

  char model[mLenght];
  fread(model, mLenght, 1, myFile);
  fread(&lastBit, sizeof(char), 1, myFile);

  struct router* rp = malloc(sizeof(struct router)); //256 (er det pluss peker-størrelse?)

  rp -> id = id;
  rp -> flag = flag;
  rp -> mLenght = mLenght;
  memcpy(rp -> model, model, mLenght);

  for (int i = 0; i < mLenght; i++){
    printf("%c", rp->model[i]);
  }
  printf("\n");

  printf("ID: %d\n", id);
  printf("Flag: %d\n", flag);
  printf("Lengde: %d\n", mLenght);
  printf("\n");
}

void read_connections(FILE* myFile){
  unsigned int r1;
  unsigned int r2;
  unsigned int nullByte;

  long start = ftell(myFile);
  fseek(myFile, 0, SEEK_END);
  long end = ftell(myFile);
  fseek(myFile, start, SEEK_SET);

  while(ftell(myFile) != end){
    fread(&r1, sizeof(int), 1, myFile);
    fread(&r2, sizeof(int), 1, myFile);
    fread(&nullByte, 1, 1, myFile);
    printf("%d\n", r1);
    printf("%d\n", r2);
  }
}
/*
void make_connection(struct router** routerPointer, int from, int to){
  if (rp->n == NULL){
  }
*/
