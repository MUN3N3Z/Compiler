#include "Expression.h"
#include "optimizer.h"
#include "codegen.h"
#include "symtab.h"
#include "Parser.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyparse(NodeList** funcdecls, int* errorCount, SymTable* symtable,
            yyscan_t scanner);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <source file>\n", argv[0]);
    exit(0);
  }
  
  // Open file to read
  FILE* f = fopen(argv[1], "r");
  if (!f) {
    printf("Cannot find file: %s\n", argv[1]);
    exit(0);
  }
  
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  char* source_content = malloc(fsize + 1);
  fread(source_content, fsize, 1, f);
  fclose(f);
  source_content[fsize] = 0;
  //printf("Input source file\n");
  //printf("%s\n", source_content);

  NodeList* funcdecls;
  YY_BUFFER_STATE state;

  yyscan_t scanner;
  if (yylex_init(&scanner)) return -1;
  
  SymTable* symtable = init_hash_table();
  if (!symtable) {
    printf("Error creating symbolic table\n");
    return -1;
  }
  
  // Scan the source file
  state = yy_scan_string(source_content, scanner);
  // Parse the source file
  int errorCount = 0;
  int stat = yyparse(&funcdecls, &errorCount, symtable, scanner);
  if (errorCount > 0) {
    printf("%d error(s) encounter while parsing.\n", errorCount);
    return -1;
  }
  
  // Delete buffer;
  yy_delete_buffer(state, scanner);
  
  // Destroy lexer
  yylex_destroy(scanner);

  // Delete symbol table now
  DeleteSymTable(symtable);
  
  /************************************************************
   * THIS IS WHERE WE DO STUFF
   * Essentially, the variable funcdecls contain the AST of function declaration.
   * And the function declaration is what we have to compile.
   *************************************************************/
  printf("Printing the AST BEFORE OPTIMIZATION\n");
  PrintProgram(funcdecls);
  Optimizer(funcdecls); 
  printf("Printing the AST AFTER OPTIMIZATION\n");
  PrintProgram(funcdecls);
  Codegen(funcdecls);
  
  // Finish.
  free(source_content);
  FreeProgram(funcdecls);
  return 0;
}
