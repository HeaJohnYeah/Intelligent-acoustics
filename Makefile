all:
	@cd auto; make
	@cd main; make; make install
	
clean:
	@cd auto; make clean
	@cd main; make clean