CC = gcc
CFLAGS = -W -Wall -fPIC
SRCS = chrono.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
TARGET = chrono
TARGETAR = lib$(TARGET).a
TARGETSO = lib$(TARGET).so

.PHONY: all depend clean
.SUFFIXES: .c .o .c .d
.DEFAULT: all

all: $(TARGETAR) $(TARGETSO)

$(TARGETAR): $(DEPS) $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(TARGETSO): $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS)
clean:
	rm -rf *.o *.d *.so *.a

.c.o:
	$(CC) $(CFLAGS) -c $<

.c.d:
	$(CC) -E -MM -w $< > $@

-include: $(DEPS)
