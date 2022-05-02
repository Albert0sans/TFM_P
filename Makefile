

CXX = g++
CXXFLAGS = -Wall -DNDEBUG -g3 -O2 -Wall -Wextra 

LIBS =  


all: test  

main: test.cpp 
	$(CXX) $(CXXFLAGS) -o test test.cpp  $(LIBS)







clean:
	$(RM)  test  U
