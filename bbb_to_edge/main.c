/*  main.c  - main */
#include <xinu.h>

process main(void)
{
//send
    char* smsg="hello";
    sendEdge("192.168.3.3",8000,smsg,5);

//recv
    char* recv=getmem(sizeof(char)*5);
    recvEdge(recv,5);
    int i;
    for(i=0;i<5;i++){
        kprintf("%c",recv[i]);
    }
    return 0;
}
