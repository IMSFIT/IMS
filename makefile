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
	tar cvzf xmagam00_xmecav00.tar.gz model.cc  makefile
clean:
	rm -f model gen_okresu projekt gen_poctu pocitani_volebni_komise volba_volice *~ *.bak *.tar.gz
	
