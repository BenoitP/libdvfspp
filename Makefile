CXX=g++
CXXFLAGS=-O3 -g -Wall -Wextra -fPIC -flto
LDFLAGS=-O3 -g -flto -fPIC -shared

HDR=$(wildcard src/*.h)
SRC=$(wildcard src/*.cpp)
OBJ=$(addprefix obj/, $(patsubst %.cpp, %.o, $(SRC)))

all: libdvfs2.so

.PHONY: all clean distclean

libdvfs2.so: $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.cpp libdvfs2.so
	$(CXX) -O3 -g -Wall -Wextra $< -o $@ -ldvfs2 -L.

clean:
	rm -rf obj

distclean: clean
	rm -rf libdvfs2.so

obj/%.d: %.cpp
	mkdir -p $(@D)
	$(CXX) -MM $(CXXFLAGS) $< -MF $@

-include $(patsubst %.o, %.d, $(ALL_OBJ))

obj/%.o: %.cpp obj/%.d
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

