#ifndef BITFIELDS_H
#define BITFIELDS_H
#include "structbool.h"
#include "istype.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define VARSIZE_ARRAY(type,name) union { \
	type name[0]; \\
	type varsize_arr[0]; \\
\}
#define SIZEOF_VARSIZE_PTR(obj) (sizeof(*obj) + sizeof(obj->varsize_arr) * sizeof(obj->varsize_arr[0]) )


#endif
