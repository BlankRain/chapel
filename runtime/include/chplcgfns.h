#ifndef _CHPL_GEN_INTERFACE_H_
#define _CHPL_GEN_INTERFACE_H_

#include <stdlib.h>
#include "chpltypes.h"

/* This header file is for routines that are in the generated code */

extern int32_t chpl_numRealms;

/* used for entry point: */
extern void chpl_main(void);

/* used for config vars: */
extern void CreateConfigVarTable(void);

/* used by copy collection: */

/* If we're not in the generated code, define an enum _class_id; */
#ifndef CHPL_GEN_CODE
typedef enum { chpl__cid_bogus } chpl__class_id;
#endif

extern size_t cid2size(chpl__class_id cid);
extern size_t* cid2offsets(chpl__class_id cid);

/* These are defined in _type_structure.c if
   --gen-communicated-structures is true and are used by a
   communication layer to query types of communicated buffers */
extern chplType chpl_getFieldType(int typeNum, int fieldNum);
extern size_t chpl_getFieldOffset(int typeNum, int fieldNum);

#endif
