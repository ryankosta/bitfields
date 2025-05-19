#ifndef BITFIELDS_H
#define BITFIELDS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bitfield_types.h"
#include "panic.h"
#include "structbool.h"
//#include <stdio.h> //todo debug only
//#include <inttypes.h> //todo debug only
#define GEN_BITMASK(width) ((1ULL << (width)) - 1ULL)
#define ACCESS_ONCE(x) (*(volatile __typeof__(x) *)&(x))
#define MAX(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; \
})
/* main accessor */
#define declare_read_bitfield_offset(bits) \
static inline __attribute__((always_inline)) uint##bits##_t _read_bitfield_offset_##bits(uint##bits##_t *data, uint8_t offbits, uint8_t szbits){ \
	return (*data >> (offbits % bits)) & GEN_BITMASK(szbits); \
}
#define declare_read_bitfield_offset_volatile(bits) \
static inline __attribute__((always_inline)) uint##bits##_t _read_bitfield_offset_##bits##_volatile(uint##bits##_t *data, uint8_t offbits, uint8_t szbits){ \
	return (ACCESS_ONCE(*data) >> (offbits % bits)) & GEN_BITMASK(szbits); \
}



/* main accessor */
#define declare_write_bitfield_offset(bits) \
static inline __attribute__((always_inline)) void _write_bitfield_offset_##bits( uint##bits##_t *data, uint##bits##_t val, uint8_t offbits, uint8_t szbits){ \
	uint##bits##_t rest = 0; \
	if(szbits != bits) rest = *data & ~GEN_BITMASK(szbits); \
	*data = (rest & GEN_BITMASK(szbits)) | (val << (offbits % bits)); \
}

/* main accessor (volatile variant) */
#define declare_write_bitfield_offset_volatile(bits) \
static inline __attribute__((always_inline)) void _write_bitfield_offset_##bits##_volatile(volatile uint##bits##_t *data, uint##bits##_t val, uint8_t offbits, uint8_t szbits){ \
	uint##bits##_t rest = 0; \
	if(szbits != bits) ACCESS_ONCE(rest) = *data & ~GEN_BITMASK(szbits); \
	ACCESS_ONCE(*data) = (rest & GEN_BITMASK(szbits)) | (val << (offbits % bits)); \
}

/* declarations of accesors from macro templates */
declare_read_bitfield_offset(8)
declare_read_bitfield_offset(16)
declare_read_bitfield_offset(32)
declare_read_bitfield_offset(64)

declare_read_bitfield_offset_volatile(8)
declare_read_bitfield_offset_volatile(16)
declare_read_bitfield_offset_volatile(32)
declare_read_bitfield_offset_volatile(64)

declare_write_bitfield_offset(8)
declare_write_bitfield_offset(16)
declare_write_bitfield_offset(32)
declare_write_bitfield_offset(64)

declare_write_bitfield_offset_volatile(8)
declare_write_bitfield_offset_volatile(16)
declare_write_bitfield_offset_volatile(32)
declare_write_bitfield_offset_volatile(64)

/* does access tear across alignment boundary */
#define tear(offbits,sz,divisor) offbits/divisor != (offbits+sz)/divisor
static inline __attribute__((always_inline)) uint64_t _read_bitfield(uint8_t *base, uint64_t offbits, uint64_t sz, bool swap, bool vol,uint8_t accessmin){
	uint64_t val;
	base += offbits/8;
	if(MAX(sz,accessmin) <= 8 && !tear(offbits,sz,8llu)){ 
		if(vol){
			val = _read_bitfield_offset_8_volatile((void*)base,offbits,sz); 
		}else{
			val = _read_bitfield_offset_8((void*)base,offbits,sz); 
		}
	} else if(MAX(sz,accessmin) <= 16 && !tear(offbits,sz,16llu)){ 
		if(vol){
			val = _read_bitfield_offset_16_volatile((void*)base,offbits,sz);
		}else {
			val = _read_bitfield_offset_16((void*)base,offbits,sz);
		}
		if(swap) val = __builtin_bswap16(val);
	} else if(MAX(sz,accessmin) <= 32 && !tear(offbits,sz,32llu)){ 
		if(vol){
			val = _read_bitfield_offset_32_volatile((void*)base,offbits,sz);
		}else {
			val = _read_bitfield_offset_32((void*)base,offbits,sz);
		}
		if(swap) val = __builtin_bswap32(val);
	} else if(MAX(sz,accessmin) <= 64 && !tear(offbits,sz,64llu)){
		if(vol){
			val = _read_bitfield_offset_64_volatile((void*)base,offbits,sz);
		}else {
			val = _read_bitfield_offset_64((void*)base,offbits,sz);
		}
		if(swap) val = __builtin_bswap64(val);
	} else {
		panic("split read unimplemented\n");
	}
	return val;
}
static inline __attribute__((always_inline)) void _write_bitfield(uint8_t *base, uint64_t val, uint64_t offbits, uint64_t sz, bool swap, bool vol,uint8_t accessmin){
	base += offbits/8;
	if(MAX(sz,accessmin) <= 8 && !tear(offbits,sz,8llu)){ 
		if(vol){
			_write_bitfield_offset_8_volatile((void*)base,val,offbits,sz); 
		}else{
			_write_bitfield_offset_8((void*)base,val,offbits,sz); 
		}
	}else if(MAX(sz,accessmin) <= 16 && !tear(offbits,sz,16llu)){ 
		if(swap) val = __builtin_bswap16(val);
		if(vol){
			_write_bitfield_offset_16_volatile((void*)base,val,offbits,sz);
		}else {
			_write_bitfield_offset_16((void*)base,val,offbits,sz);
		}
	}else if(MAX(sz,accessmin) <= 32 && !tear(offbits,sz,32llu)){ 
		if(swap) val = __builtin_bswap32(val);
		if(vol){
			_write_bitfield_offset_32_volatile((void*)base,val,offbits,sz);
		}else {
			_write_bitfield_offset_32((void*)base,val,offbits,sz);
		}
	}else if(MAX(sz,accessmin) <= 64 && !tear(offbits,sz,64llu)){ 
		if(swap) val = __builtin_bswap64(val);
		if(vol){
			_write_bitfield_offset_64_volatile((void*)base,val,offbits,sz);
		}else {
			_write_bitfield_offset_64((void*)base,val,offbits,sz);
		}
	} else {
		panic("split write unimplemented\n");
	}
}
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define AM_LE 1
#define AM_BE 0
#else
#define AM_LE 0
#define AM_BE 1
#endif

