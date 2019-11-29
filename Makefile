# David Longnecker
# 5/26/2019
# 
# TODO
#
# - This really should compile object files into a separate intermediate
#   directory so that we can speed up compilation.
# - We ought to check to see if CHASM_HOME is set rather than hardcoding
#   it ourselves.
#
# TO BUILD A SHARED LIBRARY!
# gcc -g -fPIC -Wall -Werror -Wextra -pedantic *.c -shared -o liball.so
#

# Path to current Makefile. Uses _HACK_ to chop off last slash.
WRAW=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
WHERE=$(WRAW:%/=%)

# Get the name of the current operating system.
UNAME=$(shell uname)

# Project specific file/compiler info.
CC=gcc
STD=c99
EXT=c
HDR=h

# WARN: Path is relative to Makefile location (top level)!
BIN_DIR=$(WHERE)/bin
OBJ_DIR=$(WHERE)/obj

# Info fields for the VM library.
LIB_NAME=libcsm.so
LIB_LIB=
LIB_LIBPATH=
LIB_INC=include

LIB_GLIB=$(LIB_LIB:%=-l%)
LIB_GLIBPATH=$(LIB_LIBPATH:%=-L%)
LIB_GLINKPATH=$(LIB_LIBPATH:%=-Wl,-rpath=%)
LIB_GINC=$(LIB_INC:%=-I%)
LIB_GLOB=$(LIB_GLIB) $(LIB_GLIBPATH) $(LIB_GLINKPATH) $(LIB_GINC)

LIB_FOPT=
LIB_FWARN=-Wall -Werror -Wextra -pedantic
LIB_FBUILD=-g -fPIC -shared

LIB_CFLAGS=-std=$(STD) $(LIB_GLOB) $(LIB_FOPT) $(LIB_FWARN) $(LIB_FBUILD)
LIB_LDFLAGS=

LIB_SRC_DIR=src
LIB_SRC=$(wildcard $(LIB_SRC_DIR)/*.$(EXT))
LIB_BIN=

# Calling these unit, these are "single source file binaries".
U_PREFIX=csm
U_LIB=csm
U_LIBPATH=$(BIN_DIR)
U_INC=include

U_GLIB=$(U_LIB:%=-l%)
U_GLIBPATH=$(U_LIBPATH:%=-L%)
U_GLINKPATH=$(U_LIBPATH:%=-Wl,-rpath=%)
U_GINC=$(U_INC:%=-I%)
U_GLOB=$(U_GLIBPATH) $(U_GLIB) $(U_GLINKPATH) $(U_GINC) $(U_GLIB)

U_FOPT=
U_FWARN=-Wall -Werror -Wextra -pedantic
U_FBUILD=-g

U_CFLAGS=-std=$(STD) $(U_GLOB) $(U_FOPT) $(U_FWARN) $(U_FBUILD)
U_LDFLAGS=

U_SRC_DIR=driver
U_SRC=$(wildcard $(U_SRC_DIR)/*.$(EXT))
U_BIN=$(U_SRC:$(U_SRC_DIR)/%.$(EXT)=$(BIN_DIR)/$(U_PREFIX)-%)

# Crap info for compiling tests.
TEST_DIR=test
TEST_SRC=$(wildcard $(TEST_DIR)/*.chasm)
TEST_DUMP=dump

.PHONY: all
all: check_chasm init libcsm unit test

.PHONY: check_chasm
check_chasm:
ifeq ($(CHASM_HOME),)
	$(error The location of CHASM_HOME must be set to continue)
endif

# TODO: Make this take advantage of the Makefile dependency graph.
.PHONY: libcsm
libcsm: init
	@echo "-- Building VM shared library..."
	$(CC) -o $(BIN_DIR)/$(LIB_NAME) \
		$(LIB_SRC) \
		$(LIB_CFLAGS) $(LIB_LDFLAGS) $(LIB_LIB)

.PHONY: unit
unit: libcsm $(U_BIN)

$(U_BIN): $(BIN_DIR)/$(U_PREFIX)-% : $(U_SRC_DIR)/%.$(EXT)
	@echo "-- Building unit: " $@
	$(CC) -o $@ $< $(U_CFLAGS) $(U_LDFLAGS)

.PHONY: test
test: $(TEST_SRC)
	@mkdir -p $(TEST_DUMP)
	@echo "-- Building VM test files..."
	python3 $(CHASM_HOME)/chasm.py $(TEST_SRC) -or:$(TEST_DUMP)
	@echo "DONE!"

.PHONY: init
init:
	@mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	@test -n $(BIN_DIR) && rm -rf $(BIN_DIR)/* \
		rmdir $(BIN_DIR)

# Won't ever fire, at this rate.
%.chasm:
	@echo "-- Compiling: " $@
	python3 $(CHASM_HOME)/chasm.py $@ -or:$(TDUMP) -dsegments

