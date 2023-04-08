/*
***********************************************************************
  CODEGEN.C : IMPLEMENT CODE GENERATION HERE
************************************************************************
*/
#include "codegen.h"

int varCounter;
int argCounter;
int lastUsedOffset;
char lastOffsetUsed[100];
FILE *fptr;
regInfo *regList, *regHead, *regLast;
varStoreInfo *varList, *varHead, *varLast;
char* registers[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9", "%rax"};

/*
*************************************************************************************
   YOUR CODE IS TO BE FILLED IN THE GIVEN TODO BLANKS. YOU CAN CHOOSE TO USE ALL
   UTILITY FUNCTIONS OR NONE. YOU CAN ADD NEW FUNCTIONS. BUT DO NOT FORGET TO
   DECLARE THEM IN THE HEADER FILE.
**************************************************************************************
*/

/*
***********************************************************************
  FUNCTION TO INITIALIZE THE ASSEMBLY FILE WITH FUNCTION DETAILS
************************************************************************
*/
void InitAsm(char* funcName) {
    fprintf(fptr, "\n.globl %s", funcName);
    fprintf(fptr, "\n%s:", funcName); 

    // Init stack and base ptr
    fprintf(fptr, "\npushq %%rbp");  
    fprintf(fptr, "\nmovq %%rsp, %%rbp"); 
}

/*
***************************************************************************
   FUNCTION TO WRITE THE RETURNING CODE OF A FUNCTION IN THE ASSEMBLY FILE
****************************************************************************
*/
void RetAsm() {
    fprintf(fptr,"\npopq  %%rbp");
    fprintf(fptr, "\nretq\n");
} 

/*
***************************************************************************
  FUNCTION TO CONVERT OFFSET FROM LONG TO CHAR STRING 
****************************************************************************
*/
void LongToCharOffset() {
     lastUsedOffset = lastUsedOffset - 8;
     snprintf(lastOffsetUsed, 100,"%d", lastUsedOffset);
     strcat(lastOffsetUsed,"(%rbp)");
}

/*
***************************************************************************
  FUNCTION TO CONVERT CONSTANT VALUE TO CHAR STRING
****************************************************************************
*/
void ProcessConstant(Node* opNode) {
     char value[10];
     LongToCharOffset();
     snprintf(value, 10,"%ld", opNode->value);
     char str[100];
     snprintf(str, 100,"%d", lastUsedOffset);
     strcat(str,"(%rbp)");
     AddVarInfo("", str, opNode->value, true);
     fprintf(fptr, "\nmovq  $%s, %s", value, str);
}

/*
***************************************************************************
  FUNCTION TO SAVE VALUE IN ACCUMULATOR (RAX)
****************************************************************************
*/
void SaveValInRax(char* name) {
    char *tempReg;
    tempReg = GetNextAvailReg(true);
    if (!(strcmp(tempReg, "NoReg"))) {
        LongToCharOffset();
        fprintf(fptr, "\n movq %%rax, %s", lastOffsetUsed);
        UpdateVarInfo(name, lastOffsetUsed, INVAL, false);
        UpdateRegInfo("%rax", 1);
    }
    else {
        fprintf(fptr, "\nmovq %%rax, %s", tempReg);
        UpdateRegInfo(tempReg, 0);
        UpdateVarInfo(name, tempReg, INVAL, false);
        UpdateRegInfo("%rax", 1);
    }
}



/*
***********************************************************************
  FUNCTION TO ADD VARIABLE INFORMATION TO THE VARIABLE INFO LIST
************************************************************************
*/
void AddVarInfo(char* varName, char* location, long val, bool isConst) {
   varStoreInfo* node = malloc(sizeof(varStoreInfo));
   node->varName = varName;
   node->value = val;
   strcpy(node->location,location);
   node->isConst = isConst;
   node->next = NULL;
   node->prev = varLast;
   if (varHead == NULL) {
       varHead = node;
       varLast = node;;
       varList = node;
   } else {
       //node->prev = varLast;
       varLast->next = node;
       varLast = varLast->next;
   }
   varList = varHead;
}

/*
***********************************************************************
  FUNCTION TO FREE THE VARIABLE INFORMATION LIST
************************************************************************
*/
void FreeVarList()
{  
   varStoreInfo* tmp;
   while (varHead != NULL)
    {  
       tmp = varHead;
       varHead = varHead->next;
       free(tmp);
    }

}

/*
***********************************************************************
  FUNCTION TO LOOKUP VARIABLE INFORMATION FROM THE VARINFO LIST
************************************************************************
*/
char* LookUpVarInfo(char* name, long val) {
    varList = varLast;
    if (varList == NULL) printf("NULL varlist");
    while (varList != NULL) {
        if (varList->isConst == true) {
            if (varList->value == val) return varList->location;
        }
        else {
            if (!strcmp(name,varList->varName)) return varList->location;
        }
        varList = varList->prev;
    }
    varList = varHead;
    return "";
}

/*
***********************************************************************
  FUNCTION TO UPDATE VARIABLE INFORMATION 
************************************************************************
*/
void UpdateVarInfo(char* varName, char* location, long val, bool isConst) {
  
   if (!(strcmp(LookUpVarInfo(varName, val), ""))) {
       AddVarInfo(varName, location, val, isConst);
   }
   else {
       varList = varHead;
       if (varList == NULL) printf("NULL varlist");
       while (varList != NULL) {
           if (!strcmp(varList->varName,varName)) {
               varList->value = val;
               strcpy(varList->location,location);
               varList->isConst = isConst;
               break;
        }
        varList = varList->next;
       }
    }
    varList = varHead;
}

/*
***********************************************************************
  FUNCTION TO PRINT THE VARIABLE INFORMATION LIST
************************************************************************
*/
void PrintVarListInfo() {
    varList = varHead;
    if (varList == NULL) printf("NULL varlist");
    while (varList != NULL) {
        if (!varList->isConst) {
            printf("\t %s : %s", varList->varName, varList->location);
        }
        else {
            printf("\t %ld : %s", varList->value, varList->location);
        }
        varList = varList->next;
    }
    varList = varHead;
}

/*
***********************************************************************
  FUNCTION TO ADD NEW REGISTER INFORMATION TO THE REGISTER INFO LIST
************************************************************************
*/
void AddRegInfo(char* name, int avail) {

   regInfo* node = malloc(sizeof(regInfo));
   node->regName = name;
   node->avail = avail;
   node->next = NULL; 

   if (regHead == NULL) {
       regHead = node;
       regList = node;
       regLast = node;
   } else {
       regLast->next = node;
       regLast = node;
   }
   regList = regHead;
}

/*
***********************************************************************
  FUNCTION TO FREE REGISTER INFORMATION LIST
************************************************************************
*/
void FreeRegList()
{  
   regInfo* tmp;
   while (regHead != NULL)
    {  
       tmp = regHead;
       regHead = regHead->next;
       free(tmp);
    }

}

/*
***********************************************************************
  FUNCTION TO UPDATE THE AVAILIBILITY OF REGISTERS IN THE REG INFO LIST
************************************************************************
*/
void UpdateRegInfo(char* regName, int avail) {
    while (regList != NULL) {
        if (regName == regList->regName) {
            regList->avail = avail;
        }
        regList = regList->next;
    }
    regList = regHead;
}

/*
***********************************************************************
  FUNCTION TO RETURN THE NEXT AVAILABLE REGISTER
************************************************************************
*/
char* GetNextAvailReg(bool noAcc) {
    regList = regHead;
    if (regList == NULL) printf("NULL reglist");
    while (regList != NULL) {
        if (regList->avail == 1) {
            if (!noAcc) return regList->regName;
            // if not rax and dont return accumulator set to true, return the other reg
            // if rax and noAcc == true, skip to next avail
            if (noAcc && strcmp(regList->regName, "%rax")) { 
                return regList->regName;
            }
        }
        regList = regList->next;
    }
    regList = regHead;
    return "NoReg";
}

/*
***********************************************************************
  FUNCTION TO DETERMINE IF ANY REGISTER APART FROM OR INCLUDING 
  THE ACCUMULATOR(RAX) IS AVAILABLE
************************************************************************
*/
int IfAvailReg(bool noAcc) {
    regList = regHead;
    if (regList == NULL) printf("NULL reglist");
    while (regList != NULL) {
        if (regList->avail == 1) {
            // registers available
            if (!noAcc) return 1;
            if (noAcc && strcmp(regList->regName, "%rax")) {
                return 1;
            }
        }
        regList = regList->next;
    }
    regList = regHead;
    return 0;
}

/*
***********************************************************************
  FUNCTION TO DETERMINE IF A SPECIFIC REGISTER IS AVAILABLE
************************************************************************
*/
bool IsAvailReg(char* name) {
    regList = regHead;
    if (regList == NULL) printf("NULL reglist");
    while (regList != NULL) {
        if (!strcmp(regList->regName, name)) {
           if (regList->avail == 1) {
               return true;
           } 
        }
        regList = regList->next;
    }
    regList = regHead;
    return false;
}

/*
***********************************************************************
  FUNCTION TO PRINT THE REGISTER INFORMATION
************************************************************************
*/
void PrintRegListInfo() {
    regList = regHead;
    if (regList == NULL) printf("NULL reglist");
    while (regList != NULL) {
        printf("\t %s : %d", regList->regName, regList->avail);
        regList = regList->next;
    }
    regList = regHead;
}

/*
***********************************************************************
  FUNCTION TO CREATE THE REGISTER LIST
************************************************************************
*/
void CreateRegList() {
    // Create the initial reglist which can be used to store variables.
    // 4 general purpose registers : AX, BX, CX, DX
    // 4 special purpose : SP, BP, SI , DI. 
    // Other registers: r8, r9
    // You need to decide which registers you will add in the register list 
    // use. Can you use all of the above registers?

    // REGISTER ASSIGNMENT (in order): %DI, %SI, DX, CX, R8, R9 **AX**
    // Create a list of empty registers
    
    AddRegInfo("%rdi", 1);
    AddRegInfo("%rsi", 1);
    AddRegInfo("%rdx", 1);
    AddRegInfo("%rcx", 1);
    AddRegInfo("%r8", 1);
    AddRegInfo("%r9", 1);
    AddRegInfo("%rax", 1);
}   



/*
***********************************************************************
  THIS FUNCTION IS MEANT TO PUT THE FUNCTION ARGUMENTS ON STACK
************************************************************************
*/
int PushArgOnStack(NodeList* arguments) {
    // Count arguments
    int arg_counter = 0;
    NodeList* temp = arguments;
    while (temp != NULL) 
    {
        temp = temp->next;
        arg_counter ++;
    }
    // Create stack frame for parameters and variables
    int stack_space = ((varCounter + arg_counter) * 8);
    if(stack_space != 0)
    {
        fprintf(fptr, "\nsubq $%d, %rsp", stack_space);
    }
    /**
     * Push arguments to stack
     *  Move arguments to registers
    */
   int index = 0;
    while (arguments != NULL)
    {
    /*
     ***********************************************************************
              TODO : YOUR CODE HERE
      THINK ABOUT WHERE EACH ARGUMENT COMES FROM. EXAMPLE WHERE IS THE 
      FIRST ARGUMENT OF A FUNCTION STORED.
     ************************************************************************
     */ 
        Node* argument = arguments->node;
        char* arg_name = argument->name;
        char* curr_register = registers[index];
        // Push argument onto the stack
        LongToCharOffset();
        // Update arg info on variable list
        AddVarInfo(arg_name, lastOffsetUsed, INVAL, false);
        fprintf(fptr, "\nmovq %s, %s", curr_register, lastOffsetUsed);
        // Update register availability
        UpdateRegInfo(curr_register, 1);
        arguments = arguments->next;
        index ++;
    }
}


/*
*************************************************************************
  THIS FUNCTION IS MEANT TO GET THE FUNCTION ARGUMENTS FROM THE  STACK
**************************************************************************
*/
void PopArgFromStack(NodeList* arguments) {
    int counter = 0;
    while (arguments != NULL) {
    /*
     ***********************************************************************
              TODO : YOUR CODE HERE
      THINK ABOUT WHERE EACH ARGUMENT COMES FROM. EXAMPLE WHERE IS THE
      FIRST ARGUMENT OF A FUNCTION STORED AND WHERE SHOULD IT BE EXTRACTED
      FROM AND STORED TO..
     ************************************************************************
     */
        Node* arg = arguments->node;
        char* curr_register = registers[counter];
        if (arg->exprCode == CONSTANT)
        {
            fprintf(fptr, "\nmovq $%li, %s", arg->value, curr_register);
        }
        else
        {
            char* arg_location = Var_Present(arg->name);
            fprintf(fptr, "\nmovq %s, %s", arg_location, curr_register);
        }
        UpdateRegInfo(curr_register, 0);
        counter ++;

        arguments = arguments->next;
    }
}

/*
 ***********************************************************************
  THIS FUNCTION IS MEANT TO PROCESS EACH CODE STATEMENT AND GENERATE 
  ASSEMBLY FOR IT. 
  TIP: YOU CAN MODULARIZE BETTER AND ADD NEW SMALLER FUNCTIONS IF YOU 
  WANT THAT CAN BE CALLED FROM HERE.
 ************************************************************************
 */  
void ProcessStatements(NodeList* statements) {
    while (statements != NULL) {
        Node* statement = statements->node;
        // Process ASSIGN statements
        if (statement->stmtCode == ASSIGN)
        {
            Node* right_side = statement->right;
            char* var_name = statement->name;
            // Allocate space for resulting variable
            LongToCharOffset();
            // CONSTANT assignment
            if (right_side->exprCode == CONSTANT)
            {
                long var_value = right_side->value;
                fprintf(fptr, "\nmovq $%li, %s", var_value, lastOffsetUsed);
                UpdateVarInfo(var_name, lastOffsetUsed, var_value, true);
            }
            // plain VARIABLE assignment
            else if (right_side->exprCode == VARIABLE || right_side->exprCode == PARAMETER)
            {
                char* variable_name = right_side->name;
                // Assume that the variable is already on the stack
                char* variable_location = Var_Present(variable_name);
                fprintf(fptr, "\nmovq %s, %s", variable_location, lastOffsetUsed);
            }
            
            // OPERATION
            else if (right_side->exprCode == OPERATION)
            {
                // **FUNCTION CALL**
                if (right_side->opCode == FUNCTIONCALL)
                {
                    // Move arguments to respective registers
                    PopArgFromStack(right_side->arguments);
                    // Call function
                    fprintf(fptr, "\ncall %s", right_side->left->name);
                    // Store returned value
                }
                // UNARY operation
                else if (right_side->opCode == NEGATE)
                {
                    // VARIABLE case
                    Node* operand = right_side->left;
                    if (operand->exprCode == VARIABLE || operand->exprCode == PARAMETER)
                    {
                        char* operand_name = operand->name;
                        // Assume that the variable is already on the stack
                        char* operand_location = Var_Present(operand_name);
                        // Move variable address to next available register
                        fprintf(fptr, "\nmovq %s, %rax", operand_location);
                        fprintf(fptr, "\nnegq %rax");
                        fprintf(fptr, "\nmovq %rax, %s", operand_location, lastOffsetUsed);
                        UpdateVarInfo(var_name, lastOffsetUsed, INVAL, false);
                    }
                }
                // BINARY OPERATION
                else
                {
                    char* left_operand_location, *right_operand_location;
                    Node* left_operand = right_side->left;
                    Node* right_operand = right_side->right;
                    // Handle DIVIDE separately
                    if (right_side->opCode == DIVIDE)
                    {
                        fprintf(fptr, "\ncqto");
                        // Move dividend into %rax
                        // Dividend is a CONSTANT
                        if (left_operand->exprCode == CONSTANT)
                        {
                            fprintf(fptr, "\nmovq $%li, %rax", left_operand->value);
                        }
                        else // Dividend is a VARIABLE/PARAMETER
                        {
                            left_operand_location = Var_Present(left_operand->name);
                            fprintf(fptr, "\nmovq %s, %rax", left_operand_location);
                        }
                    
                        // Move divisor (VARIABLE/CONSTANT/PARAMETER) into %rcx register 
                        if (right_operand->exprCode == CONSTANT)
                        {
                            fprintf(fptr, "\nmovq $%li, %rcx", right_operand->value);
                        }
                        else // Divisor is a VARIABLE/PARAMETER
                        {
                            right_operand_location = Var_Present(right_operand->name);
                            fprintf(fptr, "\nmovq %s, %rcx", right_operand_location);
                        }
                        // Perform division
                        fprintf(fptr, "\nidivq %rcx", right_operand->value);
                        // Move result to stack memory
                        fprintf(fptr, "\nmovq %rax, %s", lastOffsetUsed);
                    }
                    else
                    {
                        // Perform binary operation
                        Operate_binary(right_side->opCode, left_operand, right_operand);   
                    }
                }
            }
            // Move result to stack memory
            fprintf(fptr, "\nmovq %rax, %s", lastOffsetUsed);
            // Update variable on the left
            UpdateVarInfo(var_name, lastOffsetUsed, INVAL, false);
        }
        // RETURN statement
        else
        {
            Node* return_node = statement->left;
            // Return Variable
            if(return_node->exprCode != CONSTANT)
            {
                // Get location of VARIABLE/PARAMETER
                char* var_location = Var_Present(return_node->name);
                // Move return value to %rax
                fprintf(fptr, "\nmovq %s, %rax", var_location);
            }
            else // Return CONSTANT
            {
                long var_value = return_node->value;
                // Move return value to %rax
                fprintf(fptr, "\nmovq $%li, %rax", var_value);   
            }
            // Clear local variables
            fprintf(fptr, "\nmovq %rbp, %rsp");
        }
        
        statements = statements->next;
    }
}

/*
 ***********************************************************************
  THIS FUNCTION IS MEANT TO DO CODEGEN FOR ALL THE FUNCTIONS IN THE FILE
 ************************************************************************
*/
void Codegen(NodeList* worklist) {
    fptr = fopen("assembly.s", "w+");
    
    if (fptr == NULL) {
        printf("\n Could not create assembly file"); 
        return; 
    }
    while (worklist != NULL) {
        Node* funcdecl = worklist->node;
        NodeList* func_arguments = funcdecl->arguments;
        NodeList* func_statements = funcdecl->statements;
        // Initialize assembly file
        InitAsm(funcdecl->name);
        // Create register list
        CreateRegList();
        // Count variables used in function declaration
        CountVars(funcdecl);
        // Assign arguments to registers and push arguments to the stack
        PushArgOnStack(func_arguments);
        // Process statements sequentially
        ProcessStatements(func_statements);
        // Print returning code to assembly file
        RetAsm();
        
        worklist = worklist->next; 
    }
    fclose(fptr);
}

/*
**********************************************************************************************************************************
 YOU CAN MAKE ADD AUXILLIARY FUNCTIONS BELOW THIS LINE. DO NOT FORGET TO DECLARE THEM IN THE HEADER
**********************************************************************************************************************************
*/
/**
 * Look up variables by their name only
*/
char* Var_Present(char* name){
    varList = varLast;
    while (varList != NULL) 
    {
        char* var_name = varList->varName;
        if (strcmp(name, var_name) == 0)
        {
            return varList->location;
        }
        varList = varList->prev;
    }
    varList = varHead;
    return "";
}

/**
 * Count number of UNIQUE variables used in a function
 * Update VarList accordingly
*/
void CountVars(Node* funcdecl){
    Node* statement, *right_side, *temp_node;
    NodeList* temp;
    NodeList* statements = funcdecl->statements;
    int counter = 0;
    while (statements != NULL)
    {
        statement = statements->node;
        // Account for constants passed as function arguments
        if (statement->stmtCode == ASSIGN)
        {
            right_side = statement->right;
            if (right_side->opCode == FUNCTIONCALL)
            {
                // Check for CONSTANTS in arguments
                temp = right_side->arguments;
                while (temp != NULL)
                {
                    temp_node = temp->node;
                    if (temp_node->exprCode == CONSTANT)
                    {
                        counter ++;
                    }
                    temp = temp->next;
                }
            }
        }
        counter ++;
        statements = statements->next;
    }
    // Subtract return statement
    varCounter = (counter - 1);
}
/**
 * Print out assembly code for respective operations
*/
void Operate_binary(OpType operation_type, Node* left_op, Node* right_op){
    char* left_operand_location, *right_operand_location;
    long left_operand_val, right_operand_val;
    /**
     * 0 = both VARIABLES/PARAMETERS
     * 1 = left is VARIABLE/PARAMETER; right is a CONSTANT
     * 2 = right is VARIABLE/PARAMETER; left is a CONSTANT
    */
    int flag;
    // Perform necessary movements from stack to %rax & %rcx registers
    // Both are VARIABLES
    if ((left_op->exprCode == VARIABLE || left_op->exprCode == PARAMETER) && (right_op->exprCode == VARIABLE || right_op->opCode == PARAMETER))
    {
        flag = 0;
        // Assume that the variable is already on the stack
        left_operand_location = Var_Present(left_op->name);
        right_operand_location = Var_Present(right_op->name);
        // Move both variables to registers
        fprintf(fptr, "\nmovq %s, %rax", left_operand_location);
        fprintf(fptr, "\nmovq %s, %rcx", right_operand_location);
    }
    // Left_op is CONSTANT and right_op is VARIABLE/PARAMETER
    else if (left_op->exprCode == CONSTANT && (right_op->exprCode == VARIABLE || right_op->opCode == PARAMETER))
    {
        flag = 2;
        left_operand_val = left_op->value;
        // Only move right_op
        right_operand_location = Var_Present(right_op->name);
        fprintf(fptr, "\nmovq %s, %rax", right_operand_location);
    }
    else if(right_op->exprCode == CONSTANT && (left_op->exprCode == VARIABLE || left_op->exprCode == PARAMETER))
    {
        flag = 1;
        right_operand_val = right_op->value;
        // Only move left_op
        left_operand_location = Var_Present(left_op->name);
        fprintf(fptr, "\nmovq %s, %rax", left_operand_location);
    }
    
    // Handle operations
    switch (operation_type)
    {
    case ADD:
        if (flag == 0)
        {
            fprintf(fptr, "\naddq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\naddq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\naddq $%li, %rax", left_operand_val);
        }
        break;
    case SUBTRACT:
        if (flag == 0)
        {
            fprintf(fptr, "\nsubq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\nsubq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\nsubq $%li, %rax", left_operand_val);
        }
        break;
    case MULTIPLY:
        if (flag == 0)
        {
            fprintf(fptr, "\nimulq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\nimulq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\nimulq $%li, %rax", left_operand_val);
        }
        break;
    case BXOR:
        if (flag == 0)
        {
            fprintf(fptr, "\nxorq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\nxorq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\nxorq $%li, %rax", left_operand_val);
        }
        break;
    case BAND:
        if (flag == 0)
        {
            fprintf(fptr, "\nandq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\nandq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\nandq $%li, %rax", left_operand_val);
        }
        break;
    case BOR:
        if (flag == 0)
        {
            fprintf(fptr, "\norq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\norq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\norq $%li, %rax", left_operand_val);
        }
        break;
    case BSHR:
        if (flag == 0)
        {
            fprintf(fptr, "\nsarq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\nsarq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\nsarq $%li, %rax", left_operand_val);
        }
        break;
    case BSHL:
        if (flag == 0)
        {
            fprintf(fptr, "\nsalq %rcx, %rax");
        }
        else if(flag == 1)
        {
            fprintf(fptr, "\nsalq $%li, %rax", right_operand_val);
        }
        else if (flag == 2)
        {
            fprintf(fptr, "\nsalq $%li, %rax", left_operand_val);
        }
        break;
    default:
        break;
    }
}
/*
**********************************************************************************************************************************
 YOU CAN MAKE ADD AUXILLIARY FUNCTIONS ABOVE THIS LINE. DO NOT FORGET TO DECLARE THEM IN THE HEADER
**********************************************************************************************************************************
*/


