/*
***********************************************************************
  DEADASSIGN.C : IMPLEMENT THE DEAD CODE ELIMINATION OPTIMIZATION HERE
************************************************************************
*/

#include "deadassign.h"

int change;
refVar *last, *head;

/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/

/*
***********************************************************************
  FUNCTION TO INITIALIZE HEAD AND LAST POINTERS FOR THE REFERENCED 
  VARIABLE LIST.
************************************************************************
*/

void init()
{ 
    head = NULL;
    last = head;
}

/*
***********************************************************************
  FUNCTION TO FREE THE REFERENCED VARIABLE LIST
************************************************************************
*/

void FreeList()
{
   refVar* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}

/*
***********************************************************************
  FUNCTION TO IDENTIFY IF A VARIABLE'S REFERENCE IS ALREADY TRACKED
************************************************************************
*/
bool VarExists(char* name) {
   refVar *node;
   node = head;
   while (node != NULL) {
       if (!strcmp(name, node->name)) {
           return true;
       }
       node = node->next;
    }
    return false;
}

/*
***********************************************************************
  FUNCTION TO ADD A REFERENCE TO THE REFERENCE LIST
************************************************************************
*/
void UpdateRefVarList(char* name) {
    refVar* node = malloc(sizeof(refVar));
    if (node == NULL) return;
    node->name = name;
    node->next = NULL;
    if (head == NULL) {
        last = node;
        head = node;
    }
    else {
        last->next = node;
        last = node;
    }
}

/*
****************************************************************************
  FUNCTION TO PRINT OUT THE LIST TO SEE ALL VARIABLES THAT ARE USED/REFERRED
  AFTER THEIR ASSIGNMENT. YOU CAN USE THIS FOR DEBUGGING PURPOSES OR TO CHECK
  IF YOUR LIST IS GETTING UPDATED CORRECTLY
******************************************************************************
*/
void PrintRefVarList() {
    refVar *node;
    node = head;
    if (node == NULL) {
        printf("\nList is empty"); 
        return;
    }
    while (node != NULL) {
        printf("\t %s", node->name);
        node = node->next;
    }
}

/*
***********************************************************************
  FUNCTION TO UPDATE THE REFERENCE LIST WHEN A VARIABLE IS REFERENCED 
  IF NOT DONE SO ALREADY.
************************************************************************
*/
void UpdateRef(Node* node) {
      if (node->right != NULL && node->right->exprCode == VARIABLE) {
          if (!VarExists(node->right->name)) {
              UpdateRefVarList(node->right->name);
          }
      }
      if (node->left != NULL && node->left->exprCode == VARIABLE) {
          if (!VarExists(node->left->name)) {
              UpdateRefVarList(node->left->name);
          }
      }
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS BELOW THIS LINE
**********************************************************************************************************************************
*/


/*
********************************************************************
  THIS FUNCTION IS MEANT TO TRACK THE REFERENCES OF EACH VARIABLE
  TO HELP DETERMINE IF IT WAS USED OR NOT LATER
********************************************************************
*/

void TrackRef(Node* funcNode) {
  NodeList* statements = funcNode->statements;
  // Initialize list of variables
  init();
  while (statements != NULL) 
  {
    Node* statement = statements->node;
    Node* right_side;
    // ASSIGNMENT STATEMENT
    if (statement->stmtCode == ASSIGN)
    {
      right_side = statement->right;
    }
    // RETURN STATEMENT
    else if(statement->stmtCode == RETURN)
    {
      right_side = statement->left;
    }
    // Handle different cases where variables can be used
    // Direct VARIABLE assignment
    if (right_side->exprCode == VARIABLE)
    {
      UpdateRef(statement);
    }
    // OPERATION
    else if (right_side->exprCode == OPERATION)
    {
      Node* variable1;
      Node* variable2;
      // UNARY OPERATION
      if (right_side->opCode == NEGATE)
      {
        variable1 = right_side->left;
        if (variable1->exprCode == VARIABLE)
        {
          UpdateRef(right_side);
        }
      }
      // FUNCTIONCALL
      else if(right_side->opCode == FUNCTIONCALL)
      {
        // Check parameters
        NodeList* params = right_side->arguments;
        
        while (params != NULL)
        {
          Node* temp = params->node;
          if (temp->exprCode == VARIABLE)
          {
            UpdateRefVarList(temp->name);
          }
          params = params->next;
        }
      }
      // BINARY OPERATION
      else
      {
        variable1 = right_side->left;
        variable2 = right_side->right;
        // Variable_1 check
        if (variable1->exprCode == VARIABLE)
        {
          UpdateRef(right_side);
        }
        // Variable_2 check
        if (variable2->exprCode == VARIABLE)
        {
          UpdateRef(right_side);
        }
      }
    }
    statements = statements->next;
  }
}

/*
***************************************************************
  THIS FUNCTION IS MEANT TO DO THE ACTUAL DEADCODE REMOVAL
  BASED ON THE INFORMATION OF TRACKED REFERENCES
****************************************************************
*/
NodeList* RemoveDead(NodeList* statements) {
    refVar* varNode;
    NodeList *prev, *temp, *first, *next; // Keep track of new list of statements
    Node* curr_statement;
    // Keep track of the list of statements
    first = NULL;
    temp = NULL;
    next = NULL;
    size_t count = 0;
    change = 0;
    while (statements != NULL) 
    {
      curr_statement = statements->node;
      // End if RETURN statement is encountered
      if (curr_statement->stmtCode == RETURN)
      {
        if(temp != NULL)
        {
          temp->next = statements;
        }
        else
        {
          first = statements;
        }
        break;
      }
      char* var_name = curr_statement->name;
      // First statement
      if (count == 0)
      {
        if (!VarExists(var_name))
        { 
          change = 1;
          prev = NULL;
          next = statements->next;
          // Clean up
          FreeStatement(curr_statement);
          free(statements);
        }
        else
        {
          temp = statements;
          first = temp;
          prev = statements;
          next = statements->next;
        }
      }
      else
      {
        if (!VarExists(var_name))
        {
          next = statements->next;
          // Clean up
          FreeStatement(curr_statement);
          free(statements);
          change = 1;
        }
        else
        {
          if (prev != NULL)
          {
            // Connect statement to rest of list
            prev->next = statements;
            temp = statements;
            prev = statements;
            next = statements->next;
          }
          else
          {
            first = statements;
            prev = first;
            next = statements->next;
            temp = statements;
          }
        }
      }
      statements = next;
      count ++;
    }
   return first;
}

/*
********************************************************************
  THIS FUNCTION SHOULD ENSURE THAT THE DEAD CODE REMOVAL PROCESS
  OCCURS CORRECTLY FOR ALL THE FUNCTIONS IN THE PROGRAM
********************************************************************
*/
bool DeadAssign(NodeList* funcdecls) {
  bool madeChange = false;
  while (funcdecls != NULL) 
  {
    Node* function = funcdecls->node;
    // Track variables used in the function
    TrackRef(function);
    // Replace prev statements (with deadcode) with new list (without deadcode, HOPEFULLY)
    function->statements = RemoveDead(function->statements);
    // Clean up
    FreeList();
    funcdecls = funcdecls->next;
  }
  if (change == 1) madeChange=true;
  return madeChange;
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE
**********************************************************************************************************************************
*/
 
