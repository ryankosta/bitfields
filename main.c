#include <stdio.h>

#include "bitfields.h"

/* no bit word min, volatile, little endian */
DECLARE_BIT_TYPE(intel_mmio_bit_t, 1, 1, 0, 0, 0);
DECLARE_BIT_TYPE(intel_mmio_flag_t, 1, 1, 1, 0, 0);

/* 32 bit word min, volatile, big endian */
DECLARE_BIT_TYPE(mlx_mmio_bit_t, 0, 1, 0, 1, 0);
DECLARE_BIT_TYPE(mlx_mmio_flag_t, 0, 1, 1, 1, 0);

/* no bit word min, nonvolatile, big endian */
DECLARE_BIT_TYPE(mlx_dma_bit_t, 0, 0, 0, 0, 0);
DECLARE_BIT_TYPE(mlx_dma_flag_t, 0, 0, 1, 0, 0);

DECLARE_BITS(struct idt_entry) {
	intel_mmio_bit_t offset_lo[16];
	intel_mmio_bit_t seg_select[16];
	intel_mmio_bit_t reserved[8];
	intel_mmio_bit_t gate[4];
	intel_mmio_bit_t unused;
	intel_mmio_bit_t dpl[2];
	intel_mmio_flag_t p;
	intel_mmio_bit_t offset_hi[16];
}
__attribute__((packed));

INFER_DECLARE_BYTES(struct idt_entry)
CHECK_BITS(struct idt_entry)
int main(void) {
	struct idt_entry idt = {0};
	write_bitfield_typed(&idt, struct idt_entry, dpl, 3);
	uint8_t result = read_bitfield_typed(&idt, struct idt_entry, dpl, 3);
	printf("result %d\n", result);
	return 0;
}
