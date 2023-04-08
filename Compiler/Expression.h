/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __Node_H__
#define __Node_H__

#include <stdbool.h>

typedef enum _NodeType {
  N_NONE,
  FUNCTIONDECL,
  STATEMENT,
  EXPRESSION,
} NodeType;

typedef enum _StmtType {
  S_NONE,
  ASSIGN,
  RETURN,
} StmtType;

typedef enum _ExprType {
  E_NONE,
  VARIABLE,
  CONSTANT,
  PARAMETER,
  OPERATION
} ExprType;

typedef enum _OpType {
  O_NONE,
  FUNCTIONCALL,
  MULTIPLY,
  DIVIDE,
  ADD,
  SUBTRACT,
  NEGATE,
  BOR,
  BAND,
  BXOR,
  BSHR,
  BSHL
} OpType;

typedef struct _NodeList NodeList;

/* 
 ALL IR Nodes are expressed with Node. In cases we have to keep a list of nodes, then we use NodeList.
 Use type, exprCode, opCode, and stmtCode to identify exactly what this node represents.
 More information on each IR node in "Helper functions to create IR nodes"
*/
typedef struct _Node {
  char* name;
  long value;

  NodeType type;
  ExprType exprCode;
  OpType opCode;
  StmtType stmtCode;
  
  struct _Node *left;
  struct _Node *right;

  NodeList* arguments;
  NodeList* statements;
} Node;

typedef struct _NodeList {
  Node* node;
  NodeList* next;
} NodeList;

/*********************************************************************************************************
                   Helper functions to create Intermediate Representation (IR) nodes
 **********************************************************************************************************/

Node *AllocateNode();
NodeList *AllocateNodeList();
NodeList *CreateNodeList(Node* n, NodeList* next);
int NodeListLen(NodeList* nl);

/* Create a Node representing a function declaration.
 E.g.
 long foo (long arg1, long arg2, long arg3, ){
  statement 1;
  statement 2;
  statement 3;
  ...;
 }
 
 ** Function declaration node has the following fields set:
 type: FUNCTIONDECL
 stmtCode: S_NONE
 exprCode: E_NONE
 opCode: O_NONE
 name: Name of the function
 arguments: stores a list of parameters, ordered from left to right.
 statements: stores a list of statements, ordered from top to bottom.
 
 ** The following fields are not set and should not be used:
 left, right, and value
 */
Node* CreateFunctionDecl(char* name, NodeList* args, NodeList* stmts);

/* Create a Node representing a return statement.
 E.g.
 return x;
 
 ** Return node has the following fields set:
 type: STATEMENT
 stmtCode: RETURN
 exprCode: E_NONE
 opCode: O_NONE
 left: The node that represents the returned expression (e.g., x)
 
 ** The following fields are not set and should not be used:
 name, right, value, arguments, statements
 */
Node* CreateReturn(Node* expr);

/* Create a Node representing an assign statement:
 E.g.
 a = 2 >> 3;
 
 ** Assign node has the following fields set:
 type: STATEMENT
 stmtCode: ASSIGN
 exprCode: E_NONE
 opCode: O_NONE
 name: name of the variable
 right: Node that represents the right side of "=" sign (e.g., 2 >> 3)
 
 ** The following fields are not set and should not be used:
 left, value, arguments, statements
 */
Node* CreateAssign(char* var, Node* expr);

/* Create a node representing a binary operation:
 E.g.,
 a + b
 
 ** BinaryOperation node has the following fields set:
 type: EXPRESSION
 stmtCode: S_NONE
 exprCode: OPERATION
 opCode : the binary operation opcode (e.g., +)
 left: The left side of the binary operation (e.g., a)
 right: The right side of the binary operation (e.g., b)
 
 ** The following fields are not set and should not be used:
 name, value, arguments, statements
 */
Node* CreateBinaryOperation(OpType opcode,
                            Node* left, Node* right);

/* Create a node representing a unary operation:
 E.g.,
 -x
 
 ** UnaryOperation node has the following fields set:
 type: EXPRESSION
 stmtCode: S_NONE
 exprCode: OPERATION
 opCode: the unary operation opcode (e.g., -)
 left: The inner expression (e.g., x)
 
 ** The following fields are not set and should not be used:
 name, right, value, arguments, statements
 */
Node* CreateUnaryOperation(OpType opcode, Node* expr);

/* Create a node representing a function call:
 E.g.,
 foo(a, b, 4)
 
 ** FunctionCall node has the following fields set:
 type: EXPRESSION
 stmtCode: S_NONE
 exprCode: OPERATION
 opCode: FUNCTIONCALL
 left: The node to the function declaration
 arguments: the list of arguments, ordered from left to right
 
 ** The following fields are not set and should not be used:
 name, right, value, statements
 */
