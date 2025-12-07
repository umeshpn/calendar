
#CC=/usr/cygnus/mentor-98r2p39/H-sparc-sun-solaris2.7/bin/c++ 
CC=/usr/bin/c++ -g
MAL=/Users/umesh/uscripts/mal.py
POST_MAL=/Users/umesh/uscripts/post_mal

%.o:../src/%.c 
	$(CC) -c -I../src $<

%.o:../src/%.cxx 
	$(CC) -c -I../src $<


all: sta crpan lsta # hpanjang span search_star syn lsta
sta: ../src/ast.h sta.o ast_data.o ast_lib.o hs.o draw_ch.o gen_lib.o title.o sani.o
	$(CC) -o sta ast_data.o ast_lib.o draw_ch.o gen_lib.o hs.o sta.o title.o sani.o -lm

lsta: ../src/ast.h lsta.o ast_data.o ast_lib.o hs.o ldraw_ch.o draw_ch.o gen_lib.o lprint_lib.o kundali.o yogam.o lend_values.o
	$(CC) -o lsta ast_data.o ast_lib.o ldraw_ch.o draw_ch.o gen_lib.o hs.o lsta.o lprint_lib.o kundali.o yogam.o lend_values.o -lm

just_gn: ../src/ast.h just_gn.o ast_data.o ast_lib.o hs.o ldraw_ch.o draw_ch.o gen_lib.o lprint_lib.o kundali.o yogam.o lend_values.o
	$(CC) -o just_gn ast_data.o ast_lib.o ldraw_ch.o draw_ch.o gen_lib.o hs.o just_gn.o lprint_lib.o kundali.o yogam.o lend_values.o -lm

intsta: ../src/ast.h intsta.o ast_data.o ast_lib.o hs.o draw_ch.o gen_lib.o title.o 
	$(CC) -o intsta ast_data.o ast_lib.o draw_ch.o gen_lib.o hs.o intsta.o title.o sani.o -lm

perigee_test: ../src/ast.h perigee_test.o ast_data.o ast_lib.o gen_lib.o draw_ch.o sani.o
	$(CC) -o perigee_test perigee_test.o draw_ch.o ast_data.o ast_lib.o gen_lib.o sani.o -lm

panjang: ../src/ast.h panjang.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o
	$(CC) -o panjang panjang.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o  -lm

# lpanjang: ../src/ast.h lpanjang.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o lend_values.o
# 	$(CC) -o lpanjang lpanjang.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o  lend_values.o -lm

crpan: ../src/ast.h crpan.o ast_lib.o ast_data.o gen_lib.o lend_values.o hs.o draw_ch.o
	$(CC) -o crpan crpan.o ast_lib.o ast_data.o gen_lib.o  lend_values.o hs.o  draw_ch.o -lm

crcal: ../src/ast.h crpanchart.o ast_lib.o ast_data.o gen_lib.o lend_values_cal.o hs.o draw_ch.o
	$(CC) -o crcal crpanchart.o ast_lib.o ast_data.o gen_lib.o  lend_values_cal.o hs.o  draw_ch.o -lm

hpanjang: ../src/ast.h hourly_panjang.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o
	$(CC) -o hpanjang hourly_panjang.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o -lm

span: ../src/ast.h span.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o
	$(CC) -o span span.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o -lm

search_star: ../src/ast.h search_star.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o
	$(CC) -o search_star search_star.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o -lm

find_comb: ../src/ast.h find_comb.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o
	$(CC) -o find_comb find_comb.o ast_lib.o ast_data.o gen_lib.o draw_ch.o sani.o -lm

syn : ../src/ast.h ast_data.o gen_lib.o ast_lib.o hs.o draw_ch.o title.o syn.o sani.o
	$(CC) -o syn ast_data.o gen_lib.o ast_lib.o hs.o draw_ch.o title.o syn.o sani.o -lm


print_muh : ../src/ast.h print_muh.o ast_data.o ast_lib.o hs.o ldraw_ch.o draw_ch.o gen_lib.o lprint_lib.o kundali.o yogam.o lend_values.o
	$(CC) -o print_muh print_muh.o ast_data.o ast_lib.o ldraw_ch.o draw_ch.o gen_lib.o hs.o lprint_lib.o kundali.o yogam.o lend_values.o -lm


