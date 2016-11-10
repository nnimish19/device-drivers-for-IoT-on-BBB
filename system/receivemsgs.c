/* receivemsgs.c - receivemsgs */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receivemsgs  -  Wait for a number of messages in mailbox and return OK or SYSERR
 *------------------------------------------------------------------------
 */

/*
 * Receiving a group (msg_count) of messages (msgs). It causes the calling process to wait for msg_count messages to be sent.
 * When all messages are in the queue, they are then all together immediately received.
 */

/*Scenario-1 Avoid Deadlock
 * P1,P2: Sender, R1: receiver
 * Readylist: P1,P2,R1   (P1 executes 1st)
 * Message queue size =10
 *
 * P1: sends 3 msgs to R1
 * P2: wants to send 8 msgs gets error.
 * R1: wants to read 8 msgs, should get error. Do not put to wait, otherwise deadlock would arise. P2 can;t send, R1 can't read.
 * Can be resolved if thow error.
 */

/*
 * Scenario-2, Early WakeUp Call for Receiver
 * R1:Receiver, P1:Sender, P2:Sender  > All in readylist in left to right. Left first.
 * MailBox of R1 is empty.
 *
 * Case1 Same process priorities: R1=P1=P2
 * R1 : receive 3 msgs. Goes to wait
 * P1 : puts 3 msgs in R1. Ads R1 to ready
 * P2 : puts 1 msg to R1
 * R1 : reads 3 msgs exits. comes back to read 1 more msg.
 *
 * Case2 Diff process priorities: R1=P2>P1
 * R1 : receive 3 msgs. Goes to wait
 * P2 : puts 1 msg to R1. Ads R1 to Ready
 * R1 : tries to read 3 - Err.
 * P1 : puts 3 msgs in R1.
 */
syscall receiveMsgs ( umsg32* msgs, uint32 msg_count ){

    intmask	mask;			/* Saved interrupt mask		*/
    struct procent *prptr;  /* Ptr to process's table entry	*/
    struct mailbox *prmb;   /* pointer to recipient mailbox */
    uint32 count;           /* count of messages received */

    mask = disable();
    prmb = &mailtab[currpid];
    prptr = &proctab[currpid];

    /* If less msgs, block until ALL msgs arrives. */
    while (prmb->mcount < msg_count) {              /*Handle Scenario-2 case 2*/
        if(msg_count > (MB_LIMIT - prmb->mcount)){  /*Handle Scenario-1 */
            restore(mask);
            return SYSERR;
        }
        prptr->prstate = PR_RECV;
        resched();
    }

    count=0;
    while(count < msg_count) {
        if((prmb->mcount)<1) {       /*Handle Scenario-2 case 2*/
            restore(mask);
            return SYSERR;
        }
        msgs[count++]= prmb->mailq[prmb->tail++];
        prmb->mcount--;
        prmb->tail%=MB_LIMIT;
        kprintf("R:%d,%u\n",currpid,msgs[count-1]);
    }
    kprintf("R_total:%d,%d\n",currpid,msg_count);
    restore(mask);
    return OK;
}