Node* CreateFunctionCall(Node* functionDecl, NodeList* args);

/* Create a node representing a variable:
 E.g.,
 x
 ** This node is primarily used for making a connection between a referred
    variable and its origin.
 
 ** Variable node has the following fields set:
 type: EXPRESSION
 stmtCode: S_NONE
 exprCode: VARIABLE
 opCode: O_NONE
 name: Name of the variable
 left: Either an assignment node or a function parameter node referred by this
       variable.

 ** The following fields are not set and should not be used:
 right, value, arguments, statements
 */
Node* CreateVariable(char* name, Node* ref);

/* Create a node representing a function parameter
 E.g., arg1, arg2, or arg3 in 'long foo(long arg1, long arg2, long arg3)'
 
 ** These nodes are used ONLY for a function parameter. It is not used for
    locally declared variable.
 
 ** Parameter node has the following fields set:
 type: EXPRESSION
 stmtCode: S_NONE
 exprCode: PARAMETER
 opCode: O_NONE
 name: Name of the parameter
 
 ** The following fields are not set and should not be used:
 left, right, value, arguments, statements
 */
Node* CreateParameter(char* name);

// Example: 5
// this is an expression
// constant saved into "value" field

/* Create a node representing a long integer value
 E.g.,
 5
 
 ** Number node has the following fields set:
 type: EXPRESSION
 stmtCode: S_NONE
 exprCode: CONSTANT
 opCode: O_NONE
 value: Value of the long integer.
 
 ** The following fields are not set and should not be used:
 name, left, right, arguments, statements
 */
Node* CreateNumber(long value);


/*********************************************************************************************************
                                  Helper functions to print the program
 **********************************************************************************************************/
void PrintProgram(NodeList* list);
void PrintNode(Node* node);
void PrintFunctionDecl(Node* node);
void PrintParameters(NodeList* node);
void PrintStatements(NodeList* nodelist);
void PrintStatement(Node* node);
void PrintReturn(Node* node);
void PrintAssignment(Node* node);
void PrintExpression(Node* node);
void PrintOperation(Node* node);
void PrintBinaryOperation(Node* node, char* op);
void PrintUnaryOperation(Node* node, char* op);
void PrintFunctionCall(Node* node);

/*********************************************************************************************************
                               Helper functions to free IR Node and NodeList
 **********************************************************************************************************/
// Recursively free all nodes representing the entire program.
void FreeProgram(NodeList* list);
/*
 Recursively frees all nodes representing a function declaration, including 
 parameters and statements.
 **Caution**: If a statement references the freed function declaration, 
 it will result in null reference error
 */
void FreeFunctionDecl(Node* node);
// Frees a list of nodes representing parameters (in a function declaration).
void FreeParameters(NodeList* node);
// Frees aa node representing a parameter.
void FreeParameter(Node* node);
// Recursively frees nodes representing a list of statements.
void FreeStatements(NodeList* nodelist);
/*
 Recursively frees nodes representing a statement (assignment or return statement).
 ** Caution**: It does not free a referenced statement. For example, in
 long a = b + 3;
 This function will not free the node referenced by the variabel node "b",
 But it will free the "b" node.
 */
void FreeStatement(Node* node);
/*
 Recursively frees nodes representing a return statement.
 ** Caution**: It does not free a referenced statement. For example, in
 return a;
 This function will not free the node referenced by "a".
 */
void FreeReturn(Node* node);
/*
 Recursively frees nodes representing an assignment statement.
 ** Caution**: It does not free a referenced statement. For example, in
 long x = a + b;
 This function will not free the node referenced by "a" or "b".
 */
void FreeAssignment(Node* node);
/*
 Recursively frees nodes representing an expression.
 ** Caution**: It does not free a referenced statement. For example, in
 a + 3
 This function will not free the node referenced by "a".
 */
void FreeExpression(Node* node);
// Free the node representing a variable
void FreeVariable(Node* node);
// Free the node representing a constant
void FreeConstant(Node* node);
/*
 Recursively frees nodes representing an operation (binary, unary, function 
 call).
 ** Caution**: It does not free a referenced statement. For example, in
 a + 3
 This function will not free the node referenced by "a".
 */
void FreeOperation(Node* node);
// Recursively frees nodes representing a binary operation
void FreeBinaryOperation(Node* node);
// Recursively frees nodes representing a unary operation
void FreeUnaryOperation(Node* node);
/*
 Recursively frees nodes representing a function call including arguments.
 ** Caution**: It does not free the function declaration that it references.
 */
void FreeFunctionCall(Node* node);
// Frees a list of expression. Primarily used to free function call arguments.
void FreeExpressionList(NodeList* list);

#endif /* __Node_H__ */
