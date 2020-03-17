#include "shared.h"

struct router{
  unsigned int id;
  unsigned char flag;
  unsigned char mLenght;
  char model[248];

  int nCounter; // Antall naboer
  struct router* nList[10];
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
    read(myFile, i);
  }

  // Tester innlesing av øvrig infoblokker
  read_connections(myFile, routers);

  // !!Segmentation fault!!
  find_router(routers, 0);

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

// innlesing til struct
void read(FILE *myFile, int n){

  struct router* rp = malloc(sizeof(struct router)); //256

  fread(&rp -> id, sizeof(int), 1, myFile);
  fread(&rp -> flag, sizeof(char), 1, myFile);
  fread(&rp -> mLenght, sizeof(char), 1, myFile);
  fread(&rp -> model, rp -> mLenght + 1, 1, myFile);

  printf("Model: %s\n", rp -> model);
  printf("ID: %d\n", rp -> id);
  printf("Flag: %d\n", rp -> flag);
  printf("Lengde: %d\n", rp -> mLenght);
  printf("\n");
}

void read_connections(FILE* myFile, int routers){
  unsigned int r1;
  unsigned int r2;
  unsigned int nullByte;

  long start = ftell(myFile);
  fseek(myFile, 0, SEEK_END);
  long end = ftell(myFile);
  fseek(myFile, start, SEEK_SET);

  struct router* np = malloc(sizeof(struct router*) * 10);

  while(ftell(myFile) != end){
    fread(&r1, sizeof(int), 1, myFile);
    fread(&r2, sizeof(int), 1, myFile);
    fread(&nullByte, 1, 1, myFile);
    printf("%d\n", r1);
    printf("%d\n", r2);
    np->nCounter ++;
  }

  printf("\nAntall koblinger: %d\n", np->nCounter);
}

  /*
  for(int i = 0; i < routers; i++){
    if (routerPointer[i] != NULL){
      if (routerPointer[i]->id == r1){
        r1 -> nList[nCounter];
      }
    }
  }
  */


struct router* find_router(int routers, int id){
  for(int i = 0; i < routers; i++){
    if(routerPointer[i] != NULL){
      if(routerPointer[i]->id == id){
        return routerPointer[i];
        printf("sneik\n");
      }
    }
  }
  return NULL;
  printf("usnel\n");
}
