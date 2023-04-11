BIN := server client
CFLAGS := -Wall
LDFLAGS := -lpthread

all: $(BIN)

server: server.o server_code_etudiant.o
	$(CC) -o server server.o server_code_etudiant.o $(LDFLAGS)

client: client_code_etudiant.o client.o
	$(CC) -o client client.o client_code_etudiant.o $(LDFLAGS)

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -f $(BIN) *.o