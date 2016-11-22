#ifndef ACT_H
#define ACT_H

devcall	actinit (struct	dentry *devptr);
devcall	actwrite(struct dentry* devptr, char* buff, int32 count);

#endif
