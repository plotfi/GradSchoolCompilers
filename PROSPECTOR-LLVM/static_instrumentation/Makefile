LEVEL = ../../..
LIBRARYNAME = LLVMProspector
BUILD_ARCHIVE = 1

include $(LEVEL)/Makefile.common

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q . /usr/include/*.h /usr/include/c++/ ~/llvm-2.8

super:
	make -j3
	make -j3 -C dynamic_profiling

superclean:
	make clean
	rm -f tags cscope* *.csv *.bc *.exe a.out *.ll *.s *.mdata
	make -C dynamic_profiling clean

