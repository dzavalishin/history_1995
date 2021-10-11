call version
set src=C
set files=*.c *.h *.mak makefile *.ic
cd %src%
echo ci -u %files%
echo rcs -nBeta_12: %files%
cd ..

