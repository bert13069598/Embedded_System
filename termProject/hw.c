#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/photo.hpp>
static struct termios init_setting, new_setting;
char seg_num[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
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
	if (read(STDIN_FILENO, &ch, 1) != 1)
		ch = -1;
	return ch;
}
void print_menu()
{
	printf("\n----------menu----------\n");
	printf("[p] : cnt setting\n");
	printf("[q] : program exit\n");
	printf("[u] : cnt up\n");
	printf("[d] : cnt down\n");
	printf("------------------------\n\n");
}
int main(int argc, char **argv)
{
	unsigned short data[4];

	Mat img;
	char key;
	int tmp_n;
	int delay_time;
	int camStatus;
	int tmp_d;
	int tmp_d1, tmp_d2, tmp_d3, tmp_d4;
	char buff;
	char tmp;
	char prev = 'r';
	int dev_seg = open("/dev/my_segment", O_RDWR);
	int dev_gpio = open("/dev/my_gpio", O_RDWR);
	if (dev_seg == -1)
	{
		printf("Seg opening was not possible!\n");
		return -1;
	}
	if (dev_gpio == -1)
	{
		printf("Gpio opening was not possible!\n");
		return -1;
	}
	VideoCapture cap;
	cap.open("/dev/video0", CAP_V4L2);
	if (!cap.isOpened())
	{
		printf("Can't open Camera\n");
		return -1;
	}

	printf("device opening was successfull!\n");
	init_keyboard();
	print_menu();
	tmp_n = 0; // related to pos
	tmp_d = 0; // related to cnt
	camStatus = 0;

	char string[4];
	// delay_time = 500000;
	delay_time = 500;
	while (1)
	{
		cap >> img;

		if(img.empty())
		{
			printf("empty image");
			return 0;
		}

		
		
		tmp_d1 = tmp_d / 1000; // related to seg
		tmp_d2 = (tmp_d % 1000) / 100;
		tmp_d3 = (tmp_d % 100) / 10;
		tmp_d4 = tmp_d % 10;
		data[0] = (seg_num[tmp_d1] << 4) | D1;
		data[1] = (seg_num[tmp_d2] << 4) | D2;
		data[2] = (seg_num[tmp_d3] << 4) | D3;
		data[3] = (seg_num[tmp_d4] << 4) | D4;
		key = get_key();
		if (key == 'q')
		{
			printf("exit this program.\n");
			break;
		}
		else if (key == 'u')
		{
			printf("cnt up\n");
			tmp_d++;
		}
		else if (key == 'd')
		{
			printf("cnt down\n");
			tmp_d--;
		}
		else if (key == 'p')
		{
			printf("cnt setting\n");
			for (int i = 0; i < 4; i++)
			{
				while (1)
				{
					key = get_key();
					if (key == '0' || key == '1' || key == '2' || key == '3' ||
						key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9')
					{
						string[i] = key;
						key = -1;
						break;
					}
				}
			}
			tmp_d1 = (int)string[0] - 48;
			tmp_d2 = (int)string[1] - 48;
			tmp_d3 = (int)string[2] - 48;
			tmp_d4 = (int)string[3] - 48;
			printf("%d %d %d %d\n", tmp_d1, tmp_d2, tmp_d3, tmp_d4);
			tmp_d = tmp_d1 * 1000 + tmp_d2 * 100 + tmp_d3 * 10 + tmp_d4;
		}
		write(dev_seg, &data[tmp_n], 2);
		// write(dev_gpio, &data[tmp_n],2);
		usleep(delay_time);
		// tmp_d++;
		if (tmp_d > 9999)
		{
			tmp_d = 0;
		}
		else if (tmp_d < 0)
		{
			tmp_d = 9999;
		}
		tmp_n++;
		if (tmp_n > 3)
		{
			tmp_n = 0;
		}
		// related to button
		// read(dev_seg, &buff,1);
		read(dev_gpio, &buff, 1);
		prev = tmp;
		tmp = buff;
		// write(dev,&tmp, 1);

		//visualize camera
		imshow("camera img",img);

		// change camStatus ON
		if (prev == '0' && tmp == '1' && camStatus == 0)
			camStatus = 1;
		// else if (prev == '0' && tmp == '2')
		//	tmp_d -= 1;

		if (camStatus == 1)
		{
			imwrite("test.jpg",img);
			//activate shell script
			int ret = system("");	//실행할 이름
			camStatus = 0;
		}
	}
	close_keyboard();
	write(dev_seg, 0x0000, 2);
	close(dev_seg);
	close(dev_gpio);

	return 0;
}
