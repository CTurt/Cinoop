.PHONY: all
.PHONY: clean

all: cinoop.exe cinoop.nds

cinoop.exe:
	make -f Makefile.win

cinoop.nds:
	make -f Makefile.ds

clean:
	rm -f wbuild/*
	rm -f dbuild/*
	rm -f cinoop.elf
	rm -f cinoop.nds
	rm -f cinoop.exe
	rmdir wbuild
	rmdir dbuild
