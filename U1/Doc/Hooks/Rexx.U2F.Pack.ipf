:h2 
    x=left y=top width=25% height=100%
    scroll=none group=1 clear
    res=82.Rexx.U2F.Pack&colon. UU2Fido UUCP File Compression Hook
:link reftype=hd res=821
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=2 auto dependent.
:p.
:link reftype=hd res=821
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=2 dependent.Description:elink.
:p.
:link reftype=hd res=822
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=3 dependent.Example:elink.


:h3 res=821 x=25% y=top width=75% height=100%.Description of Rexx.U2F.Pack

:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.This hook can be used to compress files, attached to the FIDO
letters by UU2Fido, with the archiving program you like.

:dt.:hp6.Call reason:ehp6.
:dd.This hook is called when letter passed to UU2Fido is large enough
to be sent as an attached file instead of simple FIDO message.

:dt.:hp6.Parameters:ehp6.
:dd.The first parameter is full name of the file to be compressed,
second one points to the directory where compressed file must be
created.

:dt.:hp6.Return value:ehp6.
:dd.This hook must return name of resulting compressed file.
If compression failed, return name of the original file, and, please,
leave the file itself intact. If compression is succeded, it is
your responsibility to delete original file.

:edl.

:h3 res=822 x=25% y=top width=75% height=100%.
Sample script for Rexx.U2F.Pack
:xmp.
/*****************************************************/
/* attachment packing hook for U1 UU2Fido.exe        */
/*                                                   */
/*                                                   */
/* Input:  unpacked file name (full)                 */
/*         directory to put packed file to           */
/*                                                   */
/* Output: packed file name (full)                   */
/*                                                   */
/*****************************************************/

/* get parameters */
Parse Arg unp_name, out_dir

/* make sure execution environment is U1's */
address "U1"

/* log parameters */
log "Rexx packer ("unp_name", "out_dir")"

/* initialize output to default */
OutString = unp_name

/* extract filename.extension part form the full name */
p=lastpos('\',unp_name)
if p = 0 then unp_file_ext=unp_name
else unp_file_ext=substr(unp_name,p+1)

p=lastpos('/',unp_file_ext)
if \ (p = 0) then unp_file_ext=substr(unp_file_ext,p+1)

debug "unp_file_ext: "unp_file_ext

/* cut extension off */
p=lastpos('.',unp_file_ext)
if p = 0 then unp_file=unp_file_ext
else unp_file=substr(unp_file_ext,1,p-1)

debug "unp_file: "unp_file

/* build name of compressed file */
p_name=out_dir||'\'||unp_file||'.zip'

debug "p_name: "p_name

say "Compressing with zip "unp_name

/* execute command */
Address "CMD" 'zip -oj '||p_name||' '||unp_name
if rc = 0 then
        do
        /* Success. Set return value to compressed file name... */
        OutString=p_name
        /* ...and delete uncompressed file */
        call SysDeleteFile unp_name
        end
else
        Error "Unable to zip file, exit code = "||rc

/* return string, removing any blanks on the front or back. */
Return Strip(OutString)                


:exmp.
