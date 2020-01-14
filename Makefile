#<===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
## File Name:    Makefile
## Author:       Hao-Kai SUN
## Authors:      Thomas Latham, John Back, Paul Harrison
## Created:      2020-01-12 Sun 20:28:15 CST
## <<=====================================>>
## Last Updated: 2020-01-14 Tue 19:07:48 CST
##           By: Hao-Kai SUN
##     Update #: 159
## <<======== COPYRIGHT && LICENSE =======>>
##
## Copyright University of Warwick 2004 - 2013.
## Copyright © 2020 SUN Hao-Kai <spin.hk@outlook.com>. All rights reserved.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or (at
## your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.
##
## ============================== CODES ==============================>>>
# Set Library/Binary information --------------------------------
PACKAGE=QuickFit
SONAME=lib$(PACKAGE).so

# Check whether ROOTSYS is defined ------------------------------
ifndef ROOTSYS
  ROOTSYS := $(shell root-config --prefix)
  ROOTBINDIR := $(shell root-config --bindir)
  ifeq ($(ROOTSYS), )
    $(error running of root-config failed or reported null value)
  endif
else
  ROOTBINDIR := $(ROOTSYS)/bin
endif

ROOTCONFIG  := $(ROOTBINDIR)/root-config
ARCH        := $(shell $(ROOTCONFIG) --arch)
PLATFORM    := $(shell $(ROOTCONFIG) --platform)
ROOTVERSION := $(shell $(ROOTCONFIG) --version | awk -F. '{print $$1}')
## Check ROOT version
ifeq ($(ROOTVERSION),5)
	$(error ROOT version used: $(ROOTVERSION)! Please use the latest!)
endif
ROOTLIBD    := $(shell $(ROOTCONFIG) --libdir)

# Directory structure -------------------------------------------
INC_DIR = inc
SRC_DIR = src

WORK_DIR = tmp
DEP_DIR=$(WORK_DIR)/dep
OBJ_DIR=$(WORK_DIR)/obj

LIB_DIR = lib
BIN_DIR = bin

SKIPLIST = LauRooFitSlave

ROOTINC = $(shell $(ROOTCONFIG) --incdir)
INCLUDES = -I$(ROOTINC) -I$(INC_DIR)

# Compilers and Flags ------------------------------------------
CXX = g++

DEBUGFLAGS = -Wall -Wextra -Wshadow -Woverloaded-virtual -g -Og -fPIC
OPTFLAGS = -march=native -O2 -pipe -m64 -fPIC -fno-plt -std=c++17
ifeq (debug,$(findstring debug,$(BUILDTYPE)))
	BUILDFLAGS = $(DEBUGFLAGS)
else
	BUILDFLAGS = $(OPTFLAGS)
endif

ROOTCFLAGS = $(shell $(ROOTCONFIG) --cflags)
CXXFLAGS = $(ROOTCFLAGS) $(BUILDFLAGS) $(INCLUDES)
# CXXFLAGS = $(ROOTCFLAGS) $(BUILDFLAGS)
# CXXFLAGS = -g -O2 -Wall -Wextra -Wshadow -Woverloaded-virtual -w -fPIC
ROOTLIBS = $(shell $(ROOTCONFIG) --libs --glibs)
CXXLIBS = $(ROOTLIBS) -lCore -lEG -lHist -lMathCore -lMatrix -lNet -lRIO -lTree -lRooFit -lRooFitCore -lRooStats -lMinuit -lFoam -lGpad
MFLAGS = -MM

# Linker and Flags ---------------------------------------------
LD = g++
ROOTLDFLAGS = $(shell $(ROOTCONFIG) --ldflags)
LDFLAGS = $(ROOTLDFLAGS) -rdynamic -shared -fPIC
BINLDFLAGS = $(ROOTLDFLAGS) -fPIC

CINTFILE  = $(WORK_DIR)/$(PACKAGE)Cint.cc
CINTOBJ   = $(OBJ_DIR)/$(PACKAGE)Cint.o
LIBFILE   = $(LIB_DIR)/lib$(PACKAGE).a
SHLIBFILE = $(LIB_DIR)/$(SONAME)
ROOTMAPFILE = $(patsubst %.so,%.rootmap,$(SHLIBFILE))

