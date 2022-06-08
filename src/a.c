#include "a.h"

int measure(int fd, double* angle, double* index)
{
	double data[3];
	char buf[max_buf];
	for(int i=0;i<3;i++)
	{
		read(fd, &buf, sizeof(buf));
		data[i]=atof(buf);
		*(angle+i)=atan(data[i]/height);
		*(index+i) =  sin_45 / sin(angle[i]);
	}
}

int save_csv(int cnt, double* indexs, double* angles)
{
	FILE *csv=fopen("index.csv","w");
	if(csv==NULL)
		return -1;
	fprintf(csv,"NO., angle, index\n");
	fprintf(csv,"acrylic, %.2f, %.2f\n",angles[0], indexs[0]);
	for(int i=1;i<=cnt;i++)
		fprintf(csv,"%d, %.2f, %.2f\n",i, angles[i], indexs[i]);
	fclose(csv);
	return 0;
}

double avg(double* value, int n)
{
	double res=0;
	for(int i = 0;i<n;i++)
	{
		res+=*(value+i);
	}
	res/=n;
	return res;
}
