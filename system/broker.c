#include <xinu.h>

//APIs provided by BROKER. Only BROKER can use the underlying Data Structure i.e.,  Subscriber, Notification

syscall subscribe(topic16 topic, void (*handler)(topic16, void *, uint32)){
    intmask	mask;			        // Saved interrupt mask
    uint8 grp,tp;                   // Not required. creating two variables just for understanding.
    struct subscriber *sub_ptr;

    mask = disable();
    grp = (topic)>>(8);
    tp = (topic)&(0xff);
    sub_ptr = &sublist[tp];

    //If topic has reached max subscribers
    if(sub_ptr->id >= TS_LIMIT){        //First entry sublist[tp] contains number of subscribers for this topic
        restore(mask);
        return SYSERR;
    }

    //If this process is already subscribed to same topic in same or another grp, return error
    while (sub_ptr->next != NULL) {
        sub_ptr = sub_ptr->next;
        if (sub_ptr->id == currpid) {   //already subscribed to any grp of this topic
            restore(mask);
            return SYSERR;
        }
    }

    //else join
    sub_ptr->next = (struct subscriber*)getmem(sizeof(struct subscriber));
    sub_ptr=sub_ptr->next;
    sub_ptr->gid = grp;
    sub_ptr->id = currpid;
    sub_ptr->handler = handler;
    sub_ptr->next = NULL;
    sublist[tp].id++;

    kprintf("Process %d subscribes with a topic16 value of 0x%04X and handler %u\n", currpid, topic, handler);
    restore(mask);
    return OK;
}

syscall unsubscribe(topic16 topic){
    intmask mask;
    uint8 grp,tp;
    struct subscriber *sub_ptr,*prev;

    mask = disable();
    grp = (topic)>>(8);
    tp = (topic)&(0xff);
    sub_ptr = &sublist[tp];

    if(sub_ptr->id <= 0){              //No subscribers
        restore(mask);
        return SYSERR;
    }

    while (sub_ptr->next != NULL) {
        prev = sub_ptr;
        sub_ptr = sub_ptr->next;
        if (sub_ptr->id == currpid){   //Each topic has a uniq subscriber(currpid)
            prev->next= sub_ptr->next;
            freemem(sub_ptr, sizeof(struct subscriber));
            sublist[tp].id--;
            kprintf("Process %d unsubscribes with a topic16 value of 0x%04X\n", currpid, topic);
            restore(mask);
            return OK;
        }
    }

    restore(mask);
    return SYSERR;                      //No such topic subscribed
}

syscall publish(topic16 topic, void *data, uint32 size){
    intmask mask;
    mask = disable();

    note_insert(topic, data, size);           //Enqueue notification

    printf("Process %d publishes data %u to topic16 0x%04X\n", currpid, data, topic);
    restore(mask);
    return OK;
}

void broker(){

    uint8 grp,tp;
    topic16 topic;
    struct subscriber *sub_ptr;

    while(TRUE){
        if (note_empty()) continue;

        struct notification *n = note_first();

        //Notification has {topic, data}. Send this data to all subscribers of this topic.
        topic = n->topic;

        grp = (topic)>>(8);
        tp = (topic)&(0xff);

        sub_ptr = &sublist[tp];
        while (sub_ptr->next != NULL) {
            sub_ptr = sub_ptr->next;
            if(grp==0 || sub_ptr->gid==grp)  //if grp = 0, send to all!
                sub_ptr->handler(topic, n->data, n->size);
        }

        note_delete();              //Dequeue notificaiton
    }
}