# Collect list of files ----------------------------------------
## List of all header files
HHLIST:=$(filter-out $(addprefix $(INC_DIR)/, $(addsuffix .hh, $(SKIPLIST))),$(wildcard $(INC_DIR)/*.hh))

## List of all source files to build
CCLIST:=$(filter-out $(addprefix $(SRC_DIR)/, $(addsuffix .cc, $(SKIPLIST))),$(wildcard $(SRC_DIR)/*.cc))

## List of all source files that contain main functions
BINCCLIST:=$(shell egrep -l "^[[:space:]]*int[[:space:]]*main\>" $(CCLIST))

## List of all source files to be compiled into the library
LIBCCLIST:=$(filter-out $(BINCCLIST), $(CCLIST))

## List of all object files to build
OOLIST:=$(patsubst %.cc,%.o,$(addprefix $(OBJ_DIR)/,$(notdir $(CCLIST))))

## List of all object files to build
LOLIST:=$(patsubst %.cc,%.o,$(addprefix $(OBJ_DIR)/,$(notdir $(LIBCCLIST))))

## List of all dependency files to make
DDLIST:=$(patsubst %.cc,%.d,$(addprefix $(DEP_DIR)/,$(notdir $(CCLIST))))

## List of all binary files to make
BINLIST:=$(patsubst %.cc,%,$(notdir $(BINCCLIST)))

# Makefile Rules -----------------------------------------------
.PHONY: lib shlib bin clean all

all: lib shlib bin

## Implicit rule making all dependency Makefiles
## included at the end of this makefile
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cc
	@echo "Making $@"
	@bash setup.sh
	@mkdir -pv $(DEP_DIR)
	@set -e; $(CXX) $(MFLAGS) $(CXXFLAGS) $< \
	          | sed 's#\($(notdir $*)\)\.o[ :]*#$(OBJ_DIR)/\1.o $@ : #g' > $@; \
	        [ -s $@ ] || rm -f $@

## Implicit rule to compile all classes
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	@echo "Compiling $<"
	@mkdir -pv $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

## Rule to make ROOTCINT output file
$(CINTOBJ) : $(HHLIST) $(INC_DIR)/$(PACKAGE)_LinkDef.h
	@mkdir -pv $(OBJ_DIR)
	@mkdir -pv $(LIB_DIR)
	@echo "Running rootcling"
	@$(ROOTBINDIR)/rootcling -f $(CINTFILE) -s $(SHLIBFILE) -rml $(SHLIBFILE) $(addprefix -rml , $(ROOTLIBS)) -rmf $(ROOTMAPFILE) -I$(PWD)/$(INC_DIR) $(notdir $(HHLIST)) $(INC_DIR)/$(PACKAGE)_LinkDef.h
	@echo "Compiling $(CINTFILE)"
	@$(CXX) $(CXXFLAGS) -c $(CINTFILE) -o $(CINTOBJ)

## Rule to combine objects into a library
$(LIBFILE) : $(LOLIST) $(CINTOBJ)
	@echo "Making $(LIBFILE)"
	@mkdir -p $(LIB_DIR)
	@rm -f $(LIBFILE)
	@ar rcs $(LIBFILE) $(LOLIST) $(CINTOBJ)

## Rule to combine objects into a shared library
$(SHLIBFILE) : $(LOLIST) $(CINTOBJ)
	@echo "Making $(SHLIBFILE)"
	@mkdir -p $(LIB_DIR)
	@rm -f $(SHLIBFILE)
	@$(CXX) $(LOLIST) $(CINTOBJ) $(LDFLAGS) $(CXXLIBS) -o $(SHLIBFILE)

## Useful build targets
lib: $(LIBFILE)

shlib: $(SHLIBFILE)

## Rule to compile all binaries
%: $(OBJ_DIR)/%.o $(CINTOBJ) $(SHLIBFILE)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $@"
	@$(CXX) $(BINLDFLAGS) $< -o $(BIN_DIR)/$@ $(CINTOBJ) $(SHLIBFILE) $(CXXLIBS)

bin: $(BINLIST)

clean:
	@rm -rvf $(WORK_DIR)
	@rm -vf $(LIBFILE)
	@rm -vf $(SHLIBFILE)
	@rm -vf $(BINLIST)
	@rm -vf $(ROOTMAPFILE)

-include $(DDLIST)
#===================================================================<<<
#========================= Makefile ends here =========================
