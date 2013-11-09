# vychozi obrazky
SRC= tabulka.png logo.jpg lex_an.jpg image10.png image11.png image12.png image5.png image6.png image7.png image8.png image9.png image4.png image3.png image2.png image1.png priklad1.png priklad2.png priklad3.png priklad4.png priklad5.png priklad6.png logaritmus.png logo.jpg lomchyba.png lomenac.jpg logax.png log_rada.png tanh.png tanh_vzorec.png tay_cosh.png tay_sinh.png
# pozadovany format pro LaTeX
DEST=arcsin.eps logax.eps log_rada.eps tanh.eps tanh_vzorec.eps tay_cosh.eps tay_sinh.eps

.SUFFIXES: .fig .eps .png .jpg

all: $(DEST)

.fig.eps:
	fig2dev -L eps $< $@

.png.eps:
	convert $< $@

.jpg.eps:
	convert $< $@
