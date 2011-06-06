#DEBUG	= -g -DDEBUG
OPT	= -O3 -funroll-loops -D__USE_STRING_INLINES -fhosted
OPT	+= -DEPOLL_MODE
#OPT	+= -DNS_CLIENT

AR = ar
AROPT=-curv

CC	= gcc
CPP	= g++
CPPFLAGS   = -g -Wall $(INC) -DENABLE_DEBUG_MODE
LOGTEST	= logtest
SERVERTEST = servertest
HTTPTEST = httptest
SRCS 	= logtest.c log.c util.c 
#BINS = $(LOGTEST) $(SERVERTEST)
#BINS = $(HTTPTEST)
BINS = $(LIB_transfer)
LIB_transfer = libhttptransfer.a

TOOLS   = log.o util.o

INCLUDE_HOME = ..
INC     =  -I. -I${INCLUDE_HOME}/include 

CFLAGS	= -g -Wall -MMD $(DEBUG) -D_REENTRANT -D_GNU_SOURCE $(OPT) -z defs 

LIBS	+= 

all: $(BINS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<
	@-mv -f $*.d .dep.$@

%.o: %.cpp Makefile
	$(CPP) $(CPPFLAGS) -c -o $@ $< 
#	@-mv -f $*.d .dep.$@

$(HTTPTEST): httpmanager.o transferworker.o task.o HttpCli.o Socket.o SockProxy.o Url.o Utils.o Types.o
	$(CC)  $(LDFLAGS) -c $@ $^ $(LIBS)

$(LOGTEST): $(SRCS:.c=.o)
	$(CC)  $(LDFLAGS) -o $@ $^ $(LIBS)

$(LIB_transfer): httpmanager.o transferworker.o task.o HttpCli.o Socket.o SockProxy.o Url.o Utils.o Types.o
	$(AR)  $(AROPT) $@ $? $(LIBS)

$(SERVERTEST): log.o util.o servertest.o
	$(CC)  $(LDFLAGS) -o $@ $^ $(LIBS)

#	rm ../bin/$(BINS); mv $(BINS) ../bin/
clean:
	@-rm -f *.o *.po *.so *.d .dep.* $(BINS) core

-include /dev/null $(wildcard .dep.*)
