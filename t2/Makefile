CC = g++
CXXFLAGS = -O3 -Wall -I$(INC_BASE_DIR) `pkg-config --libs --cflags opencv`

APP_BASE_DIR = app
INC_BASE_DIR = inc
LIB_BASE_DIR = lib

APPS =
APPS += YuvShow
#APPS += TestFrame
#APPS += TestBitStream
#APPS += TestGolomb
#APPS += TestCoder
#APPS += TestBlocks
APPS += TestHyb
#APPS += TestDCT
APPS += TestDCTCoder
#APPS += TestIntraCoder
APPS += EncodeIntraPred
APPS += DecodeIntraPred
APPS += EncodeIntraDCT
APPS += DecodeIntraDCT

HEADERS = $(shell find $(INC_BASE_DIR) -name '*.h' -type f)

LIBS =
LIBS += YuvFrame.o
LIBS += YuvReader.o
LIBS += YuvDisplay.o
#LIBS += YuvEffects.o
LIBS += YuvWriter.o
LIBS += BitStream.o
#LIBS += TSubSampler.o
LIBS += Block.o
LIBS += Golomb.o
LIBS += Predictor.o
LIBS += IntraCoder.o
LIBS += HybCoder.o
LIBS += DCTCoder.o


APPS    := $(patsubst %,$(APP_BASE_DIR)/%,$(APPS))
LIBS    := $(patsubst %.o,$(LIB_BASE_DIR)/%.o,$(LIBS))
HEADERS := $(patsubst %.o,$(INC_BASE_DIR)/%.o,$(HEADERS))


all: $(APPS)

$(APPS): $(APPS:%=%.cpp) $(LIBS) $(HEADERS)
	@echo "Build bin '$@'"
	@g++ -o $@ $(@:%=%.cpp) $(LIBS) $(CXXFLAGS)

%.o: %.cpp $(HEADERS)
	@echo "Build lib '$(@:%.o=%)'"
	@g++ $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(APPS) $(APP_BASE_DIR)/*.o $(LIB_BASE_DIR)/*.o *~

.PHONY: clean all
