;
; Please read docs for complete description of this stuff.
;
; Format:
;
; Host[/Alias[,Alias...]]    mode [/option] [-option] groups
;
; Host: Either uucp host name, domain name or address, as
;       required by delivery scheme
;
; Alias: alternative name of host to check path for. If no one defined,
;        path is checked for host name only. If alias[es] defined host
;        name is ignored.
;
;        NB!
;	 You MUST define some (maybe empty) alias for viafilt mode,
;        otherwise path will be compared with the name of filter program.
;
; Mode: one of viauux, viamail, viafilt
;
;   viauux              - uux host!rnews <batch
;   viamail             - rmail host <batch
;   viarsh              - rsh host -b rnews <batch
;   viafilt             - host <batch 
;                       (Here program name must be given instead of host name)
;
; Options: 
;   -batch              - Batched only;
;   -comp12             - Batched & compressed, 12 bits;
;   -comp16             - Batched & compressed, 16 bits.
;   -codetab=tab_name   - Code conversion table name
;   -snews=via-host     - modifies viauux to do uux via-host!snews host <batch
;   -lines              - add "Lines:" headline. (Resource hog!)
;
; Groups: Space-separated list of newsgroups to feed. Wildcard ('*')
;         are allowed in rightmost position. (In any position, in fact :)
;

;
;	Examples
;

; For use with KIAE newsserver, UUPC/* 3.0-4.07
; Needs no -codetab parameter since UUPC/* sendmail does code
; conversion by itself.
;
; (this one is in action! comment out if you don't need it!)
;
newsserv@kiae.su viamail *

; Same, but UUPC/ext 1.11 or Waffle
; Assumed that this host represents itself as 'kiae' or 'relcom'
; in path headline
;newsserv@kiae.su/kiae,relcom viamail -codetab=CP866-KOI8 *

; ViaUux, batched - UUPC/ext or Waffle
;kiae viauux -batch -codetab=CP866-KOI8 *

; ViaUux, batched & compressed (use with caution, not fully tested!)
;kiae viauux -comp12 -codetab=CP866-KOI8 *

; If you have FTP PCTCP installed: feed via TCPIP to Unix machine 'daisy'
;daisy viarsh -batch -codetab=CP866-KOI8 *

; Via your own program called 'sendnews.exe', batched
; Note empty alias list included. Without it all articles that have
; 'sendnews' in path will not be passed to this filter.
;sendnews/    viafilt -batch               *


