:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.Can be used to tell user about fatal error.

:dt.:hp6.Parameters:ehp6.
:dd.Text to be displayed and logged.

:dt.:hp6.Notes:ehp6.
:dd.Text will be prepended with 'Fatal: ' before displaying it.
:caution.
Executing operator 'fatal' you'll stop currently running gate module
non-gracefully, possibly loosing some data (letters or articles).
:ecaution.


:dt.:hp6.Sample:ehp6.
:dd.
:xmp.
/* Issue an error message and terminate gate */
fatal 'Out of disk space!'
:exmp.

:edl.
