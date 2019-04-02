SRCS=$(wildcard *.cpp)
BIN_SRCS=$(shell grep "int main" -l *.cpp) 
LIB_SRCS=$(patsubst $(BIN_SRCS),,$(SRCS))
LIB_DIR=-L../lib

DEPS=$(SRCS:.cpp=.d) 
OBJS=$(SRCS:.cpp=.o) 

LIB_OBJS=$(patsubst %.cpp,%.o,$(LIB_SRCS)) 
BINS=$(patsubst %.cpp,%.bin,$(BIN_SRCS))
DEP_LIBS=

LIB_NAME=
SO_LIB=$(patsubst %,lib%.so,$(LIB_NAME))
STATIC_LIB=$(patsubst %,lib%.a,$(LIB_NAME))

APP_VERSION=$(shell git show --format="%H" --quiet)

CC=g++
CFLAGS= -std=c++11 -Wall -D_REENTRANT -D_GNU_SOURCE -fPIC -DAPP_VERSION=\"$(APP_VERSION)\"
INC= -I. -I../../comm
LDFLAGS=-lcomm 

ifeq ($(release), 1)
CFLAGS += -O2 -DNDEBUG 
else
CFLAGS += -g -DDEBUG 
endif

all: $(BINS) $(SO_LIB) $(STATIC_LIB) 

$(BINS): %.bin : %.o $(LIB_OBJS) $(DEP_LIBS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIB_DIR)
$(SO_LIB): $(LIB_OBJS) 
	$(CC) $(CFLAGS) -shared -o $@ $^ 
$(STATIC_LIB): $(LIB_OBJS) 
	ar -rcs -o $@ $^ 
dist :
	rm -f $(DEPS)
clean:
	rm -f $(BINS) $(OBJS) $(DEPS) $(SO_LIB) $(STATIC_LIB)
release: clean
	@make release=1

%.d : %.cpp
	$(CC) $(CFLAGS)  -MT $(subst .cpp,.o,$<) -MM $(INC)  $< >$@
%.o : %.cpp
	$(CC) $(CFLAGS) $(INC) -c  $< 

-include  $(DEPS)

