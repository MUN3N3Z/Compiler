#include <stdio.h>
#include "Expression.h"

#ifndef SYMTABLE_HEADER
#define SYMTABLE_HEADER
/* maximum size of hash table */
#define SYMTABLE_SIZE 61

/* maximum size of tokens-identifiers */
#define MAXTOKENLEN 40

/* token types */
#define UNDEF 0
#define LONG_TYPE 1
#define LOGIC_TYPE 2
#define FUNCTION_TYPE 3

// struct that represents a list node
typedef struct list_t{
  int st_size;
  int scope;
	// type
  int st_type;
  // Pointer to the AST node
  Node* node;
  // pointer to next item in the list
	struct list_t *next;
} list_t;

/* symtable struct. */
typedef struct _symtable {
  list_t **hash_table;
  int cur_scope;
} SymTable;

// Function Declarations
// initialize hash table
SymTable* init_hash_table();
// hash function
unsigned int hash(char *key);
// insert entry
void Insert(SymTable* symtable, Node* node, int type);
// search for entry
list_t *Lookup(SymTable* symtable, char *name);
// search for entry in the current scope
list_t *LookupScope(SymTable* symtable, char *name);
// Clear symbol table for the next function
void ClearSymTable(SymTable* symtable);
// Completely delete the symbol table
void DeleteSymTable(SymTable* symtable);
// dump file
void symtab_dump(SymTable* symtable, FILE *of);

void DecrScope(SymTable* symtable);
void IncrScope(SymTable* symtable);
#endif
