###########################################################################
#
#       $Log: main.mak $
#       Revision 1.2  1996/03/09 01:36:22  dz
#       *** empty log message ***
#
#   Revision 1.1  1995/05/08  23:32:22  dz
#   Initial revision
#
#
#
#

!include dllver.mak
!include version.mak


#
#       Definitions
#
#

#
# Root of U1 tree
#
WE=d:\u1

SYS=OS2
M_OUT=OUT\OS2Dist.OUT
L_OUT=OUT\OS2Dist.LCL
STRIP=copy
ZIP=zip -j
MAKE=nmake /nologo "NODEBUG = $(NODEBUG)" 
HATCH=N:\Net\NEF\nef -cN:\Net\NEF\NEF.CFG match 
PGP=n:\net\ge\pgp\pgp32
LXLITE=lxlite /d-

#
# Where to put hot version and alpha revision
#
HOT_DIR=d:\U1\Out
HOT_FILE=HU1_$(SYS).zip
HOT_SIG_FILE=HU1_$(SYS).SIG

AR_DIR=d:\U1\Out
AR_FILE=AU1_$(SYS).zip
AR_DOX=AU1_INF.zip

BR_DIR=d:\U1\Out
BR_FILE=BU1_$(SYS).zip
BR_SIG_FILE=BU1_$(SYS).SIG



#
# Where to put internal stuff
#

U1_LIBS=$(WE)\LIB\OUT

#
# Other defs
#

DLLF=U1_$(DLLVER).DLL
DLLP=$(WE)\lib\dll\$(DLLF)
LIBS = Libs
LHDR = $(WE)\lib\dll\*.h $(WE)\*.h

###########################################################################
#
#       Goals
#
#

help:
        @echo make all/hot/local/doc/sig/alpha/hatch_alpha

all:	\
        $(M_OUT)\rnews.exe $(M_OUT)\unbatch.exe \
        $(M_OUT)\snews.exe $(M_OUT)\batch.exe \
        $(M_OUT)\setup.U1 $(M_OUT)\users.U1 $(M_OUT)\feeds.U1 \
        $(M_OUT)\echoes.U1 $(M_OUT)\read.me \
        $(M_OUT)\uu2fido.exe $(M_OUT)\fido2uu.exe \
	$(M_OUT)\gremote.exe \
	$(M_OUT)\U1.INF $(M_OUT)\U1_Hooks.INF
#        $(L_OUT)\file2uu.exe $(L_OUT)\getfatt.exe                       
#        $(L_OUT)\sendrule.exe
#        $(M_OUT)\U1.doc $(M_OUT)\setup.doc $(M_OUT)\worlds.doc         \
#        $(M_OUT)\uustat.exe                                             \

hot:    $(HOT_DIR)\$(HOT_FILE)   sig
    
sig:    $(HOT_DIR)\$(HOT_SIG_FILE)

alpha:  $(AR_DIR)\$(AR_FILE)

#beta:   clean $(BR_DIR)\$(BR_FILE)
beta:   $(BR_DIR)\$(BR_FILE) $(BR_DIR)\$(BR_SIG_FILE)

ahatch: hatch_alpha

hatcha: hatch_alpha

hatch_alpha: alpha anews
   copy $(AR_DIR)\$(AR_FILE) $(AR_DIR)\AU1_$(VER_).Zip
   $(HATCH) $(AR_DIR)\AU1_$(VER_).Zip UU2 "U1 $(VER) Alpha"

hatch_dox: dox
   $(HATCH) $(AR_DIR)\$(AR_DOX) UU2 "U1 $(VER) Documentation Alpha"


dox: doc

doc:    $(M_OUT)\U1.INF $(M_OUT)\U1_Hooks.INF  $(AR_DIR)\$(AR_DOX)

bhatch: hatch_beta

hatchb: hatch_beta

hatch_beta: bnews beta
   copy $(BR_DIR)\$(BR_FILE) $(BR_DIR)\BU1_$(VER_).Zip
   $(HATCH) $(AR_DIR)\BU1_$(VER_).Zip UU2 "U1 $(VER) Beta"
   copy $(BR_DIR)\$(BR_SIG_FILE) $(BR_DIR)\HU1_$(VER_).Sig
   $(HATCH) $(BR_DIR)\HU1_$(VER_).Sig UU2 "U1 $(VER) Beta PGP Signature"



