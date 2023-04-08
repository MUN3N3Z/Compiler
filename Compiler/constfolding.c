/*
   CONSTFOLDING.C : THIS FILE IMPLEMENTS THE CONSTANT FOLDING OPTIMIZATION
*/

#include "constfolding.h"
/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL 
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO 
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/                                                                                                          
bool madeChange = true;

/*
******************************************************************************************
FUNCTION TO CALCULATE THE CONSTANT EXPRESSION VALUE 
OBSERVE THAT THIS IMPLEMENTATION CONSIDERS ADDITIONAL OPTIMIZATIONS SUCH AS:
1.  IDENTITY MULTIPLY = 1 * ANY_VALUE = ANY_VALUE - AVOID MULTIPLICATION CYCLE IN THIS CASE
2.  ZERO MULTIPLY = 0 * ANY_VALUE = 0 - AVOID MULTIPLICATION CYCLE
3.  DIVIDE BY ONE = ORIGINAL_VALUE - AVOID DIVISION CYCLE
4.  SUBTRACT BY ZERO = ORIGINAL_VALUE - AVOID SUBTRACTION
5.  MULTIPLICATION BY 2 = ADDITION BY SAME VALUE [STRENGTH REDUCTION]
******************************************************************************************
*/
long CalcExprValue(Node* node)
{
     long result;
     Node *leftNode, *rightNode;
     leftNode = node->left;
     rightNode = node->right; 
     switch(node->opCode){
         case MULTIPLY:
             if (leftNode->value == 1) {
                 result = rightNode->value;
             } 
             else if (rightNode->value == 1) {
                 result = leftNode->value;
             }
             else if (leftNode->value == 0 || rightNode->value == 0) {
                 result = 0;
             }
             else if (leftNode->value == 2) {
                 result = rightNode->value + rightNode->value;
             }              
             else if (rightNode->value == 2) {
                 result = leftNode->value + leftNode->value;
             }
             else {
                 result = leftNode->value * rightNode->value;
             }
             break;
         case DIVIDE:
             if (rightNode->value == 1) {
                 result = leftNode->value;
             }
             else {
                 result = leftNode->value / rightNode->value;
             }
             break;
         case ADD:
             result = leftNode->value + rightNode->value;
             break;
         case SUBTRACT:
             result = leftNode->value - rightNode->value;
             break;
         case NEGATE:
             result = -leftNode->value;
             break;
         default:
             break;
     }
     return result;
}


/*
**********************************************************************************************************************************
// YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS BELOW THIS LINE
**********************************************************************************************************************************
*/

/*
*****************************************************************************************************
THIS FUNCTION IS MEANT TO PROCESS THE CANDIDATE STATEMENTS AND PERFORM CONSTANT FOLDING 
WHEREVER APPLICABLE.
******************************************************************************************************
*/
long ConstFoldPerStatement(Node* stmtNodeRight){
    long result;
    // OPERATION folding
    // UNARY operation
    if (stmtNodeRight->opCode == NEGATE)
    {   
        // Only process NUMBERS
        if(stmtNodeRight->left->exprCode == CONSTANT)
        {
            result = CalcExprValue(stmtNodeRight);
            return result;
        }
    }
    // BINARY operation
    else if (stmtNodeRight->opCode != FUNCTIONCALL)
    {
        // Only process NUMBERS
        if(stmtNodeRight->left->exprCode == CONSTANT && stmtNodeRight->right->exprCode == CONSTANT)
        {
            result = CalcExprValue(stmtNodeRight);
            return result;
        }
    }                                                                                              
    return __LONG_MAX__;
}


/*
*****************************************************************************************************
THIS FUNCTION IS MEANT TO IDENTIFY THE STATEMENTS THAT ARE ACTUAL CANDIDATES FOR CONSTANT FOLDING
AND CALL THE APPROPRIATE FUNCTION FOR THE IDENTIFIED CANDIDATE'S CONSTANT FOLDING
******************************************************************************************************
*/
void ConstFoldPerFunction(Node* funcNode) {
    Node *rightNode, *leftNode, *stmtNodeRight;
    long result;
    NodeList* statements = funcNode->statements;
    while (statements != NULL) 
    {
        Node* statement = statements->node;
        // ASSIGN STATEMENT
        if (statement->stmtCode == ASSIGN)
        {
            rightNode = statement->right;
        }   
        else if(statement->stmtCode == RETURN)  
        {
            rightNode = statement->left;
        }
        // Filter BINARY AND UNARY operations
        if (rightNode->exprCode == OPERATION && rightNode->opCode != FUNCTIONCALL)
        {
            // Fold operation
            result = ConstFoldPerStatement(rightNode);
            // Skip if operation was unsuccessful
            if (result == __LONG_MAX__)
            {
                statements = statements->next;
                continue;
            }
            // Replace operation expression with computed value
            if (statement->stmtCode == ASSIGN) // ASSIGN STATEMENT
            {
                statement->right = CreateNumber(result);
                madeChange = true;
            }
            else if(statement->opCode == RETURN)
            {
                statement->left = CreateNumber(result);
                madeChange = true;
            }
            // Clean up
            if (rightNode->opCode == NEGATE) // UNARY 
            {
                FreeUnaryOperation(rightNode);
            }
            else if(rightNode->opCode != FUNCTIONCALL) // BINARY
            {
                FreeBinaryOperation(rightNode);
            }
        }
        statements = statements->next;
    }
}



/*
**********************************************************************************************************************************
// YOU CAN MAKE CHANGES/ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE
********************************************************************************************************************************
*/

/*
*****************************************************************************************************
THIS FUNCTION ENSURES THAT THE CONSTANT FOLDING OPTIMIZATION IS DONE FOR EVERY FUNCTION IN THE PROGRAM
******************************************************************************************************
*/

bool ConstantFolding(NodeList* list) {
    madeChange = false;
    while (list != NULL) 
    {
        Node* functNode = list->node;
        ConstFoldPerFunction(functNode);  
	    list = list->next;
    }
    return madeChange;
}

/*
****************************************************************************************************************************
 END OF CONSTANT FOLDING
*****************************************************************************************************************************
*/                
