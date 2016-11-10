/*  main.c  - main */

/*
 *   author: Nimish
 */

#include <xinu.h>

pid32 A_id, B_id, C_id,broker_id;

void foo(topic16 topic, void* data, uint32 size){
    uint32 i;
    printf("Function foo() is called with topic16 0x%04X and data ",topic);
    for(i=0;i<size;i++)
        printf("%d ",*((char*)(data)+i));
    printf("\n");
}

void bar(topic16 topic, void* data, uint32 size){
    uint32 i;
    printf("Function bar() is called with topic16 0x%04X and data ",topic);
    for(i=0;i<size;i++)
        printf("%d ",*((char*)(data)+i));
    printf("\n");
}
void subs1(){
    topic16 topic=0x013F;
    subscribe(topic, (*foo));
    while(TRUE){;}
}
void subs2(){
    topic16 topic=0x013F;
    subscribe((topic16)(0x114F), (*bar));
    subscribe(topic, (*bar));
    while(TRUE){;}
}
void pub1(){

    char data1[5] = { 1, 2, 3, 4, 5 };
    char data2[2] = { 66,77 };

    topic16 topic=0x003F;
    publish(topic, data1, 5);

    topic=0x114F;
    publish(topic, data2, 2);
}
process main(void){
    recvclr();
    broker_id = create(broker, 4096, 50, "C", 0);
    A_id = create(subs1, 4096, 50, "A", 0);
    B_id = create(subs2, 4096, 50, "B", 0);
    C_id = create(pub1, 4096, 50, "C", 0);
    resched_cntl(DEFER_START);
    resume(broker_id);
    resume(A_id);
    resume(B_id);
    resume(C_id);
    resched_cntl(DEFER_STOP);
    return OK;
}