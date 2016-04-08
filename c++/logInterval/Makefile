CXX=g++
SOURCES=logInterval.cpp
OBJECTS=$(SOURCES:.cpp=.o)
PROGRAM=logInterval

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) 
	$(CXX) -g $(OBJECTS) -o $@ -framework m3api -framework CoreServices

.cpp.o: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) -g -F../bin/OSX -c $< -o $@ 

clean:
	rm -f $(PROGRAM) $(OBJECTS)
