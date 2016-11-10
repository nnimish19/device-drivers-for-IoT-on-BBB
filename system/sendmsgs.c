/* sendmsgs.c - sendmsgs */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendmsgs  -  Store multiple messages in recipient mailbox and start recipient if waiting
 *------------------------------------------------------------------------
 */

/*
 * Sending a group (msg_count) of messages (msgs) to process (pid). It will return the number of msgs successfully sent or SYSERR on error.
 */

uint32 sendMsgs ( pid32 pid, umsg32* msgs, uint32 msg_count ){
    intmask	mask;			/* Saved interrupt mask		*/
    struct	procent *prptr; /* Ptr to process's table entry	*/
    struct mailbox *prmb;   /* pointer to recipient mailbox */
    int count;              /* count of messages sent */

    mask = disable();
    if (isbadpid(pid)) {
        restore(mask);
        return SYSERR;
    }
    prptr = &proctab[pid];
    prmb = &mailtab[pid];

    //If number of messages to be delivered are more than current limit of mailbox, return SYSERR
    if (msg_count > (MB_LIMIT - prmb->mcount)) {
        restore(mask);
        return SYSERR;
    }

    //Store #msg_count messages in recipient mailbox
    count=0;
    while(count<msg_count){
        prmb->mailq[prmb->head++] = msgs[count++];
        prmb->mcount++;
        prmb->head%=MB_LIMIT;
        kprintf("S:%d >%d,%u\n",currpid,pid,msgs[count-1]);
    }

    //If recipient waiting or in timed-wait make it ready
    if (prptr->prstate == PR_RECV) {
        ready(pid);
    }

    kprintf("S_total:%d,%u\n",currpid,msg_count);
    restore(mask);		/* Restore interrupts */
    return msg_count;
}

//check error in head++? Also, Can multiple senders be active inside this section? critical section on head?
//No need. Disabling interrupts reserves the processor for current process,
//until it calls resched() within Or restores the interrupt.