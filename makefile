CC = g++
CPPFLAGS = -g -I. -std=c++11
router: router.o
router.o: router.cpp
clean:
	rm router
	rm *.o
