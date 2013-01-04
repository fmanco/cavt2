CC = g++
CXXFLAGS = -O3 -Wall -I$(INC_BASE_DIR) `pkg-config --libs --cflags sndfile`
#CXXFLAGS = -g -O0 -Wall -I$(INC_BASE_DIR) `pkg-config --libs --cflags sndfile` #DEBUG

APP_BASE_DIR = app
INC_BASE_DIR = inc
LIB_BASE_DIR = lib

APPS =
APPS += TestCoder
APPS += wavCopy

HEADERS = $(shell find $(INC_BASE_DIR) -name '*.h' -type f)

LIBS =
LIBS += BitStream.o
LIBS += Golomb.o
LIBS += Predictor.o

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
