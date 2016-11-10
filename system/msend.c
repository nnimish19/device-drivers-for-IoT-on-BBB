#include <xinu.h>

syscall msend(pid32 pid, umsg32 msg){

    struct mailbox *prmb;
    prmb = &mailtab[pid];
    //    Check if pool is full
    if (prmb->mcount >= MB_LIMIT) {
        return SYSERR;
    }

    prmb->mcount++;
    prmb->mailq[prmb->head++] = msg;
    prmb->head%=MB_LIMIT;
    return OK;
}

////    Enqueue
////    Check if pool is full

//if (prmb->mcount == MBSIZE) {
//restore(mask);
//return SYSERR;
//}
//
////    Doubt if after above check, current process gets contexed switched? and another process executes mcount++? (interrupts are disabled. can it get con?)
////prmb->queue[prmb->head].pid = pid;

//prmb->mcount++;
//prmb->mailq[prmb->head++] = msg;
//if(prmb->head++ == MBSIZE)      //mailbox size
//prmb->head=0;


//Dequeue
//    msg = mqueue_getfirst();		    /* Retrieve message		*/
//    msg = prmb->queue.last().msg
//    prmb->queue.pop();
//    OR
//    entry  = prmb->queue.pop()
//    msg=entry.msg

//    msg= prmb->queue[prmb->tail++].msg;
