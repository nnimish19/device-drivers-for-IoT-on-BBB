CC = gcc
CFLAGS = -I/usr/local/include/libxml2 -L/usr/local/lib -lxml2
OBJ = ddl_parser.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) 
	
ddl_parser: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)