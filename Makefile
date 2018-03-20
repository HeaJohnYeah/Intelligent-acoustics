all:
	@cd auto; make
	@cd msr; make
	@cd main; make; make install
	
clean:
	@cd auto; make clean
	@cd msr; make clean
	@cd main; make clean