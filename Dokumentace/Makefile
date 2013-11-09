

dokumentace: dokumentace.tex
	latex dokumentace.tex
	bibtex dokumentace
	latex dokumentace.tex
	latex dokumentace.tex
	dvips dokumentace.dvi
	ps2pdf -sPAPERSIZE=a4 dokumentace.ps
clean:
	rm -f *.dvi *.ps *.aux *.pdf  *.toc *.log *.bbl *.blg *.out *~
pack:
	tar cvxf xmagam00_xmecav00.tar.gz dokumentace.pdf
