@echo off
copy ..\frip%ver%.zip
set desc=Frip Beta %ver%
set HATCH_DESC=%desc%, OS/2 version
set ECHO=AFTNFRIP
call hatch frip%ver%.zip/frip%over%.zip %ECHO%

copy ..\frip%ver%d.zip
set HATCH_DESC=%desc%, DOS/32 version
call hatch frip%ver%d.zip/frip%over%d.zip %ECHO%

rem copy ..\frip%ver%s.zip
rem set HATCH_DESC=%desc%, DOS/16 version - NOT RECOMMENDED!
rem if exist frip%ver%s.zip call hatch frip%ver%s.zip/frip%over%s.zip %ECHO%

copy ..\frip%ver%n.zip
set HATCH_DESC=%desc%, NT/Win95 version
call hatch frip%ver%n.zip/frip%over%n.zip %ECHO%


frip send ..\frip%ver%n.zip 388
frip send ..\frip%ver%d.zip 518
frip send ..\frip%ver%.zip 518
