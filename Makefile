#CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS	:= -std=c++14 -O3 -Wno-format
# Add more warnings
# CXXFLAGS	+= -Wall -Wextra


#vpath %.cpp observables
#vpath %.hpp observables

MAIN	:= cdt2d.x
SOURCES := $(wildcard *.cpp) $(wildcard observables/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))


# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: $(MAIN)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(MAIN)

# Linking the executable from the object files
$(MAIN): $(OBJECTS)
	echo $(OBJECTS)
	$(CXX)  $(CXXFLAGS) $^ -o $@

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CXX)  $(CXXFLAGS) -MMD -MP -c $< -o $@

#%.x: %.o
#	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^
#
#clean:
#	$(RM) *.o *.x
