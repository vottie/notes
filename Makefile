CXX=g++
CXXFLAGS = -O0 -Wall -g -std=c++11 -fthreadsafe-statics -FPIC
LDFLAGS =
TARGET=Notes

OPT=
INC=
LIB  = -lfcgi
LIB += -lboost_system
LIB += -lboost_filesystem
LIB += -lboost_regex
LIB += -lboost_date_time


SRC = Notes.cpp Parser.cpp Config.cpp
HED = Notes.h Parser.h Config.h
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIB)

clean:
	$(RM) $(TARGET) *.o

