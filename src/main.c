#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "a.h"

#define bandrate B115200
#define open_OPT O_RDWR | O_NOCTTY | O_NONBLOCK

#define laser_off "0"
#define laser_on "1"
#define measure_dis "2"

int main(int argc, char **argv)
{
	int fd=0;
	int select=0;
	int cnt=0;
	int size=5;
	int t;

	char device_name[20];

	double angle[4] = {0,};
	double index[4] = {0,};
	double* indexs;
	double* angles;
	double dis[4]={0,};
	double avg_dis;
	
	struct termios ttyio;

	indexs=(double*)malloc(sizeof(double)*5);
	angles=(double*)malloc(sizeof(double)*5);
	memset(indexs, 0, sizeof(double)*5);
	memset(angles, 0, sizeof(double)*5);

	if(argc == 1)
	{
select:
		puts("1. show serial devices");
		puts("2. select serial device");
		puts("3. exit");
		printf("select : ");
		scanf("%d",&select);
	
		switch(select)
		{
			case 1:
				system("ls /dev/ttyACM*");
				goto select;
			case 2:
				printf("device name : ");
				scanf("%s",&device_name);
				break;
			case 3:
				return 0;
			default:
				puts("err : please select 1 - 3");
				goto select;
		}
		fd = open(device_name, open_OPT);
	}
	else
		fd = open(argv[1], open_OPT);
	if(fd<0)
	{
		perror(device_name);
		return -1;
	}
	//serial i/o setting
	ttyio.c_cflag = bandrate | CRTSCTS | CS8 | CLOCAL | CREAD;
	ttyio.c_iflag = 0;
	ttyio.c_oflag = 0;
	ttyio.c_lflag = 0;
	ttyio.c_cc[VTIME] = 0;
	ttyio.c_cc[VMIN] = 1;

	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&ttyio);

	system("clear");
	while(1)
	{
		printf("1. turn off laser\n");
		printf("2. turn on laser\n");
		printf("3. measure index\n");
		printf("4. measure index of acrylic\n");
		printf("5. show data\n");
		printf("6. save & exit\n");
		printf("7. back\n");
		printf("8. exit\n");
		printf("input : ");
		scanf("%d",&select);
		switch(select)
		{
			case 1://laser off
				write(fd,laser_off,1);
				break;
			case 2://laser on
				write(fd,laser_on,1);
				break;
			case 3://measure indexs
			       do
			       {
					write(fd, measure_dis, 1);
					measure(fd,&dis);
					avg_dis=avg(&dis,3);
					angles[cnt+1]=atan((sqrt(pow(height,2)+pow(avg_dis,2)))/(avg_dis*indexs[0]));
					indexs[cnt+1]=sin_45/sin(angles[cnt+1]);
			       }while(index[0]==INFINITY | index[1]==INFINITY | index[2]==INFINITY | indexs[cnt+1]==INFINITY);
				cnt++;
				break;
			case 4://measure index of arcylic
			       do
			       {
					write(fd, measure_dis, 1);
					measure_arc(fd, &angle,&index);	
					//index of arcylic is average of three measured data			
					indexs[0]=avg(&index,3);
					angles[0]=avg(&angle,3);
				} while(indexs[0]==INFINITY);
				break;
			case 5:
				if(indexs[0]==0)
					break;
				printf("index of acrylic : %.2f\n",indexs[0]);
				for(int i=1;i<=cnt;i++)
					printf("%d | index : %.2f\n",i, indexs[i]);
				puts("enter any number to back");
				scanf("%s",&t);
				break;
			case 6:
				save_csv(cnt, indexs, angles);
				return 0;
			case 7:
				goto select;
			case 8: 
				close(fd);
				return 0;
			default:
				puts("please select 1-8");
				break;
		}
		system("clear");
		if(indexs[0]==0)
			if(select==3)
				puts("*run NO.4 first");
		else if(size-1==cnt)
		{
			indexs=(double*) realloc(indexs, (size+5)*sizeof(double));
			angles=(double*) realloc(angles, (size+5)*sizeof(double));
			size+=5;
		}
	}
}
