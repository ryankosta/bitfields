#ifndef ISTYPE_H
#define ISTYPE_H
#define istype(obj, type)  __builtin_types_compatible_p(__typeof__(obj), type)
#endif
