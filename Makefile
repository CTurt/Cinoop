.PHONY: all
.PHONY: clean

# DS settings
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

TARGET	:=	cinoop
BUILD	:=	dbuild
SOURCES	:=	source source/ds  
INCLUDES:=	include

CFLAGS	:=	-Wall -fms-extensions -Ofast -fomit-frame-pointer -ffast-math
CFLAGS	+=	-I$(LIBNDS)/include -I../include -DARM9 -DDS

ASFLAGS	:=	$(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:= -lfat -lnds9 -lm
LIBDIRS	:=	$(LIBNDS)

ifneq ($(BUILD),$(notdir $(CURDIR)))
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(BINFILES:.bin=.o) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD)

all: cinoop.exe $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

else
 
DEPENDS	:=	$(OFILES:.o=.d)

$(OUTPUT).nds	: 	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

%.o	:	%.bin
	@echo $(notdir $<)
	$(bin2o)

-include $(DEPENDS)

endif

# Windows
WCC		:=	gcc
SDIR	:=	source
IDIR	:=	-Iinclude -I../LDFS/LDFS/include
WCFLAGS	:=	$(IDIR) -Wall -fms-extensions -Ofast

WLFLAGS	:=	../LDFS/libstaticLDFS.a -lmingw32 -mwindows -lwinmm -lws2_32 -lopengl32 -lglu32
WODIR	:=	wbuild

CFILES	:=	$(wildcard $(SDIR)/*.c)
WCFILES	:=	$(CFILES) $(wildcard $(SDIR)/windows/*.c)

WOBJS	:=	$(patsubst $(SDIR)/windows/%.c, $(WODIR)/%.o, $(WCFILES))
WOBJS	:=	$(patsubst $(SDIR)/%.c, $(WODIR)/%.o, $(WOBJS))

cinoop.exe: $(WODIR) $(WOBJS)
	$(WCC) $(WODIR)/*.o -o cinoop.exe $(WLFLAGS)

$(WODIR)/%.o: $(SDIR)/%.c
	$(WCC) -c -o $@ $< $(WCFLAGS)

$(WODIR)/%.o: $(SDIR)/windows/%.c
	$(WCC) -c -o $@ $< $(WCFLAGS)

$(WODIR):
	@mkdir $@

clean:
	rm -f $(BUILD)/*
	rm -f $(WODIR)/*
	rm -f cinoop.elf
	rm -f cinoop.nds
	rm -f cinoop.exe
	rmdir $(BUILD)
	rmdir $(WODIR)
