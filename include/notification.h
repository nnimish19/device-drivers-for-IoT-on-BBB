//This is the structure that is private to Broker.

typedef void (*fp)(topic16, void*, uint32);

#ifndef	TS_LIMIT
#define	TS_LIMIT 8		/* A maximum of 8 processes can subscribe to a single topic*/
#endif

#ifndef	TOPIC_LIMIT
#define	TOPIC_LIMIT 256		/* Maximum no. of topics 0-255*/
#endif


//Topic subscribers list
struct subscriber{
    uint8 gid;                 //grp id
    pid32 id;                  /*For the first entry it represents count of subscribers in this topic. For others its the process id*/
    fp handler;
    struct subscriber *next;   /*First subscriber is added in next*/
};
extern	struct subscriber sublist[];

//Queue of {Topic,Data} published
struct notification{
    topic16 topic;      /*topic published*/
    char* data;         /*pointer to data location*/
    uint32 size;        /*length*/
    struct notification *next;
}*note_front,*note_rear;