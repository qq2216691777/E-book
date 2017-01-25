#
#	Makefile  topdir
#
CROSS_COMPILE 	:=	arm-linux-
AS				:=	$(CROSS_COMPILE)as
LD				:=	$(CROSS_COMPILE)ld
CC  			:=	$(CROSS_COMPILE)gcc
CPP				:=	$(CC) -E
AR 				:=	$(CROSS_COMPILE)ar
NM				:=	$(CROSS_COMPILE)nm

STRIP 			:=	$(CROSS_COMPILE)strip
OBJCOPY			:=	$(CROSS_COMPILE)objcopy
OBJDUMP			:=	$(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJDUMP OBJCOPY


CFLAGS			:=	-Wall -o2
CFLAGS			+=	-I $(shell pwd)/include
LDFLAGS			:=	-lm	-lfreetype -lts

export CFLAGS LDFLAGS

TOPDIR			:=	$(shell pwd)
export TOPDIR

TARGET	:=	show_file

obj-y	+= main.o
obj-y	+= fb/
obj-y	+= freetype/
obj-y	+= file/
obj-y	+= page/
obj-y	+= input/

all:
	make -C ./  -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o



clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(shell find -name "*~")
	rm -f $(TARGET)