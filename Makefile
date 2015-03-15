.PHONY: all
.PHONY: clean

all: cinoop.exe cinoop.nds cinoop.dol cinoop.3ds

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

clean:
	rm -f wbuild/*
	rm -f dbuild/*
	rm -f cinoop.elf
	rm -f cinoopstripped.elf
	rm -f cinoop.nds
	rm -f cinoop.dol
	rm -f cinoop.3ds
	rm -f cinoop.3dsx
	rm -f cinoop.cia
	rm -f cinoop.smdh
	rm -f cinoop.exe
	rmdir wbuild
	rmdir dbuild
	rmdir gbuild
	rmdir 3build
