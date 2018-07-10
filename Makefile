################################################################################
#
#	lib-ppd : PORTABLE PROGRAM DATA LIBRARY
#
#	Copyright (c) 2018 Ammon Dodson
#	You should have received a copy of the licence terms with this software. If
#	not, please visit the project homepage at:
#	https://github.com/ammon0/lib-ppd
#
################################################################################


# Change these variables to point to the appropriate installation directories
WORKDIR   :=./work
INSTALLDIR:=$(HOME)/prg
LIBDIR    :=$(INSTALLDIR)/lib
INCDIR    :=$(INSTALLDIR)/include


#################################### FLAGS #####################################


# My code builds without warnings--ALWAYS
CWARNINGS:=-Wall -Wextra -pedantic \
	-Wmissing-prototypes -Wstrict-prototypes -Wmissing-declarations \
	-Wredundant-decls  -Wnested-externs \
	-Wshadow -Wbad-function-cast \
	-Wcast-align \
	-Wdeclaration-after-statement \
	-Winline \
	-Wswitch-default -Wswitch-enum \
	-Wsuggest-attribute=pure -Wsuggest-attribute=const \
	-Wsuggest-attribute=noreturn -Wsuggest-attribute=format \
	-Wtrampolines -Wstack-protector \
	-Wwrite-strings \
	-Wdisabled-optimization \
	-Wpadded \
	-Wconversion # -Wc++-compat

CXXWARNINGS:=-Wall -Wextra -pedantic \
	-Wmissing-declarations \
	-Wredundant-decls -Wshadow \
	-Wpointer-arith -Wcast-align \
	-Wuninitialized -Wmaybe-uninitialized \
	-Winline -Wno-long-long \
	-Wsuggest-attribute=pure -Wsuggest-attribute=const \
	-Wsuggest-attribute=noreturn -Wsuggest-attribute=format \
	-Wtrampolines -Wstack-protector \
	-Wwrite-strings \
	-Wdisabled-optimization -Wno-switch \
	-Wswitch -Wswitch-default -Wswitch-enum \
	#-Wconversion

DEBUG_OPT:=

CFLAGS:=  --std=c11   -g $(CWARNINGS)   -I./ -I$(INCDIR) -L$(LIBDIR)/util
CXXFLAGS:=--std=c++14 -g $(CXXWARNINGS) -I./ -I$(INCDIR) -L$(LIBDIR)
LFLAGS:=#-d
LEX:= flex


############################### FILES AND FOLDERS ##############################


srcdir    :=./src
headerdir :=./ppd

headers    :=$(wildcard $(headerdir)/*.h)
cpp_sources:=$(wildcard $(srcdir)/*.cpp)
c_sources  :=$(wildcard $(srcdir)/*.c)
prv_headers:=$(wildcard $(srcdir)/*.hpp) $(wildcard $(srcdir)/*.h)

allfiles:= $(headers) $(cpp_sources) $(c_sources) $(prv_headers)

flex_l := $(srcdir)/dyn.l
flex_c := $(srcdir)/dyn.l.c

# Object files
flex_o := dyn.l.o
omem_o := object_mem.o
parse_o:= dynParser.o
c_o    := string_table.o syms.o
cpp_o  := gen-pexe.o

tests:=testMemory testScanner testParser


# Prefix the object files
flex_o :=$(addprefix $(WORKDIR)/, $(flex_o))
omem_o :=$(addprefix $(WORKDIR)/, $(omem_o))
parse_o:=$(addprefix $(WORKDIR)/, $(parse_o))
c_o    :=$(addprefix $(WORKDIR)/, $(c_o))
cpp_o  :=$(addprefix $(WORKDIR)/, $(cpp_o))


c_o += $(omem_o) $(parse_o)


################################### TARGETS ####################################


.PHONEY: docs debug install

debug: $(tests) $(pexe_objects)


################################# PRODUCTIONS ##################################


testMemory: $(srcdir)/testMemory.c $(omem_o)
	$(CC) $(CFLAGS) -o $@ $< -lmsg $(omem_o)
	chmod +x $@

testScanner: $(srcdir)/testScanner.c $(flex_o)
	$(CC) $(CFLAGS) -o $@ $< -lmsg $(flex_o)
	chmod +x $@

testParser:$(srcdir)/testParser.c $(parse_o)
	$(CC) $(CFLAGS) -o $@ $< -lmsg $(parse_o) $(flex_o)
	chmod +x $@

libppd.a: $(cpp_o) $(c_o) $(flex_o)
	ar rcs $@ $(cpp_o) $(c_o) $(flex_o)

# objects

$(cpp_o): $(WORKDIR)/%.o: $(srcdir)/%.cpp $(headers) $(prv_headers) | $(WORKDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(c_o): $(WORKDIR)/%.o: $(srcdir)/%.c $(headers) $(prv_headers) | $(WORKDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(flex_o): $(flex_c)
	$(CC) $(CFLAGS) -Wno-conversion -c -o $@ $<

$(flex_c): $(flex_l)
	$(LEX) $(LFLAGS) -o $@ $<

# misc

$(WORKDIR):
	mkdir -p $@

docs: Doxyfile README.md $(allfiles)
	doxygen Doxyfile

install: $(headers) libppd.a
	install -d $(LIBDIR) $(INCDIR)/ppd
	install -C libppd.a $(LIBDIR)
	for f in $(headers)  ; do install -C $$f $(INCDIR)/ppd; done


################################## UTILITIES ###################################


cleanfiles:=*.a *.o src/dyn.l.c $(tests)

.PHONEY: clean todolist count veryclean

veryclean: clean
	rm -r ./docs/

clean:
	rm -f $(cleanfiles)
	rm -fr $(WORKDIR)

todolist:
	-@for file in $(allfiles:Makefile=); do fgrep -nH -e TODO -e FIXME $$file; done; true

count:
	cat $(allfiles) |line_count


