#include <xinu.h>

// to insert elements in queue
void note_insert(topic16 topic, void* data, uint32 size){
    struct notification *temp;
    uint32 i;

    temp = (struct notification*)getmem(sizeof(struct notification));

    temp->topic = topic;
    temp->data = (char*)getmem(size);
    temp->size = size;
    temp->next = NULL;

    for(i=0;i<size;i++){
        temp->data[i] = *((char*)(data)+i);
    }

    if (note_rear  ==  NULL)
    {
        note_front = note_rear = temp;
    }
    else
    {
        note_rear->next= temp;
        note_rear = temp;
    }
}

// delete elements from queue
void note_delete(){
    struct notification *temp = note_front;

    if (note_front == NULL)
    {
        note_front = note_rear = NULL;
    }
    else
    {
        note_front = note_front->next;
        freemem(temp->data, temp->size);
        freemem(temp, sizeof(struct notification));
    }
}

// returns first element of queue
struct notification* note_first(){
    return note_front;      //if (note_front == NULL) "The queue is empty"

}

// check if queue is empty or not
byte note_empty(){
    if (note_front == NULL)
        return TRUE;
    return FALSE;
}