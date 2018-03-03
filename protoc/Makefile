
TARGET= my

CFLAGS = -Ia
LDFLAGS = $(shell pkg-config --libs "protobuf")
target: $(TARGET)

%.o: %.cpp
	g++ $(CFLAGS) $< -c
my: my.o a/my.pb.o
	g++ $^ -o $@ $(LDFLAGS)
