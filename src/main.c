#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bandrate B115200

#define laser_off "0"
#define laser_on "1"
#define measure_dis "2"

main(int argc, char *argv[])
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

	struct termios ottyio, nttyio;	

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
				break;
			case 2:
				printf("device name : ");
				scanf("%s",&device_name);
				break;
			case 3:
				return 0;
			default:
				puts("err : please select 1 - 3");
				goto select;
				break;
		}
		fd = open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
	}
	else
		fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fd<0)
	{
		perror(device_name);
		return -1;
	}
	//serial i/o setting
	tcgetattr(fd,&ottyio);
	nttyio.c_cflag = bandrate | CRTSCTS | CS8 | CLOCAL | CREAD;
	nttyio.c_iflag = 0;
	nttyio.c_oflag = 0;
	nttyio.c_lflag = 0;
	nttyio.c_cc[VTIME] = 0;
	nttyio.c_cc[VMIN] = 1;

	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&nttyio);


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
				write(fd,laser_off ,1);
				break;
			case 2://laser on
				write(fd, laser_on,1);
				break;
			case 3://measure indexs
				write(fd, measure_dis, 1);
				measure(fd,&angle,&index);
				for(int i=0;i<3;i++)
					index[i]*=indexs[0];
				angles[1+cnt]=(angle[0]+angle[1]+angle[2])/3;
				indexs[1+cnt++]=(index[0]+index[1]+index[2])/3;
				break;
			case 4://measure index of arcylic
			       do
			       {
					write(fd, measure_dis, 1);
					measure(fd, &angle,&index);	
					//index of arcylic is average of three measured data			
					indexs[0]=index[0]+index[1]+index[2];
					indexs[0]/=3;
					angles[0]=angle[0]+angle[1]+angle[2];
					angles[0]/=3;
				}while(indexs[0]==INFINITY);
				break;
			case 5:
				if(indexs[0]==0)
					break;
				printf("index of acrylic : %.2f\n",indexs[0]);
				for(int i=1;i<=cnt;i++)
					printf("%d | index : %.2f\n",i, indexs[i]);
				puts("enter any number to back");
				scanf("%d",&t);
				break;
			case 6:
				save_csv(cnt, indexs, angles);
				return 0;
			case 7:
				goto select;
			case 8: 
				close(device_name);
				return 0;
			default:
				puts("please select 1-8");
				break;
		}
		system("clear");
		if(indexs[0]==0)
			if(select==3)
				puts("*run NO.4 first");
		if(size-1==cnt)
		{
			indexs=(double*) realloc(indexs, (size+5)*sizeof(double));
			angles=(double*) realloc(angles, (size+5)*sizeof(double));
			size+=5;
		}
	}
}
