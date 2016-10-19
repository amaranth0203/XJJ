CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g
LDFLAGS=-g 
LDLIBS=

SRCS=XJJ_server.c XJJ_util.c
OBJS=$(subst .c,.o,$(SRCS))

ifeq ($(debug),1)
	CPPFLAGS+=-D_DEBUG_ -DEBUG -O0
endif

ifeq ($(release),1)
	LDFLAGS+=-DNDEBUG -O3 -s
endif

all: tool

tool: $(OBJS)
	$(CXX) $(LDFLAGS) -o server $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
