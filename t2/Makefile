CC = g++
CFLAGS = `pkg-config --libs --cflags opencv` -O3
BIN = .

OBJS = YuvFrame.o                                \
       YuvReader.o                               \
       YuvEffects.o                              \
       YuvWriter.o BitStream.o YuvDisplay.o      \
       TSubSampler.o Block.o

TARGETS = a

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
