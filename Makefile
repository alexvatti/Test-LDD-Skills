obj-m:=sample-char-drv-example.o

KERNEL_DIR=/lib/modules/3.13.0-96-generic/build

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf *.ko *.o 
