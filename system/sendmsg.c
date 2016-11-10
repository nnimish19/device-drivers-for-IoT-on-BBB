/* sendmsg.c - sendmsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendmsg  -  Store a message recipient mailbox and start recipient if waiting
 *------------------------------------------------------------------------
 */

/*Sending message (msg) to process (pid). In case a message or a number of messages are already waiting on pid to
 * receive them, the new msg will be queued. It will return OK on success or SYSERR on error.
 */

syscall sendMsg ( pid32 pid, umsg32 msg ){
    intmask	mask;			/* Saved interrupt mask		*/
    struct	procent *prptr; /* Ptr to process's table entry	*/
    struct mailbox *prmb;   /* pointer to recipient mailbox */

    mask = disable();
    if (isbadpid(pid)) {
        restore(mask);
        return SYSERR;
    }
    prptr = &proctab[pid];
    prmb = &mailtab[pid];

    //If mailbox limit reached, return SYSERR
    if (prmb->mcount >= MB_LIMIT) {
        restore(mask);
        return SYSERR;
    }

    //Store in recipient mailbox
    prmb->mailq[prmb->head++] = msg;
    prmb->mcount++;     /*increment unread message count*/
    prmb->head%=MB_LIMIT;
    kprintf("S:%d>%d,%u\n",currpid,pid,msg);

    //If recipient waiting or in timed-wait make it ready
    if (prptr->prstate == PR_RECV) {
        ready(pid);
    }

    restore(mask);		/* Restore interrupts */
    return OK;
}