/*  main.c  - main */

/*
 *   author: Nimish
 */

#include <xinu.h>


process main(void){
    recvclr();

    //netstart();

    char recv[10];
    char on[]={'1'},off[] = {'0'};
    int status, count=1,t0=0,t1=0;

    kprintf("\n~~~~~ Start: Main function ~~~~~~\n");

    //send initial LED/BUZZER status
    sendEdge("192.168.1.104",8001,"0,off",5);
    sendEdge("192.168.1.104",8001,"1,off",5);

    while(TRUE)
    {
        status = recvEdge(recv,10);
        if(status<0)
        {	//kprintf("status %d",status);
            continue;   //no input
        }
        kprintf("received: %s\n",recv);
        if(recv[0]=='1'){		//LED
            kprintf("Button1\n");
            t0^=1;
            if(t0){
                write(LED, on, count);
                sendEdge("192.168.1.104",8001,"0,on",5);
            }else{
                write(LED, off, count);
                sendEdge("192.168.1.104",8001,"0,off",5);
            }
        }
        else if(recv[0]=='2'){	//BUZZER
            t1^=1;
            kprintf("Button2\n");
            if(t1){
                write(BUZZER, on, count);
                sendEdge("192.168.1.104",8001,"1,on",5);
            }else{
                write(BUZZER, off, count);
                sendEdge("192.168.1.104",8001,"1,off",5);
            }
        }
        //write(BUZZER, recv, count);

    }
    kprintf("\n\nEND\n");
    return OK;
}