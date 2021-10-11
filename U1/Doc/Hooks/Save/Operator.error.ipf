:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.Can be used to tell user about non-fatal error.

:dt.:hp6.Parameters:ehp6.
:dd.Text to be displayed and logged.

:dt.:hp6.Notes:ehp6.
:dd.Text will be prepended with 'Error: ' before displaying it.

:dt.:hp6.Sample:ehp6.
:dd.
:xmp.
/* Issue an error message */
error 'No username in address 'in_address'. Forwarding letter to postmaster.'
:exmp.

:edl.
