/* kill.c - kill */

#include <xinu.h>
#include "../include/prototypes.h"

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
    struct subscriber *sub_ptr,*prev;
	int32	i;			/* Index into descriptors	*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}
	freestk(prptr->prstkbase, prptr->prstklen);
//-------------
//    unsubscribe from all topics
    for(i=0;i<TOPIC_LIMIT;i++){
        sub_ptr = &sublist[i];
        while (sub_ptr->next != NULL) {
            prev = sub_ptr;
            sub_ptr = sub_ptr->next;
            if (sub_ptr->id == pid){   //Each topic has a uniq subscriber(currpid)
                prev->next= sub_ptr->next;
                freemem(sub_ptr, sizeof(struct subscriber));
                sublist[i].id--;
                kprintf("Process %d unsubscribes with a topic16 value of 0x%04X ", pid, i);
                kprintf("Topic %d count: %d \n", i, sublist[i].id);
                break;
            }
        }
    }
//-------------

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}
