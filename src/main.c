#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max_buf 512 //set max buffer size 512 bytes
#define height 15 //height : 15cm

main()
{
	FILE* fd=0;
	int select=0;

	char buf[max_buf];
	char device_name[20];
	
select:
	puts("1. show serial devices");
	puts("2. select serial device");
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
		default:
			puts("err : please select 1 or 2");
			goto select;
			break;
	}
	fd = fopen(device_name,"r+");
	if(fd==NULL)
	{
		printf("can't open %s\n",device_name);
		return -1;
	}
	else
		printf("%s access ok.", device_name);
	system("clear");
	while(1)
	{
		puts("1. turn off laser");
		puts("2. turn on laser");
		puts("3. measure");
		printf("input : ");
		scanf("%d",&select);
		switch(select)
		{
			case 1:
				fputc("0",fd);
				break;
			case 2:
				fputc("1",fd);
				break;
			case 3:
				fputc("2",fd);
				break;
			default:
				puts("please select 1-3");
		}
	}
}
