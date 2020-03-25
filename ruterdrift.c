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

  // Tester å legge til en kobling
  add_connection(routers, 9, 1);
  printf("\nRuter 9 antall naboer: %d\n", routerPointer[9]->nCounter);
  printf("\nRuter 9 sin nabo: %d\n", routerPointer[9]->nList[0]->id);

  add_connection(routers, 6, 1);

  printf("\nRuter 6 sine naboer: %d, %d, %d, %d\n", routerPointer[6]->nList[0]->id, routerPointer[6]->nList[1]->id, routerPointer[6]->nList[2]->id, routerPointer[6]->nList[3]->id);

  // Tester om ting kobles sammen
  printf("\nHEI: %d\n", routerPointer[6]->nList[2]->id);

  //Tester å bytte modellnavn
  set_model(routers, 0, "Fin ny ruter");
  printf("\nNytt navn: %s\n", routerPointer[0]->model);

  // Tester print
  print(routers, 6);


  // Tester sletting
  delete(routers, 2);
  printf("Naboen til 6: %u\n", routerPointer[5]->nList[1]->id);
  printf("ID: %d\n", routerPointer[2]->id);

  // Lukker fil
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

// Innlesing til struct
// Ferdig
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

  routerPointer[n]=rp;
}

// Leser inn og legger inn connections
// Legger til naboteller
void read_connections(FILE* myFile, int routers){
  unsigned int ru1;
  unsigned int ru2;
  unsigned int nullByte;

  long start = ftell(myFile);
  fseek(myFile, 0, SEEK_END);
  long end = ftell(myFile);
  fseek(myFile, start, SEEK_SET);

  while(ftell(myFile) != end){
    fread(&ru1, sizeof(int), 1, myFile);
    fread(&ru2, sizeof(int), 1, myFile);
    fread(&nullByte, 1, 1, myFile);

    struct router* r1 = (find_router(routers, ru1));
    struct router* r2 = (find_router(routers, ru2));
    r1->nList[r1->nCounter] = r2;
    r1->nCounter++;

    printf("%d\n", ru1);
    printf("%d\n", ru2);
  }
}

// General methis to find a router based on ID
struct router* find_router(int routers, int id){
  for(int i = 0; i < routers; i++){
    if(routerPointer[i]->id == id){
      return routerPointer[i];
    }
  }
  return NULL;
}

/* ----------------------- */
// COMMANDS
/* ----------------------- */

// Free Memory - free allocated memory to avoid leaks
void free_memory(int routers) {
  for(int i = 0; i < routers; i++) {
    struct router* r = routerPointer[i];
      if (r != NULL) {
        free(r); // Free 256 Bytes
        r = NULL;
      }
    }
    free(routerPointer); // Free routers * 8 Bytes
    return;
}

// Set modell
// Funker slik den skal
void set_model(int routers, int id, char* name){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }
  struct router* routerPointer = find_router(routers, id);
  strcpy(routerPointer->model, name);
  routerPointer -> mLenght = strlen(name)+1;
}

// Add connections
/*
Metoden funker. Burde ta hensyn til og legge til at den sjekker tilkoblinger som allerede finnes
*/
void add_connection(int routers, int id, int id2){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }
  if (find_router(routers, id2) == NULL){
    printf("Router: %d does not exist.\n", id2);
    return;
  }
  struct router* r1 = (find_router(routers, id));
  struct router* r2 = (find_router(routers, id2));
  r1->nList[r1->nCounter] = r2;
  r1->nCounter++;
}

// Ferdig kommando, printe bitwise på flagg?
// Gjør det ved overskudd
void print(int routers, int id){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }

  struct router* routerPointer = find_router(routers, id);
  printf("Model: %s\n", routerPointer-> model);
  printf("ID: %d\n", routerPointer -> id );
  printf("Flag: %d\n", routerPointer -> flag);

  printf("\nNaboer:\n");

  // Finner naboer i nList og printer id og modell
  for(int i = 0; i < routerPointer->nCounter; i++){
    printf("Router: %d\nModel: %s\n", routerPointer-> nList[i]->id, routerPointer->nList[i]->model);
  }
}


// Slette ruter
// FERDIG
void delete(int routers, int id){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }

  for(int i = 0; i < routers; i++){
    struct router* rp = find_router(routers, i);
    for (int j = 0; j < rp->nCounter; j++){
      if (rp->nList[j]->id == id){
        for(int k = j; k < rp->nCounter - 1; k++){
          rp->nList[k] = rp->nList[k+1];
        }
        rp->nCounter --;
      }
    }
  }
  int l;
  for(l = 0; l < routers -1; l++){
    if(routerPointer[l]->id == id){
      printf("l: %d\n", l);
      break;
    }
  }
  int m;
  for(m = l; m < routers - 1; m++){
    printf("m: %d\n", m);
    printf("Router 1: %d\n", routerPointer[m]->id);
    printf("Ruter 2: %d\n", routerPointer[m +1 ]->id);
    routerPointer[m] = routerPointer[m + 1];
    printf("Router hei: %d\n", routerPointer[m]->id);
  }
}
