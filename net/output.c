#include "ns.h"

#define TIMEOUT_TICKS		20

extern union Nsipc nsipcbuf;

static void
send_frame(void)
{
	int ret, sent = 0, counter = 0;

	while (sent != nsipcbuf.pkt.jp_len && counter < TIMEOUT_TICKS) {
		ret = sys_send(nsipcbuf.pkt.jp_data + sent,
				nsipcbuf.pkt.jp_len - sent);
		if (ret == 0) {
			counter++;
			sys_yield();
		}
		sent += ret;
	}
}

void
output(envid_t ns_envid)
{
	int whom, req, perm;

	binaryname = "ns_output";

	// LAB 6: Your code here:
	// 	- read a packet from the network server
	//	- send the packet to the device driver
	while (1) {
		req = ipc_recv(&whom, &nsipcbuf, &perm);
		if (whom != ns_envid)
			panic("receive from unknown enviroment: [%08x]", whom);
		if (req != NSREQ_OUTPUT)
			continue;

		send_frame();
		sys_page_unmap(0, &nsipcbuf);
	}
}
