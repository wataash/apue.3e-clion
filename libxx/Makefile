ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

OBJS = z.o

all: libxx.a

libxx.a: $(OBJS)
	ar rv libxx.a $(OBJS)

# 2.5 Letting make Deduce the Recipes
# 10.2 Catalogue of Built-In Rules
# z.o: z.cpp
# 	$(CXX) $(CXXFLAGS) -c z.cpp

.PHONY: clean
clean:
	rm -f libxx.a $(OBJS)

include $(ROOT)/Make.libapue.inc
