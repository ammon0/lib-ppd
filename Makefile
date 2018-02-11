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


#################################### FILES #####################################


srcdir    :=./src
headerdir :=./ppd

# Change these variables to point to the appropriate installation directories
WORKDIR   :=./work
INSTALLDIR:=$(HOME)/prg
LIBDIR    :=$(INSTALLDIR)/lib
INCDIR    :=$(INSTALLDIR)/include

headers    :=$(wildcard $(headerdir)/*.hpp)
cpp_sources:=$(wildcard $(srcdir)/*.cpp)
c_sources  :=$(wildcard $(srcdir)/*.c)
prv_headers:=$(wildcard $(srcdir)/*.hpp) $(wildcard $(srcdir)/*.h)

allfiles:= $(headers) $(cpp_sources) $(c_sources) $(prv_headers)

# Object files
ppd_objects :=routine.o ppd.o definitions.o
pexe_objects:=gen-pexe.o read-pexe.o
mpl_objects:=gen-mpl.o read-mpl.o 
opt_objects :=opt-dead.o
flex_objects:=mpl.l.o

# Prefix the object files
ppd_objects :=$(addprefix $(WORKDIR)/, $(ppd_objects) )
pexe_objects:=$(addprefix $(WORKDIR)/, $(pexe_objects))
mpl_objects :=$(addprefix $(WORKDIR)/, $(mpl_objects) )
opt_objects :=$(addprefix $(WORKDIR)/, $(opt_objects) )
flex_objects:=$(addprefix $(WORKDIR)/, $(flex_objects))

cpp_objects:= $(ppd_objects) $(mpl_objects) $(pexe_objects) $(opt_objects)
c_objects  := $(flex_objects)


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
	-Wc++-compat -Wpadded 
	#-Wconversion -Werror=implicit-function-declaration

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
	-Wconversion -Wdisabled-optimization -Wno-switch
#	-Wswitch -Wswitch-default -Wswitch-enum

DEBUG_OPT:=

CFLAGS:=  --std=c11   -g $(CWARNINGS)   -I./ -I$(INCDIR) -L$(LIBDIR)
CXXFLAGS:=--std=c++14 -g $(CXXWARNINGS) -I./ -I$(INCDIR) -L$(LIBDIR)
LFLAGS:=#-d
LEX:= flex


################################### TARGETS ####################################


.PHONEY: docs debug

debug: libppd.a


################################# PRODUCTIONS ##################################


libppd.a: $(ppd_objects) $(mpl_objects) $(flex_objects)
	ar rcs $@ $(ppd_objects) $(mpl_objects)

$(srcdir)/mpl.l.c: $(srcdir)/mpl.l
	$(LEX) $(LFLAGS) -o $@ $<

docs: Doxyfile README.md $(allfiles)
	doxygen Doxyfile

$(cpp_objects): $(WORKDIR)/%.o: $(srcdir)/%.cpp $(headers) $(prv_headers) | $(WORKDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(c_objects): $(WORKDIR)/%.o: $(srcdir)/%.c $(headers) $(prv_headers) | $(WORKDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# working directory
$(WORKDIR):
	mkdir -p $@

install: $(headers) libppd.a
	install -d $(LIBDIR) $(INCDIR)/ppd
	install -C libppd.a $(LIBDIR)
	for f in $(headers)  ; do install -C $$f $(INCDIR)/ppd; done


################################## UTILITIES ###################################


cleanfiles:=*.a *.o

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


