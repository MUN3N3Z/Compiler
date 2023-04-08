#pragma once
/*
*************************************************************************************
     CODEGEN.H : DECLARATION OF FUNCTIONS ADDED IN CODEGEN.C HERE
**************************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Expression.h"
#define INVAL -999

/*
*************************************************************************************
     GLOBAL VARIABLES
**************************************************************************************
*/
// The arg counter is used to iterate through the arg list.
extern int argCounter;

// The last used offset will help determine the last used stack location
extern int lastUsedOffset;
extern char lastOffsetUsed[100]; 

// The file pointer is the file pointer for the assembly file
extern FILE *fptr;

/*
*************************************************************************************
     THE REGINFO LIST TRACKS IF REGISTERS ARE AVAILABLE FOR USE
**************************************************************************************
*/
typedef struct _register {
    char* regName;
    int avail;
    struct _register* next;
} regInfo;

/*
*************************************************************************************
     THE VARSTOREINFO LIST TRACKS A VARIABLE NAME, VALUE AND WHERE IT IS STORED
**************************************************************************************
*/
typedef struct _varStoreInfo {
    char* varName;
    // FLAG TO IDENTIFY IF A VARIABLE IS A CONSTANT OR NOT.
    bool isConst;
    long value;
    // LOCATION COULD BE A STACK LOCATION OR A REGISTER 
    // eg: -8(%rbp) or %rcx
    char location[10];
    struct _varStoreInfo* next;
    struct _varStoreInfo* prev;
} varStoreInfo;

extern regInfo *regList, *regHead, *regLast;
extern varStoreInfo *varList, *varHead, *varLast;


/*
*************************************************************************************
     FUNCTION DECLARATIONS
**************************************************************************************
*/
void InitAsm(char* funcName);
void FreeRegList();
void FreeVarList();
void AddRegInfo(char* name, int avail);
void CreateRegList();
bool IsAvailReg(char* name);
void UpdateRegInfo(char* regName, int avail);
char* GetNextAvailReg(bool noAcc);
void AddVarInfo(char* varName, char* location, long val, bool isConst);
char* LookUpVarInfo(char* name, long val);
void PrintVarListInfo();
void UpdateVarInfo(char* varName, char* location, long val, bool isConst);
void PrintRegListInfo();
int PushArgOnStack(NodeList* arguments);
void PopArgFromStack(NodeList* arguments);
void RetAsm();
void LongToCharOffset();
void SaveValInRax(char* name);
void ProcessStatements(NodeList* statements);
void Codegen(NodeList* funcdecls);

/*
***********************************************************************
  ADD DECLARATIONS OF ANY FUNCTIONS YOU ADD BELOW THIS LINE
************************************************************************
*/
char* Var_Present(char* name);
void CountVars(Node* funcdecl);
void Operate_binary(OpType operation_type, Node* left_op, Node* right_op);