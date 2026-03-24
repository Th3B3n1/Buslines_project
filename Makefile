#
# Makefile a CppEmber feladathoz
#

# MEMTRACE-hez:
CXXFLAGS = -g -Wall -Werror -pedantic -DMEMTRACE

# Mpatrol-hoz:
#CXXFLAGS = -g -Wall -Werror -pedantic
LIBMPATROL = -lmpatrol -lbfd

SRCS = $(filter-out memtrace.cpp, $(wildcard *.cpp))
OBJS = $(SRCS:.cpp=.o)

# Ez a default cel
.PHONY: all
all: program_mtrac

# Nincs mem ellenorzes
program: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# MPATROL valtozat
program_mpat: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBMPATROL) -o $@

# MEMTRACE
program_mtrac: $(OBJS) memtrace.o
	$(CXX) $(CXXFLAGS) $(OBJS) memtrace.o -o $@

clean:
	rm -f *.o *.log program program_mpat program_mtrac

check:  program_mpat
	MPATROL_OPTIONS=LEAKTABLE ./program_mpat
	
