#ifndef STRUCTBOOL_H
#define STRUCTBOOL_H
#include <stdbool.h>
#include "istype.h"
typedef struct{
	int vol[0];
} true_t;
_Static_assert(sizeof(true_t) == 0, "true type must have zero size\n");
typedef struct{
	int vol[0];
} false_t;
_Static_assert(sizeof(false_t) == 0, "false type must have zero size\n");

#define BOOL_TYPE0 false_t
#define BOOL_TYPE1 true_t
/* double ref so macro true evals to 1 and macro false evals to 0 */
#define _BOOL_TYPE(cond) BOOL_TYPE##cond
#define BOOL_TYPE(cond) _BOOL_TYPE(cond)
_Static_assert(istype(true_t,BOOL_TYPE(true)),"stdbool to structbool works\n");
_Static_assert(istype(false_t,BOOL_TYPE(false)),"stdbool to structbool works\n");


/* check cond */
#define struct_check_cond(cond) istype(cond,true_t)
#define struct_check_cond_member(cond) istype(__typeof__(cond),true_t)
_Static_assert(struct_check_cond(true_t),"true type is true\n");
_Static_assert(!struct_check_cond(false_t),"false type is false\n");

/* declare member/object of type cond (cond must be known at compile time) */
#define DECLARE_BOOL(name,cond) BOOL_TYPE##cond name

struct structbool_tmp_test{
	//TODO still a little finicky with true/false
	DECLARE_BOOL(should_be_true,1);
	DECLARE_BOOL(should_be_false,0);
};


#endif
