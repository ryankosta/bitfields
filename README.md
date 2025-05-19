# Bitfields
## Introduction
basically just a library I made for fun after dealing with a lot of MMIO bitfields and wondering what crazy C tricks I could use to do it better :)

### Benefits
- Gauranteed ordering
- Easy to describe access semantics and bit layout
- Comparable to manual bit manipulation (with -O2 turned on)
- Enforces Big Endian/Little endian swaps (depending on type settings)
- Enforces volatile (depending on type settings)
- Operations like Malloc/memcpy/free still work
- can optionally overlay the bitfields with a regular struct
### Downsides
- uses some new compiler features
    - types compatable: `__builtin_types_compatible_p` 
- hacky use of preprocessor and macros (see `structbool.h`)
- uses compiler features which are not in standard but are allowed in gcc/clang
- designed for 64 bit architectures which tolerate 8/16/32/64 bit accesses 
- not yet tested much (will probably test more if/when I decide to use it)

