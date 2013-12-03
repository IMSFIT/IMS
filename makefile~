#makefile pre projekt z IMS
#autori: Meca, Maga
#loginy: xmagam00,xmecav00

CXX=g++

c= gcc

CXXFLAGS  = -Wall -std=c++98
CXXFLAGS += -g  # debugging info
CXXFLAGS += -O2 # optimization level

gen_poctu: gen_poctu.c
	$(c) -std=c99 -Wall -g -O2 gen_poctu.c -o gen_poctu
	
gen_okrsku: gen_okresu.c
	$(c) -std=c99 -Wall -g -O2 gen_okresu.c -o gen_okresu
	
projekt: projekt_v4.cc
	$(CXX) $(CXXFLAGS) projekt_v4.cc -o projekt  -lsimlib -lm 
	
pokus: pokus.cc
	$(CXX) $(CXXFLAGS) pokus.cc -o projekt  -lsimlib -lm 
	
volba_volice: volba_volice.cc
	$(CXX) $(CXXFLAGS) volba_volice.cc -o volba_volice  -lsimlib -lm 
pocitani_volebni_komise: pocitani_volebni_komise.cc
	$(CXX) $(CXXFLAGS) pocitani_volebni_komise.cc -o pocitani_volebni_komise  -lsimlib -lm 
model: model.cc
	$(CXX) $(CXXFLAGS) model.cc -o model  -lsimlib -lm 
pack:
	tar cvzf xmagam00_xmecav00.tar.gz model.cc  makefile
clean:
	rm -f model gen_okresu projekt gen_poctu pocitani_volebni_komise volba_volice *~ *.bak *.tar.gz
	
