#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static struct termios init_setting, new_setting;
char seg_num[10] = {0xc0,0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
char seg_dnum[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x58, 0x00, 0x10};

#define D1 0x01
#define D2 0x02
#define D3 0x04
#define D4 0x08

void init_keyboard()
{
	tcgetattr(STDIN_FILENO, &init_setting);
	new_setting = init_setting;
	new_setting.c_lflag &= ~ICANON;
	new_setting.c_lflag &= ~ECHO;
	new_setting.c_cc[VMIN] = 0;
	new_setting.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_setting);
}

void close_keyboard()
{
	tcsetattr(0, TCSANOW, &init_setting);
}

char get_key()
{
	char ch = -1;

	if(read(STDIN_FILENO, &ch, 1) != 1)
		ch = -1;
	return ch;
}

void print_menu()
{
	printf("\n----------menu----------\n");
	printf("[r] : program reset\n");
	printf("[q] : program exit\n");
	printf("------------------------\n\n");
}

int main(int argc, char **argv)
{
	unsigned short data[4];
	char key;
	int tmp_n;
	int delay_time;
	
	int tmp_d;
	int tmp_d1, tmp_d2, tmp_d3,tmp_d4;

	char buff;
    char tmp;
    char prev='r';

	int dev = open("/dev/my_segment", O_RDWR);

	if(dev == -1){
		printf("Opening was not possible!\n");
		return -1;
	}
	printf("device opening was successfull!\n");

	init_keyboard();
	print_menu();
	tmp_n=0; // related to pos 
	tmp_d =4321;
	tmp_d1=tmp_d/1000; // related to seg
	tmp_d2=(tmp_d%1000)/100;
	tmp_d3=(tmp_d%100)/10;
	tmp_d4=tmp_d%10;
	// delay_time = 500000;
	delay_time = 500;

	data[0] = (seg_num[tmp_d1] << 4) | D1;
	data[1] = (seg_num[tmp_d2] << 4) | D2;
	data[2] = (seg_num[tmp_d3] << 4) | D3;
	data[3] = (seg_num[tmp_d4] << 4) | D4;

	while(1){
		key = get_key();
		if(key == 'q'){
			printf("exit this program.\n");
			break;
		}
		else if(key == 'r'){
			//delay_time = 500000;
			delay_time = 500;
			tmp_n=0;
		}

		write(dev, &data[tmp_n],2);
		usleep(delay_time);

		//tmp_d++;
		if (tmp_d>9999){
			tmp_d=0;
		}
		else if (tmp_d<0){
			tmp_d=9999;
		}

		tmp_n++;
		if(tmp_n > 3){
			tmp_n = 0;
		}

		// related to button
		read(dev, &buff,1);
        prev=tmp;
        tmp=buff;
        // write(dev,&tmp, 1);
        if(prev=='0' && tmp=='1')
			tmp_d+=1;
		else if (prev=='0' && tmp=='2')
			tmp_d-=1;
		
	}

	close_keyboard();
	write(dev, 0x0000, 2);
	close(dev);
	return 0;
}

