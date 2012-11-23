CC = g++
CFLAGS = `pkg-config --libs --cflags opencv` -O3 -Wall
BIN = .

OBJS =
OBJS += YuvFrame.o
OBJS += YuvReader.o
OBJS += YuvDisplay.o
#OBJS += YuvEffects.o
OBJS += YuvWriter.o
OBJS += BitStream.o
#OBJS += TSubSampler.o
OBJS += Block.o
OBJS += Golomb.o
OBJS += Predictor.o
OBJS += IntraCoder.o
#OBJS += HybCoder.o

TARGETS =
TARGETS += YuvShow
#TARGETS += TestBitStream
#TARGETS += TestGolomb
#TARGETS += TestCoder
#TARGETS += TestBlocks

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
