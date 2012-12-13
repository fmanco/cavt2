CC = g++
CXXFLAGS = -O3 -Wall -I$(INC_BASE_DIR) `pkg-config --libs --cflags opencv`
#CXXFLAGS = -g -O0 -Wall -I$(INC_BASE_DIR) `pkg-config --libs --cflags opencv` #DEBUG

APP_BASE_DIR = app
INC_BASE_DIR = inc
LIB_BASE_DIR = lib

APPS =
APPS += YuvShow
#APPS += TestFrame
#APPS += TestGolomb
#APPS += TestBlocks
#APPS += TestHyb
#APPS += TestDCT
APPS += TestDCTCoder
#APPS += TestIntraCoder
APPS += EncodeIntraPred
APPS += DecodeIntraPred
APPS += EncodeIntraDCT
APPS += DecodeIntraDCT
APPS += EncodeHybrid
APPS += DecodeHybrid
APPS += CompPSNR
APPS += CalcEntropy

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
LIBS += InterCoder.o
LIBS += HybCoder.o
LIBS += DCTCoder.o
LIBS += YuvAnalyse.o

APPS    := $(patsubst %,$(APP_BASE_DIR)/%,$(APPS))
LIBS    := $(patsubst %.o,$(LIB_BASE_DIR)/%.o,$(LIBS))


all: $(APPS)

$(APPS): %: %.cpp $(LIBS) $(HEADERS)
	@echo "Build bin '$@'"
	@g++ -o $@ $(@:%=%.cpp) $(LIBS) $(CXXFLAGS)

%.o: %.cpp $(HEADERS)
	@echo "Build lib '$(@:%.o=%)'"
	@g++ $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(APPS) $(APP_BASE_DIR)/*.o $(LIB_BASE_DIR)/*.o *~

.PHONY: clean all
