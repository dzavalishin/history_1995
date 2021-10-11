call version
set src=C
zip -j etc\maps\frip%ver%_debug.zip  %src%\frip.exe %src%\frip.map
lxlite %src%\frip.exe
zip -j frip%ver%.zip  4OS2\* 4ALL\* %src%\frip.exe
copy frip%ver%.zip freq\frip.zip

zip -j frip%ver%d.zip 4DOS\* 4ALL\* 4DOS32\* %src%\frip_dos.exe
copy frip%ver%d.zip freq\fripd.zip
zip -j etc\maps\frip%ver%d_debug.zip  %src%\frip_dos.exe %src%\frip_dos.map

zip -j frip%ver%n.zip 4NT\* 4ALL\* %src%\frip_nt.exe
copy frip%ver%n.zip freq\fripn.zip
zip -j etc\maps\frip%ver%n_debug.zip  %src%\frip_nt.map

rem zip -j frip%ver%s.zip 4DOS16\* %src%\frip_d16.exe
rem copy frip%ver%s.zip freq\frips.zip

cd %src%
zip -r ..\frip%ver%sr.zip * -x *.obj -x *.exe
cd ..
copy frip%ver%sr.zip d:\backups

