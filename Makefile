TARGET = fb_debug

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

$(TARGET): $(OBJECTS) $(HEADERS)
	g++ -g -Wall -std=c++11 $^ -o $@ -lflatbuffers

%.o: %.cpp
	g++ -g -Wall -std=c++11 -c $< -o $@

.PHONY: clean
clean:
	@-rm *.o
	@-rm $(TARGET)
