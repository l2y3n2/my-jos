// LAB 6: Your driver code here

#include <inc/x86.h>
#include <inc/string.h>
#include <kern/pmap.h>
#include <kern/pci.h>
#include <kern/e100.h>

static uint32_t e100_io_addr;
static uint32_t e100_io_size;
static uint8_t e100_irq_line;

struct cb {
	volatile uint16_t status;
	uint16_t cmd;
	uint32_t link;
	uint32_t tbd;
	uint16_t count;
	uint16_t flags;
	uint8_t data[ETH_PACK_SIZE];
	struct cb *next;
	struct cb *prev;
} __attribute__((aligned(4), packed));

static struct cb cbs[SHM_LENGTH];
static struct cb *cb;

static void
delay(void)
{
	inb(0x84);
	inb(0x84);
	inb(0x84);
	inb(0x84);
}

static void
shm_init(void)
{
	int i;

	for (i = 0; i < SHM_LENGTH; i++) {
		cbs[i].cmd = 0;
		cbs[i].status = CB_STATUS_C;
		cbs[i].next = cbs + (i + 1) % SHM_LENGTH;
		cbs[i].prev = (i == 0) ? cbs + SHM_LENGTH - 1 : cbs + i - 1;
		cbs[i].link = PADDR(cbs[i].next);
	}
	cbs[0].cmd = CB_CONTROL_NOP | CB_CONTROL_S;

	outl(e100_io_addr + 4, PADDR(cbs));
	outw(e100_io_addr + 2, SCB_CMD_CU_START);

	cb = cbs + 1;
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
	shm_init();

	return 1;
}

static int
e100_pack_cb(void *data, size_t count)
{
	if (!(cb->status & CB_STATUS_C))
		return 0;

	if (count > ETH_PACK_SIZE)
		count = ETH_PACK_SIZE;

	cb->cmd = CB_CONTROL_S | CB_CONTROL_TX;
	cb->status = 0;
	cb->tbd = 0xffffffff;
	cb->flags = 0xe0;
	cb->count = count;
	memmove(cb->data, data, count);

	cb->prev->cmd &= ~CB_CONTROL_S;

	return count;
}

int
e100_send_data(void *data, size_t count)
{
	int ret, sent = 0;

	while (sent != count) {
		ret = e100_pack_cb(data + sent, count - sent);
		if (ret == 0)
			break;
		sent += ret;
		cb = cb->next;
	}

	if (sent != 0 && (inw(e100_io_addr) & SCB_STATUS_CU_MASK) == CU_SUSPEND)
		outw(e100_io_addr + 2, SCB_CMD_CU_RESUME);

	return sent;
}

int
e100_recv_data(void *data, size_t count)
{
	return 0;
}
