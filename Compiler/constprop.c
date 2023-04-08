/*
********************************************************************************
  CONSTPROP.C : IMPLEMENT THE DOWNSTREAM CONSTANT PROPOGATION OPTIMIZATION HERE
*********************************************************************************
*/

#include "constprop.h"

refConst *lastNode, *headNode;
/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/

/*
***********************************************************************
  FUNCTION TO FREE THE CONSTANTS-ASSOCIATED VARIABLES LIST
************************************************************************
*/
void FreeConstList()
{
   refConst* tmp;
   while (headNode != NULL)
    {
       tmp = headNode;
       headNode = headNode->next;
       free(tmp);
    }

}

/*
*************************************************************************
  FUNCTION TO ADD A CONSTANT VALUE AND THE ASSOCIATED VARIABLE TO THE LIST
**************************************************************************
*/
void UpdateConstList(char* name, long val) {
    refConst* node = malloc(sizeof(refConst));
    if (node == NULL) return;
    node->name = name;
    node->val = val;
    node->next = NULL;
    if (headNode == NULL) {
        lastNode = node;
        headNode = node;
    }
    else {
        lastNode->next = node;
        lastNode = node;
    }
}

/*
*****************************************************************************
  FUNCTION TO LOOKUP IF A CONSTANT ASSOCIATED VARIABLE IS ALREADY IN THE LIST
******************************************************************************
*/
refConst* LookupConstList(char* name) {
    refConst *node;
    node = headNode; 
    while (node != NULL){
        if (!strcmp(name, node->name))
            return node;
        node = node->next;
    }
    return NULL;
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS BELOW THIS LINE
**********************************************************************************************************************************
*/

/**
 * Exit program if error is detected
*/
void StopIfNot(bool cond)
{
    if (!cond) 
    {
      exit(EXIT_FAILURE);
    }
}

/**
 * Statement checker
*/
bool IsStatement(const Node* n){
  if (n->type != STATEMENT)
  {
    return false;
  }
  return true;
}

/**
 * Function Declaration checker
*/
bool IsFunctionDecl(const Node* n){
  if (n->type != FUNCTIONDECL)
  {
    return false;
  }
  return true;
}

/**
 * Print ConstList
*/
void PrintConstList(const refConst* list_head){
  const refConst* temp = list_head;
  while (temp != NULL)
  {
    printf("Name: %s; Value: %li \n", temp->name, temp->val);
    temp = temp->next;
  }
}
/*
************************************************************************************
  THIS FUNCTION IS MEANT TO UPDATE THE CONSTANT LIST WITH THE ASSOCIATED VARIABLE
  AND CONSTANT VALUE WHEN ONE IS SEEN. IT SHOULD ALSO PROPOGATE THE CONSTANTS WHEN 
  WHEN APPLICABLE. YOU CAN ADD A NEW FUNCTION IF YOU WISH TO MODULARIZE BETTER.
*************************************************************************************
*/
void TrackConst(NodeList* statements) {
      Node* node;
      while (statements != NULL) 
      {
        node = statements->node;
        // Ensure node is a statement
        StopIfNot(IsStatement(node));
        // ASSIGNMENT statement
        if (node->stmtCode == ASSIGN)
        {
          char* var_name = node->name;
          // Error-checking
          if (LookupConstList(var_name) != NULL) // Attempt to redefine a variable
          {
              exit(EXIT_FAILURE);
          }
          // Get variable value (right side of varaible assignment) and push to list ()
          Node* var_right_side = node->right;
          if (var_right_side->exprCode == CONSTANT)
          {
            UpdateConstList(var_name, var_right_side->value);
          }
        }
        statements = statements->next;
      }
}


bool ConstProp(NodeList* funcdecls) {
      Node* node;
      madeChange = false;
      while (funcdecls != NULL)
      {
        node = funcdecls->node;
        StopIfNot(IsFunctionDecl(node));
        NodeList* statements_list = node->statements;
        // Track all constant variables in the function declaration
        TrackConst(statements_list);
        // Access statements
        while (statements_list != NULL)
        {
          Node* statement = statements_list->node;
          // ASSIGNMENT statement
          if (statement->stmtCode == ASSIGN)
          {
            Node* potential_var_name = statement->right;
            // Propagate plain constant variable
            if (potential_var_name->exprCode == VARIABLE)
            {
              refConst* search_result = LookupConstList(potential_var_name->name);
              if (search_result != NULL)
              {
                statement->right = CreateNumber(search_result->val);
                madeChange = true;
                // Clean up
                FreeVariable(potential_var_name);
              }
            }
            // Propagate operation containing constant variables (function calls, binary, unary)
            else if(potential_var_name->exprCode == OPERATION)
            {
              // FUNCTION CALL
              if (potential_var_name->opCode == FUNCTIONCALL)
              {
                // Propagate function parameters
                NodeList* temp = potential_var_name->arguments;
                while (temp != NULL)
                {
                  Node* parameter =  temp->node;
                  // Assume all arguments are PARAMETERS
                  refConst* replacement;
                  if (parameter->exprCode == PARAMETER || parameter->exprCode == VARIABLE)
                  {
                    replacement = LookupConstList(parameter->name);
                  }
                  else
                  {
                    replacement = NULL;
                  }
                  
                  if (replacement != NULL)
                  {
                    // Keep track of previous variable
                    Node* old_parameter = parameter;
                    // Propagate new constant to replace parameter
                    temp->node = CreateNumber(replacement->val);
                    madeChange = true;
                    // Clean up
                    FreeParameter(old_parameter);
                  }
                  // Move to next parameter
                  temp = temp->next;
                }
              }
              // UNARY OPERATION
              else if (potential_var_name->opCode == NEGATE)
              {
                // Propagate left operand
                Node* left_op = potential_var_name->left;
                if (left_op->exprCode == VARIABLE)
                {
                  refConst* replacement = LookupConstList(left_op->name);
                  if (replacement != NULL)
                  {
                    // Propagate new constant to replace left operand
                    potential_var_name->left = CreateNumber(replacement->val);
                    madeChange = true;
                    // Clean up
                    FreeVariable(left_op);
                  }
                }
              }
              // BINARY OPERATION
              else 
              {
                // Propagate left operand
                Node* left_op = potential_var_name->left;
                if (left_op->exprCode == VARIABLE)
                {
                  refConst* replacement = LookupConstList(left_op->name);
                  if (replacement != NULL)
                  {
                    // Propagate new constant to replace left operand
                    potential_var_name->left = CreateNumber(replacement->val);
                    madeChange = true;
                    // Clean up
                    FreeVariable(left_op);
                  }
                }
                // Propagate right operand
                Node* right_op = potential_var_name->right;
                if (right_op->exprCode == VARIABLE)
                {
                  refConst* replacement = LookupConstList(right_op->name);
                  if (replacement != NULL)
                  {
                    // Propagate new constant to replace left operand
                    potential_var_name->right = CreateNumber(replacement->val);
                    madeChange = true;
                    // Clean up
                    FreeVariable(right_op);
                  }
                }
              }
            }
          }
          // RETURN Statement
          else if (statement->stmtCode == RETURN)
          {
            Node* returned_statement = statement->left;
            // Propagate if possible
            if (returned_statement->exprCode == VARIABLE)
            {
              // Get variable details
              refConst* search_result = LookupConstList(returned_statement->name);
              // Propagate variable
              if (search_result != NULL)
              {
                statement->left = CreateNumber(search_result->val);
                madeChange = true;
                // Clean up
                FreeVariable(returned_statement);
              }
            }
          }
          statements_list = statements_list->next;
        }
        // Clean up constant list for current function declaration
        FreeConstList();
        funcdecls = funcdecls->next;
      }
      return madeChange;
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE
**********************************************************************************************************************************
*/