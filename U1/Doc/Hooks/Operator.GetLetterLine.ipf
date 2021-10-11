:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.Lets you get line from the user's letter text when processing
user command.

:dt.:hp6.Parameters:ehp6.
:dd.Name of the variable to put line to.

:dt.:hp6.Notes:ehp6.
:dd.If no line can be read, variable is not changed. If you
pass variable with no value (it is possible to remove value from
variable with DROP statement) it can be checked after calling
this operator to make sure you have read something.

:dt.:hp6.Sample:ehp6.
:dd.
:xmp.
/* Read a line into the variable 'input' */
/* and display it onscreen */
address "gremote" "GetLetterLine" "line"
say 'next line: 'line
:exmp.

:edl.

