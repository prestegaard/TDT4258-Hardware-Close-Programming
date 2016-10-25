# Example Makefile
#
# Exercise 2, TDT4258

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib -lm
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

#Default make is improved solution
ex2_improved.bin : ex2_improved.elf
	${OBJCOPY} -O binary $< $@

ex2_improved.elf : ex2_improved.o timer.o dac.o gpio.o interrupt_handlers_improved.o sound.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

ex2_baseline.bin : ex2_baseline.elf
	${OBJCOPY} -O binary $< $@

ex2_baseline.elf : ex2_baseline.o timer.o dac.o gpio.o interrupt_handlers_baseline.o sound.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : pretty
pretty :
	-indent *.c *.h

#Default upload is improved solution
.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex2_improved.bin" -r

.PHONY : upload_improved
upload_imroved :
	-eACommander.sh -r --address 0x00000000 -f "ex2_improved.bin" -r

.PHONY : upload_baseline
upload_baseline :
	-eACommander.sh -r --address 0x00000000 -f "ex2_baseline.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
