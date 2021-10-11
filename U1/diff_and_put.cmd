set tf=%temp%\diff_%_pid
rcsdiff %1 > %tf
set code=%?
echo exit: %code
if %code == 0 goto equal
if %code == 1 goto not_equal

echo error comparing files, exiting
del %tf

:equal
echo files are equal
del %tf
exit 0

:not_equal
echo differrences found
pause
start /c "Differrence between %1 and latest RSC archive of it" list %tf
pause
ci -u %1
del %tf
