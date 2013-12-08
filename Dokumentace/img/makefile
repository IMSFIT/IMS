# vychozi obrazky
SRC= konceptualni_model.jpg Petri.png petri2.png 0_5.png 1_75.png 100_second_part.png 100.png
# pozadovany format pro LaTeX
DEST=arcsin.eps logax.eps log_rada.eps tanh.eps tanh_vzorec.eps tay_cosh.eps tay_sinh.eps konceptualni_model.eps Petri.eps petri2.eps 0_5.eps 1_75.eps 100_second_part.eps 100.eps

.SUFFIXES: .fig .eps .png .jpg

all: $(DEST)

.fig.eps:
	fig2dev -L eps $< $@

.png.eps:
	convert $< $@

.jpg.eps:
	convert $< $@
