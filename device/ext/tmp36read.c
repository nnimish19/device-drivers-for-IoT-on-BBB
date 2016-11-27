/* This file is automatically generated according to tmp36.xml */
#include <xinu.h>

devcall tmp36read(struct dentry *devptr, char *buffer, int32 count) {
	float s1;
	char data[4];
	devcall retval;
	struct dentry *devptr1;

	devptr1 = (struct dentry *)&devtab[ADC];
	retval = (*devptr1->dvread)(devptr1, data, 4);
	if (retval == SYSERR)
		return SYSERR;

	s1 = *(unsigned int*)data;
	s1 = (((s1 / 4095.0) * 1.8) - 0.5) * 100;

	*(float*)buffer = s1;
	return retval;
}