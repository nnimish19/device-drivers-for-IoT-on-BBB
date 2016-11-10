/* sendnmsg.c - sendnmsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendnmsg  -  Store a message in mailbox of #pid_count recipients start recipients if waiting
 *------------------------------------------------------------------------
 */

/*
 * Sending the message (msg) to the number (pid_count) of processes (pids). It will return the number of pids successfully sent to or SYSERR on error.
 */

uint32 sendnMsg (uint32 pid_count, pid32* pids, umsg32 msg){
    intmask mask;
    int32 count;

    mask = disable();
    if(pid_count > SEND_LIMIT){     /*Cannot send to more than SEND_LIMIT */
        restore(mask);
        return SYSERR;
    }

    count = 0;

    resched_cntl(DEFER_START);      /*Add multiple waiting processes(if any) to ready list before context switch*/
    while(count < pid_count){
        if(sendMsg(pids[count++], msg)==SYSERR){
            restore(mask);
            return SYSERR;
        }
        kprintf("S:%d>%d,%u\n",currpid,pid[count-1],msg);
    }
    resched_cntl(DEFER_STOP);

    kprintf("S_total:%d,%u\n",currpid, pid_count);
    restore(mask);
    return pid_count;
}