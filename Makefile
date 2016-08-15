#
# Makefile for ROOT-based Applications
# Environment variable ROOTSYS must be set
#
INSTALL_LOCATION = ~/ROOT/slib
#
mcb = $(PWD)
#
#C = $(mcb)
#F = $(mcb)
#I = $(mcb)
C = $(mcb)/src
F = $(mcb)/src
I = $(mcb)/src
IF = $(mcb)/include/f
IR = $(ROOTSYS)/include
#       Binary directories
MTYP=$(OSTYPE)
O = $(mcb)/$(MTYP)/obj
B = $(mcb)/$(MTYP)/bin
#       Binary directories
#O = $(mcb)
#B = $(mcb)
#
cernlib = $(CERNROOT)/lib
#
# Compiler/Linker and Flags
CCCOMP = g++ 
CCOMP = gcc
FCOMP = gfortran
CFLAGS = $(CCFLAGS)
FFLAGS = $(FORFLAGS)
LFLAGS = $(LDFLAGS)
C++FLAGS = $(CCCFLAGS)
MATHLIB = -lm
LIBFOR = -lgfortran
SOFLAGS=-shared
#
LIBREADLINE = -L$(TERMCAPLIB) -ltermcap   -lreadline
PLOTMATH = -lF77 -lI77 -lU77
HIGZLIB = $(cernlib)/libgrafX11.a
BUTTONLIB = -L$(XLIB) -L$(XAWLIB) -lXaw3d -lXmu -lXt -lXext -lX11 -lm
#
#
CFLAGS=-c -fPIC -Wall -I$(ROOTSYS)/include   # was by Ingolf ????
CCCFLAGS= -c -fPIC -Wall -std=c++11 -I$(ROOTSYS)/include 
FORFLAGS = -c 
#CFLAGS=-c -Wall -I$(ROOTSYS)/include   # was here
LDFLAGS= -g -Wall -v 


OBJSR   = $(O)/mcb-Oct2003u.o  $(O)/unirndhp.o $(O)/root_tree_mcb.o \
	 $(O)/libTUserMCBIO.so
MCBRH   = $(I)/TUserMCBInp.h $(I)/TUserMCBOut.h $(I)/randoms.h $(I)/vec.h


LIBPACK = -L$(cernlib) -lpacklib
LIBMATH = -L$(cernlib)  -lmathlib

ROOTGLIBS = $(shell root-config --glibs)

all:	$(O)/libTUserMCBIO.so mcbu

install: all
	@echo "installing to " $(INSTALL_LOCATION) 
	install -d $(INSTALL_LOCATION) 
	install -C $(O)/libTUserMCBIO.so $(O)/dictMCBInp_rdict.pcm $(O)/dictMCBOut_rdict.pcm $(INSTALL_LOCATION) 

mcbu:	$(B)/mcbu 

#
clean:
	@echo "cleaning up..."
	rm $O/*.o
#	rm $O/$(NAME).o $B/$(NAME)
veryclean:
	@echo "cleaning up all..."
	rm -f $O/*
	rm -f $B/*
	rm -f $C/dict*
$(B)/mcbu: $(OBJSR)	
	@echo "linking..."
	@echo "  "
	$(CCCOMP) $(OBJSR) $(LFLAGS) $(LIBPACK) $(LIBMATH) $(LIBFOR) $(ROOTGLIBS) -o $@
#
#
$(O)/mcb-Oct2003u.o:    $(C)/mcb-Oct2003u.cc $(MCBRH)
	@echo "compiling mcbu..."
	@echo " CFflag : " $(CFLAGS)
	@echo " CCFflag: " $(CCFLAGS)
	@echo " C+flag : " $(C++FLAGS)
	@echo " CCCOMP : " $(CCCOMP)
	@echo " IR : " $(IR)
	@echo " LFLAGS : " $(LFLAGS)
	@echo "  "
	$(CCCOMP) $(C++FLAGS) -I$I -I$(IR) $(C)/mcb-Oct2003u.cc -o $@
$(O)/root_tree_mcb.o:    $(C)/root_tree_mcb.cc $(MCBRH)
	@echo "compiling root_tree_mcb..."
	$(CCCOMP) $(C++FLAGS) -I$I -I$(IR) $(C)/root_tree_mcb.cc -o $@

$(O)/vec.o: $(C)/vec.c $(MCBRH)
	$(CCOMP) $(CFLAGS) -o $@ -I$I $(C)/vec.c
$(O)/randoms.o: $(C)/randoms.c $(MCBRH)
	$(CCOMP) $(CFLAGS) -o $@ -I$I $(C)/randoms.c
$(O)/read_parameter.o: $(C)/read_parameter.c $(MCBRH)
	$(CCOMP) $(CFLAGS) -o $@ -I$I $(C)/read_parameter.c
#
#
$(O)/unirndhp.o: $(F)/unirndhp.f
	@echo "compiling unirndhpf..."
	$(FCOMP) $(FFLAGS) -o $@ $(F)/unirndhp.f
#
#
$(O)/TUserMCBOut.o: $(C)/TUserMCBOut.cc $(MCBRH)
	@echo "compiling tuserMCBout ..."
	$(CCCOMP) $(C++FLAGS) -I$I -I$(IR) $(C)/TUserMCBOut.cc -o $@
$(O)/TUserMCBInp.o: $(C)/TUserMCBInp.cc $(MCBRH)
	@echo "compiling tuserMCBinp ..."
	$(CCCOMP) $(C++FLAGS) -I$I -I$(IR) $(C)/TUserMCBInp.cc -o $@
$(O)/dictMCBOut.o: $(C)/dictMCBOut.cc $(MCBRH)
	@echo "compiling dictMCBout ..."
	$(CCCOMP) $(C++FLAGS) -I$I -I$(IR) $(C)/dictMCBOut.cc -o $@
$(O)/dictMCBInp.o: $(C)/dictMCBInp.cc $(MCBRH)
	@echo "compiling dictMCBinp ..."
	$(CCCOMP) $(C++FLAGS) -I$I -I$(IR) $(C)/dictMCBInp.cc -o $@
#
#
$(O)/libTUserMCBIO.so: $(O)/TUserMCBInp.o $(O)/TUserMCBOut.o \
	$(O)/dictMCBInp.o $(O)/dictMCBOut.o
	@echo "building shared library "
	@echo "         $@ ..."
	@echo "  "
	$(CCCOMP) $(SOFLAGS) -O -o $@ $^
	cp $(C)/dictMCBOut_rdict.pcm $(C)/dictMCBInp_rdict.pcm  $(O)

$(I)/dictMCBOut.h: $(I)/TUserMCBOut.h
$(I)/dictMCBInp.h: $(I)/TUserMCBInp.h

$(C)/dictMCBOut.cc: $(I)/TUserMCBOut.h
	@echo "creating dictionary OUT $* (.h .cc) ..."
	rootcint -v -f $@ -c $^
$(C)/dictMCBInp.cc: $(I)/TUserMCBInp.h
	@echo "creating dictionary INP $* (.h .cc) ..."
	rootcint -v -f $@ -c $^

