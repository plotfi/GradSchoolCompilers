##
## PIN tools
##

##############################################################
#
# Here are some things you might want to configure
#
##############################################################

TARGET_COMPILER?=gnu
ifdef OS
    ifeq (${OS},Windows_NT)
        TARGET_COMPILER=ms
    endif
endif

##############################################################
#
# include *.config files
#
##############################################################

ifeq ($(TARGET_COMPILER),gnu)
    include ../makefile.gnu.config
    LINKER?=${CXX}
    CXXFLAGS ?= -Wall -Werror -Wno-unknown-pragmas $(DBG) $(OPT)
endif

ifeq ($(TARGET_COMPILER),ms)
    include ../makefile.ms.config
    DBG?=
endif

##############################################################
#
# build rules
#
##############################################################

TOOL_ROOTS = sim

all: tools

TOOLS = $(TOOL_ROOTS:%=$(OBJDIR)%$(PINTOOL_SUFFIX))
OBJ_ROOTS = sim.o sim_pin.o trace.o 
OBJS = $(OBJ_ROOTS:%=$(OBJDIR)%)

tools: $(OBJDIR) $(TOOLS)
test: $(OBJDIR) $(TOOL_ROOTS:%=%.test)


## build rules

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o : %.cpp  
	$(CXX) -c $(CXXFLAGS) $(PIN_CXXFLAGS) ${OUTOPT}$@ $<
$(TOOLS): $(PIN_LIBNAMES)
$(TOOLS): $(OBJS) 
	${LINKER} $(PIN_LDFLAGS) $(LINK_DEBUG) ${LINK_OUT}$@ $(OBJS)  ${PIN_LPATHS} $(PIN_LIBS) -lz $(DBG)

	
##
## In this test the tool does repeated mallocs in it's Fini function until it gets a NULL return value
## It tests that PIN's malloc supplied to the tool correctly returns NULL when out of memory
## (This test should run on unix OSes, but is is disabled due to problems on fc5 machile,
## which sends SIGKILL to the running process)
## A separate test is still needed to get PIN to internally exhaust memory and see that PIN
## outputs the "Out of memory" message to the pin logfile before exiting.

memalloc.test: $(OBJDIR)memalloc$(PINTOOL_SUFFIX) memalloc.tested memalloc.failed
	$(PIN) -t $< -- $(TESTAPP) makefile $<.makefile.copy 
	rm memalloc.tested memalloc.failed 
	

## cleaning
clean:
	-rm -rf $(OBJDIR) *.out *.tested *.failed
