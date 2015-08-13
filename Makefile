
full: translate addshebang

translate: clean
	h2tp src/ php/

addshebang:
	rm _tmp -R -f
	mkdir _tmp
	cat php/teachme-chatd.php >> _tmp/teachme-chatd.php
	rm php/teachme-chatd.php
	(echo "#/usr/bin/env php"; cat _tmp/teachme-chatd.php)\
	  >> php/teachme-chatd.php
	rm _tmp -R -f

clean:
	rm -R -f php/

confdoc:
	pdflatex -output-directory=doc doc/conf.tex
	pdflatex -output-directory=doc doc/conf.tex #double for toc
	rm doc/conf.aux doc/conf.log doc/conf.out doc/conf.toc -f

protocoldoc:
	pdflatex -output-directory=doc doc/protocol.tex
	pdflatex -output-directory=doc doc/protocol.tex
	rm doc/protocol.aux doc/protocol.log \
	doc/protocol.out doc/protocol.toc -f

doc: confdoc protocoldoc

cleandoc:
	rm doc/*.aux doc/*.log doc/*.out doc/*.pdf doc/*.toc -f
