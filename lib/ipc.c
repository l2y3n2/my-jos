// User-level IPC library routines

#include <inc/lib.h>

// Receive a value via IPC and return it.
// If 'pg' is nonnull, then any page sent by the sender will be mapped at
//	that address.
// If 'from_env_store' is nonnull, then store the IPC sender's envid in
//	*from_env_store.
// If 'perm_store' is nonnull, then store the IPC sender's page permission
//	in *perm_store (this is nonzero iff a page was successfully
//	transferred to 'pg').
// If the system call fails, then store 0 in *fromenv and *perm (if
//	they're nonnull) and return the error.
// Otherwise, return the value sent by the sender
//
// Hint:
//   Use 'env' to discover the value and who sent it.
//   If 'pg' is null, pass sys_ipc_recv a value that it will understand
//   as meaning "no page".  (Zero is not the right value, since that's
//   a perfectly valid place to map a page.)
int32_t
ipc_recv(envid_t *from_env_store, void *pg, int *perm_store)
{
	// LAB 4: Your code here.
	int r;

	if (pg == NULL)
		pg = (void *) UTOP;

	r = sys_ipc_recv(pg);
	if (r < 0) {
		if (from_env_store)
			*from_env_store = 0;
		if (perm_store)
			*perm_store = 0;
		return r;
	}

	if (from_env_store)
		*from_env_store = env->env_ipc_from;
	if (perm_store)
		*perm_store = env->env_ipc_perm;

	return env->env_ipc_value;
}

// Send 'val' (and 'pg' with 'perm', if 'pg' is nonnull) to 'toenv'.
// This function keeps trying until it succeeds.
// It should panic() on any error other than -E_IPC_NOT_RECV.
//
// Hint:
//   Use sys_yield() to be CPU-friendly.
//   If 'pg' is null, pass sys_ipc_recv a value that it will understand
//   as meaning "no page".  (Zero is not the right value.)
void
ipc_send(envid_t to_env, uint32_t val, void *pg, int perm)
{
	// LAB 4: Your code here.
	int r;

	if (pg == NULL)
		pg = (void *) UTOP;

	while ((r = sys_ipc_try_send(to_env, val, pg, perm))) {
		if (r != -E_IPC_NOT_RECV)
			panic("sys_ipc_try_send: %e", r);
	}
}
