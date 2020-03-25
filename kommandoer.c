/*
Forsøk på kommandoer når resten virker
*/

/*
print ruter-ID : Skriver ut til stdout informasjon on ruteren og ID-ene til dens naboer, nærmere bestemt bare de naboene som ruteren peker til.
*/

// Ferdig kommando, printe bitwise?
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

  // Finner naboer i nList og printer id og modell
  for(int i = 0; i < routerPointer->nCounter; i++){
    printf("Router: %d\n Model: %s\n", routerPointer-> nList[i]->id, routerPointer->nList[i]->model);
  }
}


void set_flag(int id, int position, int value){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }
}


void finnes_rute(int routers, int id){

}




// Ferdig kommando. Satt inn i hovedfil
// Sett modell
void set_model(int routers, int id, char* name){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }
  struct router* routerPointer = find_router(routers, id);
  strcpy(routerPointer->model, name);
  routerPointer -> mLenght = strlen(name)+1;
}

// Ferdig kommando. Satt inn i hovedfil
// Legg til kobling
void add_connection(int routers, int id, int id2){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }
  struct router* r1 = (find_router(routers, id));
  struct router* r2 = (find_router(routers, id2));
  r1->nList[r1->nCounter+1] = r2;
  r1->nCounter++;
}

// Free Memory - free allocated memory to avoid leaks
// satt inn i hovedfil
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

// Delete - Slette ruter
// satt inn i hovedfil
void delete(int routers, int id){
  if (find_router(routers, id) == NULL){
    printf("Router: %d does not exist.\n", id);
    return;
  }

  struct router* routerPointer = find_router(routers, id);

  for(int i = 0; i < routers; i++){
    if (routerPointer[i]->id == id){
      routerPointer[i] = NULL;
    }
  }
}



/*
// Free Memory - free allocated memory to avoid leaks
void free_memory(int routers) {
  for(int i = 0; i < routers; i++) {
      if (routerPointer[i] != NULL){
        free(routerPointer[i]); // Free 256 Bytes
        routerPointer = NULL;
      }
    }
    free(routerPointer); // Free routers * 8 Bytes
    return;
}
*/
