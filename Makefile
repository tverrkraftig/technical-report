.PHONY: all
all:
	pdflatex report > /dev/null
	biber report > /dev/null
	pdflatex report > /dev/null
	pdflatex report > /dev/null
	open report.pdf

.PHONY: clean
clean:
	rm -f *.aux *.toc *.log *.bbl *.blg

.PHONY: err
err:
	pdflatex report
	biber report
	
