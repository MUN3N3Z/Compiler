#include "optimizer.h"


void Optimizer(NodeList *funcdecls) {
     // INCOMPLETE //
     while(true)
     {
          if (!ConstProp(funcdecls) && !ConstantFolding(funcdecls) && !DeadAssign(funcdecls))
          {
               break;
          }
     }
     
}

// One line?
// Order of optimizations?