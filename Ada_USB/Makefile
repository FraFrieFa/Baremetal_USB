all:
	gprbuild
	arm-eabi-objcopy -O binary obj/kernel obj/kernel.bin

program: all
	sudo bossac -o 0x4000 -w obj/kernel.bin -U -R