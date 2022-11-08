BINS=meminit.bin
 
CC=riscv64-unknown-elf-g++
CCFLAGS=-g -march=rv32i -mabi=ilp32 -mcmodel=medany -static -nostdlib \
		-fno-threadsafe-statics -O2
 
ELFS=meminit.elf
 
LDFLAGS=-Wl,--gc-sections -nostartfiles -Wl,-T,src/startup/link.ld
 
OBJCPY=riscv64-unknown-elf-objcopy
OBJCPYFLAGS=-O binary
 
OBJDMP=riscv64-unknown-elf-objdump
OBJDMPFLAGS=-S -d
 
SRCS=src/startup/crt0.s src/main.cpp src/gpio/gpio.cpp src/stdlib/stdlib.cpp \
	 src/int/int.c src/hal/hal.cpp
 
all: meminit.elf meminit.bin
 
clean:
	rm -f dump.txt $(ELFS) $(BINS)
 
disassemble:
	$(OBJDMP) $(OBJDMPFLAGS) $(ELFS) > dump.txt
	cat dump.txt
 
meminit.elf:
	$(CC) $(CCFLAGS) $(LDFLAGS) $(SRCS) -o $(ELFS)
 
meminit.bin:
	$(OBJCPY) $(OBJCPYFLAGS) $(ELFS) $(BINS)
	rm -f ~/AFTx06/$(BINS)
	rm -f ~/AFTx06/waveform.fst
	cp -f $(BINS) ~/AFTx06/$(BINS)