clean:
	cd lib\dll
	$(MAKE) clean
	cd ..\..
	cd mail\fido2uu
	$(MAKE) clean
	cd ..\..
	cd mail\uu2fido
	$(MAKE) clean
	cd ..\..
	cd news\rnews
	$(MAKE) clean
	cd ..\..
	cd news\snews
	$(MAKE) clean
	cd ..\..
	cd news\batch
	$(MAKE) clean
	cd ..\..
	cd news\unbatch
	$(MAKE) clean
	cd ..\..


###########################################################################
#
#       Create libs
#
#



Libs: MakeLib.mak dllver.mak version.h lib\dll\*.c *.h
	$(MAKE) -f MakeLib.mak

$(DLLP): Libs
	make_sure_file_exist.cmd $(DLLP)

###########################################################################
#
#       Compile/tdstrip/pkilite programs
#
#


$(M_OUT)\fido2uu.exe: mail\fido2uu\*.c mail\fido2uu\*.h $(LIBS) $(LHDR)
        cd mail\fido2uu
        $(MAKE) out
        cd ..\..


$(M_OUT)\uu2fido.exe: mail\uu2fido\*.c mail\uu2fido\*.h $(LIBS) $(LHDR)
        cd mail\uu2fido
        $(MAKE) out
	cd ..\..


$(M_OUT)\gremote.exe:   tools\remote\*.c tools\remote\*.h $(LIBS) $(LHDR)
        cd tools\remote
        $(MAKE) out
        cd ..\..


$(M_OUT)\unbatch.exe:    news\unbatch\*.c news\unbatch\*.h $(LIBS) $(LHDR)
        cd news\unbatch
        $(MAKE) out
        cd ..\..

$(M_OUT)\rnews.exe:    news\rnews\*.c news\rnews\*.h $(LIBS) $(LHDR)
        cd news\rnews
        $(MAKE) out
        cd ..\..

$(M_OUT)\batch.exe:    news\batch\*.c news\batch\*.h $(LIBS) $(LHDR)
        cd news\batch
        $(MAKE) out
        cd ..\..

$(M_OUT)\snews.exe:    news\snews\*.c news\snews\*.h $(LIBS) $(LHDR)
        cd news\snews
        $(MAKE) out
        cd ..\..



$(L_OUT)\file2uu.exe:    file\file2uu\*.c file\file2uu\*.h $(LIBS) $(LHDR)
        cd file\file2uu
        $(MAKE) out
        cd ..\..


$(L_OUT)\getfatt.exe:    file\getfatt\*.c file\getfatt\*.h $(LIBS) $(LHDR)
        cd file\getfatt
        $(MAKE) out
        cd ..\..




###########################################################################
#
#       Copy example setup files, etc
#
#

$(M_OUT)\setup.U1:		doc\setup.U1
    copy doc\setup.U1 $(M_OUT)\setup.U1

$(M_OUT)\users.U1:     doc\users.U1
    copy doc\users.U1 $(M_OUT)\users.U1

$(M_OUT)\feeds.U1:     doc\feeds.U1
    copy doc\feeds.U1 $(M_OUT)\feeds.U1

$(M_OUT)\echoes.U1:    doc\echoes.U1
    copy doc\echoes.U1 $(M_OUT)\echoes.U1

$(M_OUT)\read.me:            doc\read.me
        copy doc\read.me $(M_OUT)\read.me

$(M_OUT)\U1_flow.pcx:          etc\U1_111w.pcx
	copy etc\U1_111w.pcx $(M_OUT)\U1_flow.pcx

$(M_OUT)\$(DLLF): $(DLLP)
        copy $(DLLP) $(M_OUT)\$(DLLF)
#        $(LXLITE) $(M_OUT)\$(DLLF)

###########################################################################
#
#       Create dox
#
#

$(M_OUT)\U1.inf:   Doc\ipf\*.ipf Doc\ipf\*.ipf
        cd Doc\ipf
        $(MAKE) out
        cd ..\..

