/* receivemsg.c - receivemsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receivemsg  -  Wait for a message in mailbox and return the message to the caller
 *------------------------------------------------------------------------
 */

/*Receiving a message from any sender. It causes the calling process to wait for a message to be sent (by any process).
 * When a message is sent, it is received and returned.
 */

umsg32 receiveMsg (void){
    intmask	mask;			/* Saved interrupt mask		*/
    struct procent *prptr;  /* Ptr to process's table entry	*/
    struct mailbox *prmb;   /* pointer to recipient mailbox */
    umsg32 msg;			    /* Message to return		*/

    mask = disable();
    prmb = &mailtab[currpid];
    prptr = &proctab[currpid];


    if (prmb->mcount<1) {   /* If no mail, Block until message arrives	*/
        prptr->prstate = PR_RECV;
        resched();
    }

    msg= prmb->mailq[prmb->tail++];
    prmb->mcount--;
    prmb->tail%=MB_LIMIT;

    kprintf("R:%d,%u\n",currpid,msg);
    restore(mask);
    return msg;
}