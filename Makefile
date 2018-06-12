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
	-Wdeclaration-after-statement -Werror=uninitialized \
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
	-Wmissing-declarations -Werror=implicit-function-declaration \
	-Wredundant-decls -Wshadow \
	-Wpointer-arith -Wcast-align \
	-Wuninitialized -Wmaybe-uninitialized -Werror=uninitialized \
	-Winline -Wno-long-long \
	-Wsuggest-attribute=pure -Wsuggest-attribute=const \
	-Wsuggest-attribute=noreturn -Wsuggest-attribute=format \
	-Wtrampolines -Wstack-protector \
	-Wwrite-strings \
	-Wconversion -Wdisabled-optimization -Wno-switch \
	-Wswitch -Wswitch-default -Wswitch-enum

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

# Object files
flex_objects :=dyn.l.o
omem_objects :=object_mem.o
pexe_objects :=string_table.o
parse_objects:=dynParser.o

tests:=testMemory testScanner testParser


# Prefix the object files
flex_objects  :=$(addprefix $(WORKDIR)/, $(flex_objects))
omem_objects  :=$(addprefix $(WORKDIR)/, $(omem_objects))
pexe_objects  :=$(addprefix $(WORKDIR)/, $(pexe_objects))
parse_objects :=$(addprefix $(WORKDIR)/, $(parse_objects))

#tests:=$(addprefix $(WORKDIR)/, $(tests))


cpp_objects:= $(pexe_objects)
c_objects  := $(flex_objects) $(omem_objects) $(parse_objects)


################################### TARGETS ####################################


.PHONEY: docs debug

debug: $(tests) $(pexe_objects)

################################# PRODUCTIONS ##################################


# working directory
$(WORKDIR):
	mkdir -p $@

$(cpp_objects): $(WORKDIR)/%.o: $(srcdir)/%.cpp $(headers) $(prv_headers) | $(WORKDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(c_objects): $(WORKDIR)/%.o: $(srcdir)/%.c $(headers) $(prv_headers) | $(WORKDIR)
	$(CC) $(CFLAGS) -c -o $@ $<


testMemory: $(srcdir)/testMemory.c $(omem_objects)
	$(CC) $(CFLAGS) -o $@ $< -lmsg $(omem_objects)
	chmod +x $@

testScanner: $(srcdir)/testScanner.c $(flex_objects)
	$(CC) $(CFLAGS) -o $@ $< -lmsg $(flex_objects)
	chmod +x $@

testParser:$(srcdir)/testParser.c $(parse_objects)
	$(CC) $(CFLAGS) -o $@ $< -lmsg $(parse_objects) $(flex_objects)
	chmod +x $@

libppd.a: $(ppd_objects) $(mpl_objects) $(flex_objects)
	ar rcs $@ $(ppd_objects) $(mpl_objects)

$(srcdir)/dyn.l.c: $(srcdir)/dyn.l
	$(LEX) $(LFLAGS) -o $@ $<

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