$(M_OUT)\U1_Hooks.INF:   Doc\Hooks\*.ipf Doc\Hooks\*.ipf
        cd Doc\Hooks
        $(MAKE) out
        cd ..\..


###########################################################################
#
#       Pack & copy hot/alpha version to BBS
#
#

U1_LIST = \
$(M_OUT)\uu2fido.exe $(M_OUT)\fido2uu.exe history $(M_OUT)\read.me \
$(M_OUT)\setup.U1 $(M_OUT)\users.U1 $(M_OUT)\U1_flow.pcx $(M_OUT)\unbatch.exe \
$(M_OUT)\rnews.exe $(M_OUT)\echoes.U1 \
$(M_OUT)\feeds.U1 $(M_OUT)\batch.exe $(M_OUT)\snews.exe \
$(M_OUT)\$(DLLF) \
$(M_OUT)\gremote.exe 
# $(DLLP) \

# $M_OUT)\uustat.exe $(M_OUT)\U1.doc $(M_OUT)\worlds.doc $(M_OUT)\setup.doc

U1_DOX_LIST = $(M_OUT)\U1.INF $(M_OUT)\U1_Hooks.INF

MMTMP=$(TMP)\mainmak.tmp

#
#         Alpha version
#

$(AR_DIR)\$(AR_FILE): $(U1_LIST) $(U1_DOX_LIST)
        -del /q $(AR_DIR)\$(AR_FILE)
	echo U1 Version $(VER) - alpha release >$(MMTMP)
        $(ZIP) -z $(AR_DIR)\$(AR_FILE) $(U1_LIST) $(U1_DOX_LIST) hot.txt <$(MMTMP)
	-del $(MMTMP)


$(AR_DIR)\$(AR_DOX): $(U1_DOX_LIST)
        -del /q $(AR_DIR)\$(AR_DOX)
	echo U1 Version $(VER) - alpha release dox>$(MMTMP)
        $(ZIP) -z $(AR_DIR)\$(AR_DOX) $(U1_DOX_LIST) history hot.txt <$(MMTMP)
	-del $(MMTMP)


#
#         Beta version
#

$(BR_DIR)\$(BR_FILE): $(U1_LIST) $(U1_DOX_LIST)
        -del /q $(BR_DIR)\$(BR_FILE)
	echo U1 Version $(VER) - beta release >$(MMTMP)
        $(ZIP) -z $(BR_DIR)\$(BR_FILE) $(U1_LIST) $(U1_DOX_LIST) <$(MMTMP)
	-del $(MMTMP)

$(BR_DIR)\$(BR_SIG_FILE): $(BR_DIR)\$(BR_FILE)
        $(PGP) -sb $(BR_DIR)\$(BR_FILE) -o $(BR_DIR)\$(BR_SIG_FILE)


#
# unused
#

$(HOT_DIR)\$(HOT_FILE): $(U1_LIST)
        del /q    $(HOT_DIR)\$(HOT_FILE)
        del /q $(HOT_DIR)\$(HOT_SIG_FILE)
        $(ZIP) $(HOT_DIR)\$(HOT_FILE) $(U1_LIST)

$(HOT_DIR)\$(HOT_SIG_FILE):    $(HOT_DIR)\$(HOT_FILE)
        $(PGP) -sb $(HOT_DIR)\$(HOT_FILE) -o $(HOT_DIR)\$(HOT_SIG_FILE)






###########################################################################
#
#       Send to betatesters a message about new hot version
#
#

bnews: message.txt

message.txt: hot.txt
	copy etc\banner.uu2 + hot.txt + etc\howget.uu2 message.txt
	n:\net\tools\sendmsg $$n:\spool\sq\uu2 NewsRobot,2:5020/32 "-sU1 News" -m <message.txt
	copy /b hot.txt + history history.new
	del history
	ren history.new history
	copy etc\hot.tpl hot.txt

anews: amessage.txt

amessage.txt: hot.txt
	copy etc\banner.uu2 + hot.txt + etc\howget.uu2 amessage.txt
	n:\net\tools\sendmsg $$n:\spool\sq\uu2 NewsRobot,2:5020/32 "-sU1 Alpha News" -m <amessage.txt






