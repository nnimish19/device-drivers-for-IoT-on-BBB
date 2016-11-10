/* msgpool.h */

#ifndef	MB_LIMIT
#define	MB_LIMIT 10		/* MAIL BOX LIMIT of each process		*/
#endif

#ifndef SEND_LIMIT      /* Broadcaste limit*/
#define SEND_LIMIT 3
#endif

/*
 * Each process(pid) has a personal mailbox accessible through mailtab[pid]. So size of mailtab[] is equal to maximum number of process = NPROC
 *
 * 1. Message(m) can be sent to Receiver(pid1) by posting in its mailbox as follows -
 *
 * struct mailbox* ptr = &mailtab[pid1];
 * ptr->mailq[ptr->head++] = m;
 *
 * 2. Message can be received by a process from tail of its mail queue.
 *
 * struct mailbox* ptr = &mailtab[pid1];
 * m = ptr->mailq[ptr->tail++];
 *
 * */

struct mailbox{
    uint32 mcount;      /*number of unread messages in mailbox*/
    uint32 head,tail;   /*head and tail indexes of unread messages in mail queue*/
    umsg32 mailq[10];   /*mail queue*/
};

extern	struct mailbox mailtab[];


//extern syscall msend(pid32 pid, umsg32 msg);
//extern msg mget(pid32 pid);


//struct	bpentry	{		/* Description of a single buffer pool	*/
//    struct	bpentry *bpnext;/* pointer to next free buffer		*/
//    sid32	bpsem;		/* semaphore that counts buffers	*/
//    /*    currently available in the pool	*/
//    uint32	bpsize;		/* size of buffers in this pool		*/
//};
//
//extern	struct	bpentry buftab[];/* Buffer pool table			*/
//extern	bpid32	nbpools;	/* current number of allocated pools	*/

//struct mailent{     //mail entry
//    int32 pid;  //in case its required to
//    int32 msg;
//};