install: sta intsta panjang hpanjang span search_star syn
	cp sta intsta panjang hpanjang span search_star syn ../bin

ptest.ps : ptest.dvi
	dvips ptest

ptest.dvi : ptest.tex panjang_macros.tex panjang_mukh.tex
	latex ptest

ptest.tex : ptest.mm
	$(MAL) ptest.mm

ptest.mm : ../src/ptest.mtx
	cp ../src/ptest.mtx ptest.mm

p.pdf : p.tex panjang_macros.tex panjang_mukh.tex p1.tex p2.tex p3.tex p4.tex p.bbl
	pdflatex -shell-escape p.tex
	pdflatex -shell-escape p.tex

# p.ps : p.dvi 
# 	dvips -Ppdf p

# p.dvi : panjang_macros.tex panjang_mukh.tex p.tex p1.tex p2.tex p3.tex p4.tex p.bbl
# 	latex p

p.tex : p.mm
	$(MAL) p.mm rachana
	$(POST_MAL) p.tex

p.mm : ../src/p.mtx
	cp ../src/p.mtx p.mm

panj_theory.pdf : panj_theory.ps
	/usr/bin/ps2pdf panj_theory.ps

panj_theory.ps : panj_theory.dvi
	dvips panj_theory

panj_theory.dvi : panjang_macros.tex panj_theory.tex panjang_det.tex rasi_nak.tex thithis.tex nityayogams.tex p.bbl
	latex panj_theory

panj_theory.tex : panj_theory.mm
	$(MAL) panj_theory.mm rachana

panj_theory.mm : ../src/panj_theory.mtx
	cp ../src/panj_theory.mtx panj_theory.mm


p1.tex : p1.mm
	$(MAL) p1.mm rachana

p2.tex : p2.mm
	$(MAL) p2.mm rachana

p3.tex : p3.mm
	$(MAL) p3.mm rachana

p4.tex : p4.mm
	$(MAL) p4.mm rachana

p5.tex : p5.mm
	$(MAL) p5.mm rachana

p.bbl : p.bib
	pdflatex -shell-escape p
	bibtex p
	pdflatex -shell-escape p
	bibtex p
	pdflatex -shell-escape p

p.bib : pbib.tex
	cp pbib.tex p.bib

pbib.tex : pbib.mm
	$(MAL) pbib.mm rachana

panjang_macros.tex : panjang_macros.mm
	$(MAL) panjang_macros.mm rachana

panjang_macros.mm : ../src/panjang_macros.mtx
	cp ../src/panjang_macros.mtx panjang_macros.mm

panjang_mukh.tex : panjang_mukh.mm
	$(MAL) panjang_mukh.mm rachana

panjang_mukh.mm : ../src/panjang_mukh.mtx
	cp ../src/panjang_mukh.mtx panjang_mukh.mm

panjang_det.tex : panjang_det.mm
	$(MAL) panjang_det.mm rachana

panjang_det.mm : ../src/panjang_det.mtx
	cp ../src/panjang_det.mtx panjang_det.mm

rasi_nak.tex : rasi_nak.mm
	$(MAL) rasi_nak.mm rachana

rasi_nak.mm : ../src/rasi_nak.mtx
	cp ../src/rasi_nak.mtx rasi_nak.mm

thithis.tex : thithis.mm
	$(MAL) thithis.mm rachana

thithis.mm : ../src/thithis.mtx
	cp ../src/thithis.mtx thithis.mm

nityayogams.tex : nityayogams.mm
	$(MAL) nityayogams.mm rachana

nityayogams.mm : ../src/nityayogams.mtx
	cp ../src/nityayogams.mtx nityayogams.mm

pbib.mm : ../src/pbib.mtx
	cp ../src/pbib.mtx pbib.mm



clean: cclean lclean

cclean:
	rm -f *.o sta intsta panjang span perigee_test search_star lpanjang syn perigee_test hpanjang core

lclean:
	rm -f *.tex *.dvi *.ps *.pdf *.aux *.log 
