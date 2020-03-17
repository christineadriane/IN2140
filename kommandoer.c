/*
Forsøk på kommandoer når resten virker
*/

/*
print ruter-ID : Skriver ut til stdout informasjon on ruteren og ID-ene til dens naboer, nærmere
bestemt bare de naboene som ruteren peker til.
*/

void print(int routers, int id){
  for(int i = 0; i < routers; i++){
    if(routerPointer[i]->id == id){
      printf("Model: %s\n", routerPointer[i]-> model);
      printf("ID: %d\n", routerPointer[i] -> id );
      printf("Flag: %d\n", routerPointer[i] -> flag);

      // Usikker på denne
      printf("Neighbours:\n");
      for(int j = 0; j <= 10; j++){
        if(nList[j] != NULL){
          printf("%d\n", nList[j]);
        }
      }
    }
  }
}

void delete(int routers, int id){

}

void set_flag(int id, int position, int value){

}

void set_model(int routers, int id, char* name){
  for (int i = 0; i < routers; i++){
    if(routerPointer[i] -> id == id){
      memcpy(routerPointer[i]->model, name, strlen(name));
      routerPointer[i]->strlen = strlen(name);
    }
  }
}

void add_connection(int routers, int id){

}

void finnes_rute(int routers, int id){

}


// Free Memory - free allocated memory to avoid leaks
// int* numRouters:	Number of routers
int free(int routers) {
  for(int i = 0; i < routers; i++) {
    struct router* r = routers[i];
      if (r != NULL) {
        free(r); // Free 256 Bytes
        r = NULL;
      }
    }
    free(routers); // Free routers * 8 Bytes
    return 0;
}
