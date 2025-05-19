#ifndef BITFIELD_TYPES_H
#define BITFIELD_TYPES_H
#include "structbool.h"
#include "istype.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define DECLARE_BIT_TYPE(name,le,vol,flag,word32,word64) typedef struct __attribute__((packed)) {uint8_t v; \
       	DECLARE_BOOL(is_vol,vol); \
	DECLARE_BOOL(is_le,le); \
	DECLARE_BOOL(is_flag,flag); \
	DECLARE_BOOL(is_word32,word32); \
	DECLARE_BOOL(is_word64,word64); \
	} name

#define type_is_le(obj) struct_check_cond(obj->is_le)
#define type_is_be(obj) (!struct_check_cond(obj->is_le))
#define type_is_vol(obj) (struct_check_cond(obj->is_vol))
#define type_is_flag(obj) (struct_check_cond(obj->is_flag))
#define type_is_word32(obj) (struct_check_cond(obj->is_word32))
#define type_is_word64(obj) (struct_check_cond(obj->is_word64))
#define type_minword(obj) (type_is_word64(obj) ? 64 : (type_is_word32(obj) ? 32 : 8))
#define is_cpu_type(member) ((type_is_le(member)&& AM_LE)||(type_is_be(member) && AM_BE))

DECLARE_BIT_TYPE(le_bit_t,1,0,0,0,0);
DECLARE_BIT_TYPE(vol_le_bit_t,1,1,0,0,0);

DECLARE_BIT_TYPE(be_bit_t,0,0,0,0,0);
DECLARE_BIT_TYPE(vol_be_bit_t,0,1,0,0,0);

DECLARE_BIT_TYPE(bit_flag_t,0,0,1,0,0);
DECLARE_BIT_TYPE(vol_bit_flag_t,0,1,1,0,0);


#define TYPE_TO_BITS(type) type##_bits

#define DECLARE_BITS(name) name##_bits

#define INFER_DECLARE_BYTES(name) \
	name { \
		uint8_t v[sizeof(TYPE_TO_BITS(name))/8]; \
	}__attribute__((packed));

#define CHECK_BITS(type) \
	_Static_assert((sizeof(TYPE_TO_BITS(type)) % 8) == 0, "bit struct must be byte aligned\n"); \
	_Static_assert((sizeof(TYPE_TO_BITS(type)) / 8) == sizeof(type),"bits must have same length as bytes");


#endif
