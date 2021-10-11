:p.
When writing U1 REXX hooks you can use additional operators, described below.
Please don't forget that additional operators are defined in "U1" environment
only, and if you change environment with 'ADDRESS' operator all of them
will be unavailable. To switch environment back use 'ADDRESS "U1"' command.
See Rexx reference book, keyword instruction 'ADDRESS' for further 
information on environments.
:p.

:h2 res=51.Operator 'log'&colon. Writing debug info to U1 log file
.im Operator.log.ipf

:h2 res=52.Operator 'warning'&colon. Displaying warning message
.im Operator.warning.ipf

:h2 res=53.Operator 'error'&colon. Displaying error message
.im Operator.error.ipf

:h2 res=54.Operator 'fatal'&colon. Displaying fatal error message and halting
.im Operator.fatal.ipf
