clean:
	rm -rf opari.rc *.mod.* *.opari.inc opari.tab.c

test: ../opari ftest ctest

ftest: ../opari
	@rm -f opari.rc
	./f90_test.sh
	./f77_test.sh
	diff out.oparirc.f opari.rc
	diff opari.tab.c opari.tab.f.out
	./table_test.sh f

ctest: ../opari
	@rm -f opari.rc
	./c_test.sh
	./table_test.sh c
