#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

SymTable* init_hash_table(){
  SymTable* symtable = calloc(sizeof(SymTable), 1);
  if (!symtable) return NULL;
  symtable->hash_table = calloc(SYMTABLE_SIZE, sizeof(list_t*));
  symtable->cur_scope = 0;
  return symtable;
}

unsigned int hash(char *key){
	unsigned int hashval = 0;
	for(;*key!='\0';key++) hashval += *key;
	hashval += key[0] % 11 + (key[0] << 3) - key[0];
	return hashval % SYMTABLE_SIZE;
}

void Insert(SymTable* symtable, Node* node, int type){
  unsigned int hashval = hash(node->name);
  list_t *l = symtable->hash_table[hashval];
  l = (list_t*) calloc(sizeof(list_t), 1);
  l->st_type = type;
  l->node = node;
  l->scope = symtable->cur_scope;
  /* add to hashtable */
  l->next = symtable->hash_table[hashval];
  symtable->hash_table[hashval] = l;
}

list_t *Lookup(SymTable* symtable, char *name){
  /* return symbol if found or NULL if not found */
  unsigned int hashval = hash(name);
  list_t *l = symtable->hash_table[hashval];
  while ((l != NULL) && (strcmp(name,l->node->name) != 0)) l = l->next;
  return l; // NULL is not found
}

list_t *LookupScope(SymTable* symtable, char *name){
  /* return symbol if found or NULL if not found */
  unsigned int hashval = hash(name);
  list_t *l = symtable->hash_table[hashval];
  while ((l != NULL) &&
         ((strcmp(name,l->node->name) != 0) ||
	  l->scope != symtable->cur_scope)) {
    l = l->next;
  }
  return l; // NULL if not found
}

void DecrScope(SymTable* symtable){ /* hide the current scope */
  if (symtable->cur_scope > 0) symtable->cur_scope--;
}

void IncrScope(SymTable* symtable){ /* go to next scope */
  symtable->cur_scope++;
}

void ClearSymTable(SymTable* symtable) {
  /* Delete scope >= this current scope */
  list_t** table = symtable->hash_table;
  for (int i=0; i < SYMTABLE_SIZE; ++i){
    if (table[i] == NULL) continue;
    
    list_t* head = table[i];
    while (head != NULL && head->scope >= symtable->cur_scope) {
      list_t* temp = head;
      head = head->next;
      free(temp);
    }
    table[i] = head;

    /*
     It should never be the case that once we find a lower scope, we find the current scope again.
    list_t* temp = head;
    list_t* prev;
    while (temp != NULL) {
      while (temp != NULL && temp->scope < symtable->cur_scope) {
        prev = temp;
        temp = temp->next;
      }
      
      if (temp == NULL) continue;
      prev->next = temp->next;
      free(temp);
      temp = prev->next;
    }
     */
  }
}

void DeleteSymTable(SymTable* symtable) {
  list_t** table = symtable->hash_table;
  for (int i=0; i < SYMTABLE_SIZE; ++i){
    if (table[i] == NULL) continue;
    list_t* head = table[i];
    while (head != NULL) {
      list_t* temp = head;
      head = head->next;
      free(temp);
    }
  }
  free(table);
  free(symtable);
}

/* print to stdout by default */ 
void symtab_dump(SymTable* symtable, FILE * of){
  int i;
  printf("------------ ----- ------ ------------\n");
  printf("Name         SCOPE Type   Line Numbers\n");
  printf("------------ ----- ------ -------------\n");
  for (i=0; i < SYMTABLE_SIZE; ++i){
    if (symtable->hash_table[i] != NULL){
      list_t *l = symtable->hash_table[i];
      while (l != NULL){
        printf("%-12s ",l->node->name);
        printf("%6d ", l->scope);
        if (l->st_type == LONG_TYPE) printf("%-7s","long");
        else if (l->st_type == FUNCTION_TYPE) printf("%-7s", "function");
        else printf("%-7s","undef"); // if UNDEF or 0
        printf("\n");
        l = l->next;
      }
    }
  }
}
