obj-m += gpio_driver.o
obj-m += seg_driver.o
RESULT = seg_hw
SRC = $(RESULT).c

all:

make -C ~/working/kernel M=$(PWD) modules
aarch64-linux-gnu-gcc -o $(RESULT) $(SRC)

clean:

make -C ~/working/kernel M=$(PWD) clean
rm -f $(RESULT)
