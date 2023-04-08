/*
 * Expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "Expression.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Node *AllocateNode() {
  Node* b = calloc(sizeof(Node), 1);
  if (b == NULL) return NULL;
  return b;
}

NodeList *AllocateNodeList() {
  NodeList* b = calloc(sizeof(NodeList), 1);
  if (b == NULL) return NULL;
  return b;
}

NodeList *CreateNodeList(Node* n, NodeList* next) {
  NodeList* b = AllocateNodeList();
  b->node = n;
  b->next = next;
  return b;
}

int NodeListLen(NodeList* nl) {
  if (nl == NULL) return 0;
  return 1 + NodeListLen(nl->next);
}

Node* CreateFunctionDecl(char* name, NodeList* args, NodeList* stmts) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = FUNCTIONDECL;
  b->stmtCode = S_NONE;
  b->exprCode = E_NONE;
  b->opCode = O_NONE;
  b->name = name;
  b->arguments = args;
  b->statements = stmts;
  return b;
}

Node* CreateReturn(Node* expr) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = STATEMENT;
  b->stmtCode = RETURN;
  b->exprCode = E_NONE;
  b->opCode = O_NONE;
  b->left = expr;
  return b;
}

Node* CreateAssign(char* var, Node* expr) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = STATEMENT;
  b->stmtCode = ASSIGN;
  b->exprCode = E_NONE;
  b->opCode = O_NONE;
  b->name = var;
  b->right = expr;
  return b;
}

Node* CreateBinaryOperation(OpType opcode, Node* left, Node* right) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;

  b->type = EXPRESSION;
  b->exprCode = OPERATION;
  b->opCode = opcode;
  b->stmtCode = S_NONE;
  b->left = left;
  b->right = right;
  return b;
}

Node* CreateUnaryOperation(OpType opcode, Node* expr) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = EXPRESSION;
  b->exprCode = OPERATION;
  b->opCode = opcode;
  b->stmtCode = S_NONE;
  b->left = expr;
  return b;
}

Node* CreateFunctionCall(Node* functionDecl, NodeList* args) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = EXPRESSION;
  b->stmtCode = S_NONE;
  b->exprCode = OPERATION;
  b->opCode = FUNCTIONCALL;
  b->left = functionDecl;
  b->arguments = args;
  return b;
}

Node* CreateVariable(char* name, Node* ref) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = EXPRESSION;
  b->exprCode = VARIABLE;
  b->opCode = O_NONE;
  b->stmtCode = S_NONE;
  b->name = name;
  b->left = ref;
  return b;
}

Node* CreateParameter(char* name) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = EXPRESSION;
  b->exprCode = PARAMETER;
  b->opCode = O_NONE;
  b->stmtCode = S_NONE;
  b->name = name;
  return b;
}

Node* CreateNumber(long value) {
  Node* b = AllocateNode();
  if (b == NULL) return NULL;
  
  b->type = EXPRESSION;
  b->exprCode = CONSTANT;
  b->opCode = O_NONE;
  b->stmtCode = S_NONE;
  b->value = value;
  return b;
}

void PrintProgram(NodeList* list) {
  while (list != NULL) {
    PrintNode(list->node);
    list = list->next;
  }
}

void PrintNode(Node* node) {
  switch(node->type) {
    case FUNCTIONDECL:
      PrintFunctionDecl(node);
      break;
    case STATEMENT:
      PrintStatement(node);
      break;
    case EXPRESSION:
      PrintExpression(node);
      break;
    default:
      break;
  }
}

void PrintFunctionDecl(Node* node) {
  printf("long %s (", node->name);
  PrintParameters(node->arguments);
  printf(") {\n");
  PrintStatements(node->statements);
  printf("}\n");
}

void PrintParameters(NodeList* nodelist) {
  for (NodeList* nl = nodelist; nl != NULL; nl = nl->next) {
    PrintExpression(nl->node);
    if (nl->next != NULL) printf(", ");
  }
}

void PrintStatements(NodeList* nodelist) {
  for (NodeList* nl = nodelist; nl != NULL; nl = nl->next) {
    printf("\t");
    PrintStatement(nl->node);
    printf(";\n");
  }
}

void PrintStatement(Node* node) {
  switch(node->stmtCode) {
    case ASSIGN :
      PrintAssignment(node);
      break;
    case RETURN :
      PrintReturn(node);
      break;
    default :
      break;
  }
}

void PrintReturn(Node* node) {
  printf("return ");
  PrintExpression(node->left);
}

void PrintAssignment(Node* node) {
  printf("%s = ", node->name);
  PrintExpression(node->right);
}

void PrintExpression(Node* node) {
  Node* ref;
  switch (node->exprCode) {
    case VARIABLE :
      ref = node->left;
      if (ref->type == EXPRESSION && ref->exprCode == PARAMETER) {
        // This variable is a parameter
        printf("%s", node->name);
      } else if (ref->type == STATEMENT && ref->stmtCode == ASSIGN) {
        // This variable is a locally initialized variable
        printf("%s", node->name);
      }
      break;
    case CONSTANT :
      printf("%ld", node->value);
      break;
    case PARAMETER :
      printf("long %s", node->name);
      break;
    case OPERATION:
      PrintOperation(node);
      break;
    default :
      break;
  }
}

void PrintOperation(Node* node) {
  switch (node->opCode) {
    case FUNCTIONCALL:
      PrintFunctionCall(node);
      break;
    case MULTIPLY:
      PrintBinaryOperation(node, "*");
      break;
    case DIVIDE:
      PrintBinaryOperation(node, "/");
      break;
    case ADD:
      PrintBinaryOperation(node, "+");
      break;
    case SUBTRACT:
      PrintBinaryOperation(node, "-");
      break;
    case NEGATE:
      PrintUnaryOperation(node, "-");
      break;
    case BOR:
      PrintBinaryOperation(node, "|");
      break;
    case BAND:
      PrintBinaryOperation(node, "&");
      break;
    case BXOR:
      PrintBinaryOperation(node, "^");
      break;
    case BSHR:
      PrintBinaryOperation(node, ">>");
      break;
    case BSHL:
      PrintBinaryOperation(node, "<<");
      break;
    default :
      break;
  }
}

void PrintFunctionCall(Node* node) {
  printf("%s(", node->left->name);
  PrintParameters(node->arguments);
  printf(")");
}

void PrintBinaryOperation(Node* node, char* op) {
  printf("(");
  PrintExpression(node->left);
  printf(" %s ", op);
  PrintExpression(node->right);
  printf(")");
}

void PrintUnaryOperation(Node* node, char* op) {
  printf("(");
  printf("%s", op);
  PrintExpression(node->left);
  printf(")");
}

void FreeProgram(NodeList* list) {
  if (list == NULL) return;
  FreeProgram(list->next);
  // Free each function declaration
  FreeFunctionDecl(list->node);
  free(list);
}

void FreeFunctionDecl(Node* node) {
  // Free the parameters
  FreeParameters(node->arguments);
  // Free statements
  FreeStatements(node->statements);
  free(node->name);
  free(node);
}

void FreeParameters(NodeList* list) {
  if (list == NULL) return;
  FreeParameters(list->next);
  FreeParameter(list->node);
  free(list);
}

void FreeParameter(Node* node) {
  free(node->name);
  free(node);
}

void FreeStatements(NodeList* list) {
  if (list == NULL) return;
  FreeStatements(list->next);
  FreeStatement(list->node);
  free(list);
}

void FreeStatement(Node* node) {
  if (node->stmtCode == ASSIGN) return FreeAssignment(node);
  if (node->stmtCode == RETURN) return FreeReturn(node);
}

void FreeReturn(Node* node) {
  FreeExpression(node->left);
  free(node);
}

void FreeAssignment(Node* node) {
  free(node->name);
  FreeExpression(node->right);
  free(node);
}

void FreeExpression(Node* node) {
  switch (node->exprCode) {
    case VARIABLE :
      return FreeVariable(node);
    case CONSTANT :
      return FreeConstant(node);
    case PARAMETER :
      return FreeParameter(node);
    case OPERATION :
      return FreeOperation(node);
    default :
      break;
  }
}

void FreeVariable(Node* node) {
  free(node->name);
  // Do not free the left field as this node does not own them.
  // Parameter nodes are owned by function declaration.
  // Assign node is owned by assignment itself.
  free(node);
}

void FreeConstant(Node* node) {
  free(node);
}

void FreeOperation(Node* node) {
  switch (node->opCode) {
    case FUNCTIONCALL:
      return FreeFunctionCall(node);
    case MULTIPLY:
    case DIVIDE:
    case ADD:
    case SUBTRACT:
    case BOR:
    case BAND:
    case BXOR:
    case BSHR:
    case BSHL:
      return FreeBinaryOperation(node);
    case NEGATE:
      return FreeUnaryOperation(node);
    default:
      break;
  }
}

void FreeFunctionCall(Node* node) {
  // We do not free left. It is owned by the function declaration itself.
  //free(node->name);
  FreeExpressionList(node->arguments);
  free(node);
}

void FreeBinaryOperation(Node* node) {
  FreeExpression(node->left);
  FreeExpression(node->right);
  free(node);
}

void FreeUnaryOperation(Node* node) {
  FreeExpression(node->left);
  free(node);
}

void FreeExpressionList(NodeList* list) {
  if (list == NULL) return;
  FreeExpressionList(list->next);
  FreeExpression(list->node);
  free(list);
}
