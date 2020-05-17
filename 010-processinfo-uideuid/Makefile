obj-m += uideuid.o
KERNEL_ROOT = /home/rootkid/Downloads/linux-4.19.119/ 

modules:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) modules

clean:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) clean
