CC = g++
CFLAGS = `pkg-config --libs --cflags opencv` -O3
BIN = .

OBJS = YUV.o

TARGETS = YuvShow YUVTest
	
BINS = $(TARGETS:%=$(BIN)/%)

all: $(BINS)

$(BINS): $(TARGETS:%=%.c) $(OBJS)
	@echo "Build bin '$@'"
	@g++ -o $@ $(@:%=%.c) $(OBJS) $(CFLAGS)

%.o: %.cpp %.h
	@echo "Build $(@:%.o=%)"
	@g++ $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(BINS) *.o
