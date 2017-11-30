CROSS_COMPILE ?=

CC = $(CROSS_COMPILE)g++
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors -I ../include  
#-std=c++11
# -std=gnu99

LDFLAGS = -ltinyalsa -lpthread 

EXEC=pi_main
OBJS=main.o wav_client.o wav_server.o state_client.o rpi_gpio.o pcm_io.o log4z.o

all: $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC)  $(OBJS)

main.o: main.c 
	$(CC) $(CFLAGS) -c $^

wav_client.o: wav_client.c 
	$(CC) $(CFLAGS) -c $^

wav_server.o: wav_server.c 
	$(CC) $(CFLAGS) -c $^

state_client.o: state_client.c 
	$(CC) $(CFLAGS) -c $^

rpi_gpio.o: rpi_gpio.c 
	$(CC) $(CFLAGS) -c $^

pcm_io.o: pcm_io.c 
	$(CC) $(CFLAGS) -c $^

log4z.o: log4z.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o $(EXEC)

