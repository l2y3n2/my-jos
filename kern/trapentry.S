/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>

#include <kern/picirq.h>


###################################################################
# exceptions/interrupts
###################################################################

/* The TRAPHANDLER macro defines a globally-visible function for handling
 * a trap.  It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 */ 
#define TRAPHANDLER(name, num, priv)					\
.text;									\
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps;							\
.data;									\
	.long num, name, priv;

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num, priv)				\
.text;									\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps;							\
.data;									\
	.long num, name, priv;

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */
.data
	.global irq_handler_array
	irq_handler_array:

TRAPHANDLER_NOEC(irq_handler_divide, T_DIVIDE, 0)
TRAPHANDLER_NOEC(irq_handler_debug, T_DEBUG, 0);
TRAPHANDLER_NOEC(irq_handler_nmi, T_NMI, 0);
TRAPHANDLER_NOEC(irq_handler_brkpt, T_BRKPT, 3);
TRAPHANDLER_NOEC(irq_handler_oflow, T_OFLOW, 0);
TRAPHANDLER_NOEC(irq_handler_bound, T_BOUND, 0);
TRAPHANDLER_NOEC(irq_handler_illop, T_ILLOP, 0);
TRAPHANDLER_NOEC(irq_handler_device, T_DEVICE, 0);
TRAPHANDLER(irq_handler_dblflt, T_DBLFLT, 0);
TRAPHANDLER(irq_handler_tss, T_TSS, 0);
TRAPHANDLER(irq_handler_segnp, T_SEGNP, 0);
TRAPHANDLER(irq_handler_stack, T_STACK, 0);
TRAPHANDLER(irq_handler_gpflt, T_GPFLT, 0);
TRAPHANDLER(irq_handler_pgflt, T_PGFLT, 0);
TRAPHANDLER_NOEC(irq_handler_fperr, T_FPERR, 0);
TRAPHANDLER(irq_handler_align, T_ALIGN, 0);
TRAPHANDLER_NOEC(irq_handler_mchk, T_MCHK, 0);
TRAPHANDLER_NOEC(irq_handler_simderr, T_SIMDERR, 0);
TRAPHANDLER_NOEC(irq_handler_syscall, T_SYSCALL, 3);

TRAPHANDLER_NOEC(irq_handler_timer, IRQ_OFFSET + IRQ_TIMER, 0);
TRAPHANDLER_NOEC(irq_handler_kbd, IRQ_OFFSET + IRQ_KBD, 0);
TRAPHANDLER_NOEC(irq_handler_serial, IRQ_OFFSET + IRQ_SERIAL, 0);
TRAPHANDLER_NOEC(irq_handler_spurious, IRQ_OFFSET + IRQ_SPURIOUS, 0);
TRAPHANDLER_NOEC(irq_handler_ide, IRQ_OFFSET + IRQ_IDE, 0);

TRAPHANDLER_NOEC(irq_handler_default, T_DEFAULT, 0);


/*
 * Lab 3: Your code here for _alltraps
 */
_alltraps:
	pushl %ds;
	pushl %es;
	pushal;
	movl $GD_KD, %eax;
	movw %ax, %ds;
	movw %ax, %es;
	pushl %esp

	call trap
