#include <xinu.h>

umsg32 mfetch(pid32 pid){

    struct mailbox *prmb;
    prmb = &mailtab[pid];
    if (prmb->mcount<1) {   /* If no mail, Block until message arrives	*/
        kprintf("$A\n");
        return SYSERR;
    }

    kprintf("^A\n");
    prmb->mcount--;
    msg= prmb->mailq[prmb->tail++];
    prmb->tail%=MB_LIMIT;
    return OK;
}