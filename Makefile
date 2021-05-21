TARGET = main
CSRC = $(TARGET).c
CSRC += server.c DBHandler.c log.c user_menu.c
COBJ = $(CSRC:.c=.o)

CC=gcc
CFLAGS=-m64 `mysql_config --cflags`
LDFLAGS=`mysql_config --libs` -lpthread

all: $(TARGET).out

$(TARGET).out: $(COBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(COBJ:%.o): %.c
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	rm -f $(TARGET).out $(COBJ)

.PHONY: clean all