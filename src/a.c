#include "a.h"

int measure(int fd, double* angle, double* index)
{
	int byte;
	double data[3];
	char buf[max_buf];
	for(int i=0;i<3;i++)
	{
		byte=read(fd, &buf, sizeof(buf));
		data[i]=atof(buf);
		*(angle+i)=atan(data[i]/height);
		*(index+i) =  sin_45 / sin(angle[i]);
	}

}
