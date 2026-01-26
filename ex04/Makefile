obj-m += hello_keyboard.o

K_BUILD_DIR=/lib/modules/$(shell uname -r)/build

all:
	make -C $(K_BUILD_DIR) M=$(PWD) modules
	cp 10-usb_keyboard.rules /etc/udev/rules.d/

check:
	$(K_BUILD_DIR)/scripts/checkpatch.pl -f *.c *.h

clean:
	make -C $(K_BUILD_DIR) M=$(PWD) clean
