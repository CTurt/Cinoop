.PHONY: all
.PHONY: clean

all: cinoop.exe cinoop.nds cinoop.dol cinoop.3ds cinoop cinoop.elf

.PHONY: cinoop.exe
cinoop.exe:
	make -f Makefile.win

.PHONY: cinoop.nds
cinoop.nds:
	make -f Makefile.ds

.PHONY: cinoop.dol
cinoop.dol:
	make -f Makefile.gc

.PHONY: cinoop.3ds
cinoop.3ds:
	make -f Makefile.3d

.PHONY: cinoop
cinoop:
	make -f Makefile.lin

.PHONY: cinoop.elf
cinoop.elf:
	make -f Makefile.ps2

clean:
	rm -f wbuild/*
	rm -f dbuild/*
	rm -f gbuild/*
	rm -f 3build/*
	rm -f lbuild/*
	rm -f cinoop.elf
	rm -f cinoopstripped.elf
	rm -f cinoop.nds
	rm -f cinoop.dol
	rm -f cinoop.3ds
	rm -f cinoop.3dsx
	rm -f cinoop.cia
	rm -f cinoop.smdh
	rm -f cinoop.exe
	rm -f cinoop
	rmdir wbuild
	rmdir dbuild
	rmdir gbuild
	rmdir 3build
	rmdir lbuild