#define write_bitfield_typed(obj, type, member, value) do { \
    TYPE_TO_BITS(type) *unused __attribute__((unused)); \
    _Static_assert(!type_is_flag(unused->member),"requires not flag"); \
    _write_bitfield( \
    (uint8_t *)(obj), \
    (value), \
    offsetof(TYPE_TO_BITS(type), member), \
    sizeof(unused->member), \
    is_cpu_type(unused->member), \
    type_is_vol(unused->member), \
    type_minword(unused->member) \
);} while(0)
#define write_bitfield_typed_vol(obj, type, member, value) do { \
    TYPE_TO_BITS(type) *unused __attribute__((unused)); \
    _Static_assert(!type_is_flag(unused->member),"requires not flag"); \
    _write_bitfield( \
    (uint8_t *)(obj), \
    (value), \
    offsetof(TYPE_TO_BITS(type), member), \
    sizeof(unused->member), \
    is_cpu_type(unused->member), \
    1, \
    type_minword(unused->member) \
);} while(0)
#define read_bitfield_typed(obj, type, member, value) ({\
    TYPE_TO_BITS(type) *unused __attribute__((unused)); \
    _Static_assert(!type_is_flag(unused->member),"requires not flag"); \
    _read_bitfield( \
    (uint8_t *)(obj), \
    offsetof(TYPE_TO_BITS(type), member), \
    sizeof(unused->member), \
    is_cpu_type(unused->member), \
    type_is_vol(unused->member), \
    type_minword(unused->member) \
);}) 
#define read_bitfield_typed_vol(obj, type, member, value) ({ \
    TYPE_TO_BITS(type) *unused __attribute__((unused)); \
    _Static_assert(type_is_flag(unused->member),"requires not flag"); \
    _read_bitfield( \
    (uint8_t *)(obj), \
    offsetof(TYPE_TO_BITS(type), member), \
    sizeof(unused->member), \
    is_cpu_type(unused->member), \
    1, \
    type_minword(unused->member) \
);})
static inline __attribute__((always_inline)) void _set_flag(uint8_t *data, uint8_t offbits){
	*data |= 1 << offbits;
}
static inline __attribute__((always_inline)) void _clear_flag(uint8_t *data, uint8_t offbits){
	*data &= ~(1 << offbits);
}
static inline __attribute__((always_inline)) void _set_flag_volatile(volatile uint8_t *data, uint8_t offbits){
	ACCESS_ONCE(*data) = ACCESS_ONCE(*data) | (1 << offbits);
}
static inline __attribute__((always_inline)) void _clear_flag_volatile(volatile uint8_t *data, uint8_t offbits){
	ACCESS_ONCE(*data) = ACCESS_ONCE(*data) & ~(1 << offbits);
}
#define set_flag_typed(obj, type, member, value) do { \
	TYPE_TO_BITS(type) *unused __attribute__((unused)); \
	_Static_assert(type_is_flag(unused->member),"requires flag"); \
	if(type_is_vol(unused->member)) _set_flag_volatile((uint8_t *)obj, sizeof(unused->member)); \
	else _set_flag((uint8_t *)obj, sizeof(unused->member)); \
    }while(0)
#define clear_flag_typed(obj, type, member, value) do { \
	TYPE_TO_BITS(type) *unused __attribute__((unused)); \
	_Static_assert(type_is_flag(unused->member),"requires flag"); \
	if(type_is_vol(unused->member)) _clear_flag_volatile((uint8_t *)obj, sizeof(unused->member)); \
	else _clear_flag((uint8_t *)obj, sizeof(unused->member)); \
    }while(0)


#endif
