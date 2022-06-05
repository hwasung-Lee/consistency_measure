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

main()
{
	int fd=0;
	int select=0;

	char device_name[20];

	double angle[4] = {0,};
	double index[4] = {0,};
	double index_m=0;
	const double sin_laser_angle = sin(M_PI/2);

	struct termios ottyio, nttyio;	
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
		printf("5. save & exit\n");
		printf("6. back\n");
		printf("7. exit\n");
		printf("input : ");
		scanf("%d",&select);
		switch(select)
		{
			case 1:
				write(fd,laser_off ,1);
				break;
			case 2:
				write(fd, laser_on,1);
				break;
			case 3://measure indexs
				write(fd, measure_dis, 1);
				measure(fd,&angle,&index);
				for(int i=0;i<3;i++)
					index[i]*=index_m;
				break;
			case 4://measure index of arcylic
				write(fd, measure_dis, 1);
				measure(fd, &angle,&index);	
				//index of arcylic is average of three measured data			
				index_m=index[0]+index[1]+index[2];
				index_m/=3;
				break;
			case 5:
				goto save;
			case 6:
				goto select;
			case 7: 
				close(device_name);
				return 0;
			default:
				puts("please select 1-6");
				break;
		}
		system("clear");
		if(select==3 && index_m==0)
				puts("*run NO.4 first");

	}
save:
	FILE* csv=fopen("index.csv","w");
	fprintf(csv,"NO., angle, index\n");
	fprintf(csv,"arc, %.2f, %.2f\n",angle[0], index_m);
	for(int i=0;i<3;i++)
	{
		fprintf("%d, %.2f, %.2f\n",i+1, angle[i], index[i]);
	}
	fclose(csv);
	return 0;
}
