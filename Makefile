#CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++14 -O3 -Wno-format

#vpath %.cpp observables
#vpath %.hpp observables

SOURCES := $(wildcard *.cpp) $(wildcard observables/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: main.x

clean:
	$(RM) $(OBJECTS) $(DEPENDS) main.x

# Linking the executable from the object files
main.x: $(OBJECTS)
	$(CXX)  $(CXXFLAGS) $^ -o $@

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CXX)  $(CXXFLAGS) -MMD -MP -c $< -o $@

#%.x: %.o
#	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^
#
#clean:
#	$(RM) *.o *.x
