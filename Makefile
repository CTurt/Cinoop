CC		:=	gcc
SDIR	:=	source
IDIR	:=	-Iinclude -I../LDFS/LDFS/include
CFLAGS	:=	$(IDIR) -Wall -fms-extensions -O2
LFLAGS	:=	../LDFS/libstaticLDFS.a -lmingw32 -mwindows -lwinmm -lws2_32 -lopengl32 -lglu32
ODIR	:=	build
CFILES	:=	$(wildcard $(SDIR)/*.c)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(wildcard $(SDIR)/*.c))

bin: $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o cinoop.exe $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f *.exe $(ODIR)/*.o
	rmdir $(ODIR)
