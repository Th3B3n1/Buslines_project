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

ifeq ($(OS),Windows_NT)
CLEAN_CMD = del /Q *.o *.log program program_mpat program_mtrac program.exe program_mpat.exe program_mtrac.exe 2>NUL
else
CLEAN_CMD = rm -f *.o *.log program program_mpat program_mtrac program.exe program_mpat.exe program_mtrac.exe
endif

# Ez a default cel
.PHONY: all
all: program

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
	$(CLEAN_CMD)

check:  program_mpat
	MPATROL_OPTIONS=LEAKTABLE ./program_mpat
	
