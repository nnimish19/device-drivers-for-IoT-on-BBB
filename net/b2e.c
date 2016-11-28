#include <xinu.h>
//send msg to dest ip and port, loacl port is 8000
syscall sendEdge(char* dip,uint16 dport,char* msg,uint16 len)
{
    intmask mask;                   /* Saved interrupt mask         */
    mask = disable();
    uint32 rip;
    uint16 lport=8000;
    dot2ip(dip,&rip);
    uid32 slot=udp_register(rip,dport,lport);
    udp_send(slot,msg,len);
    udp_release(slot);
    restore(mask);
    return 0;
}
//recv msg from all ip, loacl port is 8001
syscall recvEdge(char* msg,uint16 len)
{
    intmask mask;                   /* Saved interrupt mask         */
    mask = disable();
    uid32 slot=udp_register(0,0,8001);
    //kprintf("slot: %d\n", slot);
    int i= udp_recv(slot,msg,len,1000000);
    udp_release(slot);
    restore(mask);
    return i;
}
