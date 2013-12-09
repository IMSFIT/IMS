#makefile pre projekt z IMS
#autori: Meca, Maga
#loginy: xmagam00,xmecav00

CXX=g++

c= gcc

CXXFLAGS  = -Wall -std=c++98
CXXFLAGS += -g  # debugging info
CXXFLAGS += -O2 # optimization level

projekt: projekt_v4.cc
	$(CXX) $(CXXFLAGS) projekt_v4.cc -o projekt  -lsimlib -lm 
run: 
	./projekt
pack:
	tar cvzf 9_xmecav00_xmagam00.tar.gz projekt_v4.cc dokumentace.pdf okrsky.txt makefile
clean:
	rm -f projekt  *~ *.bak *.tar.gz
	
