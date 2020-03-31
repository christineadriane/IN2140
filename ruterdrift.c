/* include shared header with declarations and includes */
#include "shared.h"

// struct with id, flag, model lenght, model name
struct router{
  unsigned int id;
  unsigned char flag;
  unsigned char mLenght;
  char model[248];

  int nCounter; // number of neighbours
  struct router* nList[10]; // list of neighbours
};

struct router** routerPointer;

int main(int argc, char const *argv[]) {

  // Opening file with first argument, read binary
  FILE *myFile = fopen(argv[1], "rb");
  if (myFile == NULL){
    perror("Nej");
    return EXIT_FAILURE;
  }

  // keeping track of number of routers by reading the first int
  int routers;
  fread(&routers, sizeof(int), 1, myFile);
  printf("Number of routers: %d\n", routers);

  // allocate space for router pointers
  routerPointer = malloc((sizeof(struct router*)) * routers); //8 * routers byte

  int i = 0;
  for (i = 0; i < routers; i++){
    read(myFile, i);
  }

  // Tester innlesing av øvrig infoblokker
  read_connections(myFile, routers);

  // Lukker fil
  fclose(myFile);

  // Start reading argv[2]
  FILE *newFile = fopen(argv[2], "r");
  if (newFile == NULL){
    perror("Nej");
    return EXIT_FAILURE;
  }
  printf("Reading from file: %s\n", argv[2]);

  char readLine[248];
  char* string;

  while (fgets(readLine, 248, newFile)!= NULL){
    string = strtok(readLine, " ");

    // print
    if(strcmp(string, "print") == 0){
      string = strtok(NULL, " ");
      print(routers, atoi(string));
      printf("\n");
    }

    // set model
    if(strcmp(string, "sett_modell") == 0){
      string = strtok(NULL, " ");
      char* name = strtok(NULL, "\n");
      set_model(routers, atoi(string), name);
    }

    // add connection
    if(strcmp(string, "legg_til_kobling") == 0){
      string = strtok(NULL, " ");
      int id = atoi(strtok(NULL, " "));
      add_connection(routers, atoi(string), id);
      printf("\n");
    }

    // delete router
    if(strcmp(string, "slett_router") == 0){
      string = strtok(NULL, " ");
      delete(routers, atoi(string));
    }

    // set flag
    if(strcmp(string, "sett_flag") == 0){
      string = strtok(NULL, "\n");
    }

    // route?
    if(strcmp(string, "finnes_rute") == 0){
      string = strtok(NULL, "\n");
    }
  }
  // close file
  fclose(newFile);

  // write to file
  FILE *test = fopen("test.txt", "w");
  if (test == NULL){
    perror("Nej");
    return EXIT_FAILURE;
  }

  // If there is a new number of routers after some is deleted.
  int newRouters = 0;
  int a = 0;
  for(a = 0; a < routers; a++){
    struct router* r = routerPointer[a];
    if (r != NULL){
      newRouters++;
    }
  }

  // write the first 4 bytes with number of routers
  fwrite(&newRouters, sizeof(int), 1, test);

  // write information about the routers
  for(a = 0; a < routers; a++){
    struct router* r = routerPointer[a];
    if (r != NULL){
      fwrite(&r->id, sizeof(int), 1, test);
      fwrite(&r->flag, sizeof(char), 1, test);
      fwrite(&r->mLenght, sizeof(char), 1, test);
      fwrite(&r->model, routerPointer[a]->mLenght - sizeof(char), 1, test);
      fwrite("\n", sizeof(char), 1, test);
    }
  }

  // write information about connecting routers
  for(a = 0; a < newRouters; a++){
    struct router* r = routerPointer[a];
    if (r != NULL){
      int b;
      for(b = 0; b < r->nCounter; b++){
        if((r->nList[b]) != NULL){
          fwrite(&r->id, sizeof(int), 1, test);
          fwrite(&r->nList[b]->id, sizeof(int), 1, test);
          fwrite("\n", sizeof(char), 1, test);
        }
      }
    }
  }
  // closing the test file
  fclose(test);

  // free all the allocated memory
  free_memory(routers);
}

