#include "ns.h"

#define N_FRAMES	2
#define FRAME_VA	(0x10000000 - N_FRAMES * PGSIZE)

static union Nsipc *frames[N_FRAMES];
static int fid;

static void
recv_frame(void)
{
	int ret;

	while (1) {
		ret = sys_recv(frames[fid]->pkt.jp_data, PGSIZE - sizeof(int));
		if (ret != 0) {
			frames[fid]->pkt.jp_len = ret;
			break;
		}
		sys_yield();
	}
}

void
input(envid_t ns_envid)
{
	int i, r;

	binaryname = "ns_input";

	// LAB 6: Your code here:
	// 	- read a packet from the device driver
	//	- send it to the network server
	// Hint: When you IPC a page to the network server, it will be
	// reading from it for a while, so don't immediately receive
	// another packet in to the same physical page.
	fid = 0;
	for (i = 0; i < N_FRAMES; i++) {
		r = sys_page_alloc(0, (void *) FRAME_VA + i * PGSIZE,
				PTE_P | PTE_U | PTE_W);
		if (r < 0)
			panic("sys_page_alloc: %e", r);

		frames[i] = (void *) FRAME_VA + i * PGSIZE;
	}

	while (1) {
		recv_frame();
		ipc_send(ns_envid, NSREQ_INPUT, frames[fid], PTE_P | PTE_U);
		fid++;
		fid %= N_FRAMES;
	}
}
