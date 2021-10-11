@Echo Making version called '%VERNAME%'
set VD="VERDEF = %VERNAME%"

make clean
make "NODEBUG = yes" %VD%

make /f Make_DOS.Mak clean
make /f Make_DOS.Mak "NODEBUG = yes" %VD%

REM make /f Make_DOS16.Mak clean
rem make /f Make_DOS16.Mak "NODEBUG = yes" %VD%

make /f Make_NT.Mak clean
make /f Make_NT.Mak "NODEBUG = yes" %VD%

