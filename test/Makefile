CROSS_COMPILE =

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip
RANLIB = $(CROSS_COMPILE)ranlib

INC_PATH := -I../docxParse
LDFLAGS := -L../docxParse

CFLAGS := -Wall -g -DTIXML_USE_STL

ARFLAGS := -r
RANLIBFLAGS := 

TARGET := DocxTest
SRCC := $(wildcard *.c)
COBJ := $(SRCC:%.c=%.o)
SRCXX := $(wildcard *.cpp) 
CXXOBJ := $(SRCXX:%.cpp=%.o)
OBJ := $(COBJ) $(CXXOBJ)

.PHONY : clean all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -g -o $@ $(LDFLAGS) -lDocxParse -lpthread -lrt -ldl -lm

$(COBJ):%.o:%.c
	$(CC) $(INC_PATH) $(CFLAGS) -o $@ -c $<

$(CXXOBJ):%.o:%.cpp 
	$(CXX) $(INC_PATH) $(CFLAGS) -o $@ -c $<	
	
clean:
	rm -f $(TARGET).a $(TARGET).so *~ *.o 
	rm -f $(OBJ)
	rm -f $(TARGET)
