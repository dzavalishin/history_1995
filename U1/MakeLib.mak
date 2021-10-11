!include dllver.mak

OUT=Lib\Out\U1_$(DLLVER).LIB
SRCD=Lib\DLL

$(OUT): Makelib.mak Version.h $(SRCD)\*.c $(SRCD)\*.h *.h
	cd $(SRCD)
#	nmake clean
	nmake install

Makelib.mak: dllver.mak

dllver.mak: version.h
	@echo nmake dllver.mak, please
        @exit 1

