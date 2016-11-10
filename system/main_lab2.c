/*  main.c  - main */

/*
 *   author: Nimish
 */

#include <xinu.h>

pid32 test1_id, test2_id;
pid32 sender1_id, sender2_id, sender3_id;
pid32 receiver1_id, receiver2_id, receiver3_id, receiver4_id;

//-------------------------------------------------------------------------------------------------
void sender1(){
    uint32 i;
    for (i=0;i<5;i++){
        sendMsg (receiver1_id, (umsg32) i);
        //kprintf("S%u >> R%u\n: msg = %u\n",sender1_id,receiver1_id,i);
//        printf("S1: %u\n",i);
    }
}

void sender2(){
    int32 count;
    umsg32 msgs[5]={1,2,3,4,5};
//    printf("S2 Delaying time...\n");
    count=sendMsgs (receiver2_id,msgs,5);
//    printf("S2_total: %d\n",count);       //syserr =-1
}

void sender3(){
    int32 success_pids, pid_count=2;
    umsg32 msg=100;
    pid32 pids[] = {receiver3_id,receiver4_id};
//    printf("S3 Delaying time...\n");
    success_pids=sendnMsg (pid_count,pids,msg);	//syserr =-1
//    printf("S3_total: %d\n",success_pids);
}

void receiver1(){
    uint32 i;
    umsg32 msg;
    for (i=0;i<5;i++){
        msg=receiveMsg();
//        printf("R1: %u\n",msg);
    }
}
void receiver2(){
    uint32 i,msg_count=5;
    umsg32 msgs[5];
    if((receiveMsgs(msgs,msg_count))==SYSERR){   //msg_count(8) > current_mail_count(3) > buffer_available(7)!!
//        printf("R2_ERR\n");
        return ;
    }

//    kprintf("R2_total: %u\n",msg_count);
    for(i=0;i<msg_count;i++){
//        printf("R2: %u\n",msgs[i]);
    }
}
void receiver3(){
    umsg32 msg;
    msg=receiveMsg();
//    printf("R3: %u\n",msg);
}
void mytest(int32 a){
    intmask mask;
    int32 i;
    mask = disable();
    for(i=0;i<10;i++)
        printf("time waste %d",a);
    restore(mask);
}

process	main(void)
{
    recvclr();

//    test1_id = create(mytest, 4096, 50, "test1", 1, 1);
//    test2_id = create(mytest, 4096, 50, "test2", 1, 2);

//    Send 5 messages to R1, one by one
    sender1_id = create(sender1, 4096, 50, "sender1", 0);
//    Receive 5 messages from S1, one by one
    receiver1_id = create(receiver1, 4096, 50, "receiver1", 0);

//    send 5 messages in one time
//    sender2_id = create(sender2, 4096, 50, "sender2", 0);
//    receive 5 messages in one call
//    receiver2_id = create(receiver2, 4096, 50, "receiver2", 0);

//    send msg to r3,r4
//    sender3_id = create(sender3, 4096, 50, "sender3", 0);
//    receive msg from s3
//    receiver3_id = create(receiver3, 4096, 50, "receiver3", 0);
//    receiver4_id = create(receiver3, 4096, 50, "receiver4", 0);

    resched_cntl(DEFER_START);
//    resume(test1_id);
//    resume(test2_id);
    resume(sender1_id);
    resume(receiver1_id);

//    resume(sender2_id);
//    resume(receiver2_id);

//    resume(sender3_id);
//    resume(receiver3_id);
//    resume(receiver4_id);

    resched_cntl(DEFER_STOP);

    return OK;
}