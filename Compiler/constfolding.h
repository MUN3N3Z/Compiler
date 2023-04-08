/*
*************************************************************************************
     CONSTFOLDING.H : DECLARATION OF FUNCTIONS ADDED IN CONSTFOLDING.C HERE
**************************************************************************************
*/

#include "Expression.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern bool madeChange;

/*
***********************************************************************
  FUNCTION DECLARATIONS
************************************************************************
*/

long CalcExprValue(Node* node);
long ConstFoldPerStatement(Node* stmtNodeRight);
void ConstFoldPerFunction(Node* funcNode);
bool ConstantFolding(NodeList* funcdecls);

/*
***********************************************************************
  ADD DECLARATIONS OF ANY FUNCTIONS YOU ADD BELOW THIS LINE
************************************************************************
*/
