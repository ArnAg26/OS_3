obj-m += m.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc gg.c -o gg

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
