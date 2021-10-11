set over=43
set ver=44
set VERNAME=Version_%ver%
SET VF=c\version.h
del %VF%
echo /* do not edit! See %0 */ >%VF%
echo #define FRIP_VER_STR ^"Version %ver%^" >>%VF%
echo #define FRIP_VER_NUM %ver% >>%VF%

