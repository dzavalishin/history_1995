:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.Can be used to write debugging information to U1 log file.

:dt.:hp6.Parameters:ehp6.
:dd.Text to be logged.

:dt.:hp6.Notes:ehp6.
:dd.U1 setup parameter
LogLevel must have 'x' character in it's value, or debug mode must be turned
on in oprder for log operator to be effective.

:dt.:hp6.Sample:ehp6.
:dd.
:xmp.
/* Log value of variable 'input' */
log 'Rexx Hook is running. Parameter is: 'input
:exmp.

:edl.

