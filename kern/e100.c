// LAB 6: Your driver code here

#include <inc/x86.h>
#include <kern/pci.h>
#include <kern/e100.h>

static uint32_t e100_io_addr;
static uint32_t e100_io_size;
static uint8_t e100_irq_line;

static void
delay(void)
{
	inb(0x84);
	inb(0x84);
	inb(0x84);
	inb(0x84);
}

int
pci_attach_e100(struct pci_func *pcif)
{
	int i;

	pci_func_enable(pcif);
	e100_io_addr = pcif->reg_base[1];
	e100_io_size = pcif->reg_base[1];
	e100_irq_line = pcif->irq_line;

	outl(e100_io_addr + 8, 0);
	delay();
	delay();

	return 1;
}
