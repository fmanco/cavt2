CC = g++
CFLAGS = `pkg-config --libs --cflags opencv` -O3
BIN = .

OBJS = YUV.o YuvResize.o

TARGETS = YuvShow YuvCmp
	
BINS = $(TARGETS:%=$(BIN)/%)

all: $(BINS)

$(BINS): $(TARGETS:%=%.cpp) $(OBJS)
	@echo "Build bin '$@'"
	@g++ -o $@ $(@:%=%.cpp) $(OBJS) $(CFLAGS)

%.o: %.cpp %.h
	@echo "Build $(@:%.o=%)"
	@g++ $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(BINS) *.o *~
