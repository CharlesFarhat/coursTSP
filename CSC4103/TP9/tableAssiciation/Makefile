BIN=test_map_tree #test_map_tree test_map_rb_tree

all: $(BIN)

test_map: test_map.o map.o
	$(CC) -o test_map test_map.o map.o $(LDFLAGS)

test_map_tree: test_map.o map_tree.o
	$(CC) -o test_map_tree test_map.o map_tree.o $(LDFLAGS)

test_map_rb_tree: test_map.o map_rb_tree.o
	$(CC) -o test_map_rb_tree test_map.o map_rb_tree.o $(LDFLAGS)

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -f $(BIN) *.o
