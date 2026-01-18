obj-m += hello_debugfs.o

K_BUILD_DIR=/lib/modules/$(shell uname -r)/build

all:
	make -C $(K_BUILD_DIR) M=$(PWD) modules

clean:
	make -C $(K_BUILD_DIR) M=$(PWD) clean
