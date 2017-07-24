win:
	make -f makefile.win.mak

nix:
	make -f makefile.nix.mak

clean:
	make -f makefile.win.mak clean
	make -f makefile.nix.mak clean
