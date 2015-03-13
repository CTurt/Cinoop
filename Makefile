.PHONY: all
.PHONY: clean

all: cinoop.exe cinoop.nds cinoop.dol

.PHONY: cinoop.exe
cinoop.exe:
	make -f Makefile.win

.PHONY: cinoop.nds
cinoop.nds:
	make -f Makefile.ds

.PHONY: cinoop.dol
cinoop.dol:
	make -f Makefile.gc

clean:
	rm -f wbuild/*
	rm -f dbuild/*
	rm -f cinoop.elf
	rm -f cinoop.nds
	rm -f cinoop.dol
	rm -f cinoop.exe
	rmdir wbuild
	rmdir dbuild
	rmdir gbuild