// method to read info to struct
void read(FILE *myFile, int n){

  struct router* rp = malloc(sizeof(struct router)); //256

  fread(&rp -> id, sizeof(int), 1, myFile);
  fread(&rp -> flag, sizeof(char), 1, myFile);
  fread(&rp -> mLenght, sizeof(char), 1, myFile);
  fread(&rp -> model, rp -> mLenght + 1, 1, myFile);
  rp->nCounter = 0;

  /*  Tester print
  printf("Model: %s\n", rp -> model);
  printf("ID: %d\n", rp -> id);
  printf("Flag: %d\n", rp -> flag);
  //printf("Lengde: %d\n", rp -> mLenght);
  printf("\n");
  */

  routerPointer[n]=rp;
}

// reading and adding connections
// also increased neighbour counter
void read_connections(FILE* myFile, int routers){
  unsigned int ru1 = 0;
  unsigned int ru2 = 0;
  unsigned int nullByte = 0;

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

    /* Tester print av koblingene
    printf("%d\n", ru1);
    printf("%d\n", ru2);
    */
  }
}

// General method to find a router based on ID
struct router* find_router(int routers, int id){
  int i;
  for(i = 0; i < routers; i++){
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
  int i;
  for(i = 0; i < routers - 1; i++) {
    struct router* r = routerPointer[i];
      if (r != NULL) {
        free(r); // Free 256 Bytes
      }
      else{
        printf("%d already freed", i);
      }
    }
    free(routerPointer); // Free routers * 8 Bytes
    return;
}

// set modell
// change the name of an existing router
void set_model(int routers, int id, char* name){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }
  struct router* routerPointer = find_router(routers, id);
  strcpy(routerPointer->model, name);
  routerPointer -> mLenght = strlen(name)+1;
  printf("Router %d successfully changed name to '%s'\n", id, name);
}

// add connections
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

  printf("New connection: %d -> %d\n", id, id2);

}

// Ferdig kommando, printe bitwise på flagg?
void print(int routers, int id){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }

  struct router* routerPointer = find_router(routers, id);
  printf("\nPrinting info about router %d:\n", id);
  printf("Model: %s\n", routerPointer-> model);
  printf("ID: %d\n", routerPointer -> id );
  //printf("Flag: %d\n", routerPointer -> flag);
  printf("Flag: ");
  bit_print(routerPointer->flag);

  printf("\nNeighbours:\n");

  // finding neighbours in nList and printer their id and model
  int i;
  for(i = 0; i < routerPointer->nCounter; i++){
    printf("Router: %d - %s\n", routerPointer-> nList[i]->id, routerPointer->nList[i]->model);
  }
}

// delete router
// removing all pointers to said routers, decreasing nCounter for the previous neighbours
void delete(int routers, int id){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }

  int i;
  for(i = 0; i < routers; i++){
    struct router* rp = find_router(routers, i);
    int j;
    for (j = 0; j < rp->nCounter; j++){
      if (rp->nList[j]->id == id){
        int k;
        for(k = j; k < rp->nCounter - 1; k++){
          rp->nList[k] = rp->nList[k+1];
        }
        rp->nCounter --;
      }
    }
  }

  int l;
  for(l = 0; l < routers -1; l++){
    if(routerPointer[l]->id == id){
      free(routerPointer[l]);
      break;
    }
  }
  int m;
  for(m = l; m < routers - 1; m++){
    routerPointer[m] = routerPointer[m + 1];
  }
  printf("Deleted router: %d\n", id);
}

void bit_print(char b){
  int i;
  for (i = 7; i >= 0; i--){
    printf("%d", (b >> i) & 0x01);
  }
  printf("\n");
}
