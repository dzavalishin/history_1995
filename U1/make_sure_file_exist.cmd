@echo off
if not exist %1 goto error
exit 0
:error
echo %1 does not exist
exit 1
