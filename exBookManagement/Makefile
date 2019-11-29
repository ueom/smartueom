TARGET=bookManagement
CFLAGS= $(shell mysql_config --cflags)
LDFLAGS= $(shell mysql_config --libs)

all:
	gcc -o $(TARGET) $(TARGET).c $(CFLAGS) $(LDFLAGS) 

clean:
	@rm -f $(TARGET) *~
