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
### TODO
- test more
- Implement reads/writes split across alignment boundaries (ie: via multiple reads/writes)
- Implement better methods to describe MMIO which is Read only/write only or has different definitions of read vs write
    - Add option to provide backing store for writes
## Example
### Intel Interrupt Descriptor Table Entry 
``` c

/* no read/write size min, nonvolatile, little endian */
DECLARE_BIT_TYPE(intel_dma_bit_t,1,0,0,0,0);
DECLARE_BIT_TYPE(intel_dma_flag_t,1,0,1,0,0);

/* no read/write size min, volatile, little endian */
DECLARE_BIT_TYPE(intel_mmio_bit_t,1,0,0,0,0);
DECLARE_BIT_TYPE(intel_mmio_flag_t,1,0,1,0,0);

/* define bitfields for idt entry */
DECLARE_BITS(struct idt_entry) {
	intel_dma_bit_t offset_lo[16];
	intel_dma_bit_t seg_select[16];
	intel_dma_bit_t reserved[8];
	intel_dma_bit_t gate[4];
	intel_dma_bit_t unused;
	intel_dma_bit_t dpl[2];
	intel_dma_flag_t p;
	intel_dma_bit_t offset_hi[16];
} __attribute__((packed));

/* define a default opaque struct for idt entry */
INFER_DECLARE_BYTES(struct idt_entry);
/* ensure bit field is same length as byte struct 
 * (gauranteed when using INFER_DECLARE_BYTES) */
CHECK_BITS(struct idt_entry);

void update_idt_privilige_level(struct idt_entry *entry){
    uint8_t old_val = read_bitfield_typed(entry, struct idt_entry,dpl);
    /* write 0 (ring 0) to descriptor privilige level field */
    write_bitfield_typed(entry, struct idt_entry,dpl,0);
    printf("previous value of idt dpl was %d\n", old_val);
}
```
