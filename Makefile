export CC = clang++
export CFLAGS = -O4 -Wall -Wextra -std=c++98 -pedantic -Woverloaded-virtual
export LDFLAGS = -O4 -Xlinker -plugin=LLVMgold.so

#export CC = g++
#export CFLAGS= -flto -Wall -Wextra -O3 -p -pg -std=c++98 -pedantic -Woverloaded-virtual
#export LDFLAGS= -flto

all: src
	$(MAKE) $(MAKEFLAGS) -C src

clean:
	$(MAKE) $(MAKEFLAGS) -C src clean

mrpropre: clean
	rm -rf *~ doxygen

rebuild: clean all

doc: doxygen.cfg
	doxygen doxygen.cfg
