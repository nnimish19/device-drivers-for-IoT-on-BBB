/* ﻿mailinit.c - ﻿mailinit */

#include <xinu.h>

struct	mailbox	mailtab[NPROC];		/* Mail table	*/

/*------------------------------------------------------------------------
 *  mailinit  -  Initialize the Mail BOX data structure
 *------------------------------------------------------------------------
 */
status	mailinit(void)
{
    int32 i;
    /* Initialize MailBox */
    for(i=0;i<NPROC;i++){
        mailtab[i].head=0;
        mailtab[i].tail=0;
        mailtab[i].mcount=0;
    }
    return OK;
}
