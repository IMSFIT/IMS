#makefile pre projekt z IMS
#autori: Meca, Maga
#loginy: xmagam00,xmecav00

CXX=g++

CXXFLAGS  = -Wall -std=c++98
CXXFLAGS += -g  # debugging info
CXXFLAGS += -O2 # optimization level


model: model.cc
	$(CXX) $(CXXFLAGS) model.cc -o model  -lsimlib -lm 
pack:
	tar cvzf xmagam00_xmecav00.tar.gz model.cc  makefile
clean:
	rm -f model *~ *.bak *.tar.gz
	