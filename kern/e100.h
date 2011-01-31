#ifndef JOS_KERN_E100_H
#define JOS_KERN_E100_H

#include <kern/pci.h>

#define SHM_LENGTH	16

#define SCB_STATUS_CU_MASK	(3 << 6)
#define CU_IDLE			(0 << 6)
#define CU_SUSPEND		(1 << 6)
#define CU_LPQ			(2 << 6)
#define CU_HQP			(3 << 6)

#define SCB_STATUS_RU_MASK	(0xF << 2)
#define RU_IDLE			(0 << 2)
#define RU_SUSPEND		(1 << 2)
#define RU_NO_RESOURCE		(2 << 2)
#define RU_READY		(4 << 2)

#define SCB_CMD_CU_START	(1 << 4)
#define SCB_CMD_CU_RESUME	(2 << 4)
#define SCB_CMD_RU_START	(1)
#define SCB_CMD_RU_RESUME	(2)

#define CB_STATUS_C	(1 << 15)
#define CB_STATUS_OK	(1 << 13)
#define CB_CONTROL_EL	(1 << 15)
#define CB_CONTROL_S	(1 << 14)
#define CB_CONTROL_I	(1 << 13)
#define CB_CONTROL_NOP	(0)
#define CB_CONTROL_TX	(4)
#define RFD_STATUS_C	(1 << 15)
#define RFD_STATUS_OK	(1 << 13)
#define RFD_CONTROL_EL	(1 << 15)
#define RFD_CONTROL_S	(1 << 14)
#define RFD_CONTROL_H	(1 << 4)
#define RFD_CONTROL_SF	(1 << 3)
#define RFD_FLAGS_EOF	(1 << 15)
#define RFD_FLAGS_F	(1 << 14)
#define RFD_SIZE_MASK	((1 << 14) - 1)

#define ETH_PACK_SIZE	1518

int pci_attach_e100(struct pci_func *pcif);

int e100_send_data(void *data, size_t count);
int e100_recv_data(void *data, size_t count);

#endif	// JOS_KERN_E100_H
