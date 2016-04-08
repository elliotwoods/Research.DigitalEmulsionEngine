CXX=g++
SOURCES=triggerTest.cpp
OBJECTS=$(SOURCES:.cpp=.o)
PROGRAM=triggerTest

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) 
	$(CXX) -g $(OBJECTS) -o $@ -framework m3api -framework CoreServices

.cpp.o: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) -g -F../bin/OSX -c $< -o $@ 

clean:
	rm -f $(PROGRAM) $(OBJECTS)
