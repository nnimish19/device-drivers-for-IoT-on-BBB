/* noteinit.c - noteinit */

#include <xinu.h>

struct	subscriber	sublist[TOPIC_LIMIT];		/* Topic Subscriber table	*/

/*------------------------------------------------------------------------
 *  noteinit  -  Initialize the Subscriber data structure
 *------------------------------------------------------------------------
 */
status	noteinit(void)
{
    uint16 i;
    /* Initialize subscriber list */
    for(i=0; i < TOPIC_LIMIT; i++) {
            sublist[i].gid = 0;
            sublist[i].id = 0;
            sublist[i].handler = NULL;
            sublist[i].next = NULL;
    }
    return OK;
}
