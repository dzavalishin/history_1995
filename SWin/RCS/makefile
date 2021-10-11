head	1.22;
access;
symbols;
locks;
comment	@# @;


1.22
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.21;

1.21
date	96.11.22.14.58.48;	author dz;	state Exp;
branches;
next	1.20;

1.20
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.19;

1.19
date	96.11.15.16.29.28;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	96.10.11.17.02.37;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.10.03.07.43.57;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.09.30.10.35.02;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.29.05.54.19;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.09.26.22.07.12;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.21.13.26.57;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.21.08.07.06;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.19.00.29.58;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.19.00.28.13;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.09.14.53.22;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.09.01.47.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.22
log
@transparency, clock
@
text
@
all: test.exe mkmf

# mkmf

mkmf:
  mkmf -s

tree: tree.txt

tree.txt: *.c *.h
  ctb -otree.txt *.c

LD_PROFILE=/B"/DE /NOE" cppopa3.obj
OP_PROFILE=-Gh

#OP_BROWSE=-Fb*
#LD_BROWSE=/B"/BROWSE"

OP_DEBUG=-Ti
#OP_OPTIMIZE=-O -Oi100
#vmt placement 
#OP_VMT=-Wvft+

OP_MTHREAD=-Gm

CFLAGS= -Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG) $(OP_BROWSE) $(OP_VMT) $(OP_MTHREAD)

OBJS= controls.obj ctl_entry.obj ctl_list.obj device.obj device_os2.obj device_tty.obj event.obj focus.obj map.obj strng.obj sw_clock.obj sw_roundwin.obj sw_sema.obj sw_sysdep.obj tail.obj test.obj types.obj win.obj


test.exe: $(OBJS)
   icc /B"/nologo" -Fetest.exe $(OP_DEBUG) $(OBJS) $(LD_PROFILE) $(LD_BROWSE)

clean:
	-rm $(OBJS)
        -mv *.?~ bak
        -rm *.pdb

zip:
    zip -r SWin * -x *.obj -x *.exe



###
controls.obj: sw_controls.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_ctl_list.h sw_list_elem.h sw_device.h
ctl_entry.obj: sw_ctl_entry.h sw_controls.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
ctl_list.obj: sw_ctl_list.h sw_controls.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_list_elem.h
device.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
device_os2.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
device_tty.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
event.obj: sw_event_handler.h sw_focus.h sw_event.h sw_types.h sw_except.h strng.h
focus.obj: sw_focus.h sw_event.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h
map.obj: sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
strng.obj: strng.h sw_except.h strng.h
sw_clock.obj: sw_clock.h sw_roundwin.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_sysdep.h sw_device.h
sw_roundwin.obj: sw_roundwin.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
sw_sema.obj: sw_sema.h sw_except.h strng.h
sw_sysdep.obj: sw_sysdep.h
tail.obj: sw_tail.h sw_ctl_list.h sw_controls.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_list_elem.h sw_sema.h sw_device.h
test.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_controls.h sw_ctl_list.h sw_list_elem.h sw_ctl_entry.h sw_tail.h sw_sema.h sw_clock.h sw_roundwin.h sw_sysdep.h
types.obj: sw_types.h sw_except.h strng.h
win.obj: sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_device.h
@


1.21
log
@*** empty log message ***
@
text
@d2 3
a4 1
all: mkmf test.exe
d29 1
a29 1
OBJS= controls.obj ctl_entry.obj ctl_list.obj device.obj device_os2.obj device_tty.obj event.obj focus.obj map.obj strng.obj sw_roundwin.obj sw_sema.obj sw_sysdep.obj tail.obj test.obj types.obj win.obj
d56 1
d61 1
a61 1
test.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_controls.h sw_ctl_list.h sw_list_elem.h sw_ctl_entry.h sw_tail.h sw_sema.h
@


1.20
log
@Before changing event class
@
text
@d27 1
a27 1
OBJS= controls.obj ctl_entry.obj ctl_list.obj device.obj device_os2.obj device_tty.obj event.obj focus.obj map.obj strng.obj sw_sema.obj sw_sysdep.obj tail.obj test.obj types.obj win.obj
d54 1
@


1.19
log
@mouse event generator written
@
text
@@


1.18
log
@context menus done, menus code fixed to
support lone vertical menus correctly.
@
text
@d50 2
a51 2
event.obj: sw_event_handler.h sw_focus.h sw_event.h
focus.obj: sw_focus.h sw_event.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h
@


1.17
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d44 1
a44 1
controls.obj: sw_controls.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_device.h
@


1.16
log
@buttons
@
text
@d27 1
a27 1
OBJS= controls.obj ctl_entry.obj ctl_list.obj device.obj event.obj focus.obj map.obj strng.obj sw_sema.obj tail.obj test.obj types.obj win.obj
d45 2
a46 2
ctl_entry.obj: sw_ctl_entry.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
ctl_list.obj: sw_ctl_list.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_list_elem.h
d48 2
d55 2
a56 1
tail.obj: sw_tail.h sw_ctl_list.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_list_elem.h sw_sema.h sw_device.h
@


1.15
log
@bugfixes - a lot of
@
text
@d44 1
a44 1
controls.obj: sw_controls.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
@


1.14
log
@dialogs, basically written
@
text
@d31 1
a31 1
   icc -Fetest.exe $(OP_DEBUG) $(OBJS) $(LD_PROFILE) $(LD_BROWSE)
@


1.13
log
@log control written
@
text
@d2 1
a2 1
all: mkmf test.exe tree
d56 1
a56 1
win.obj: sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
@


1.12
log
@*** empty log message ***
@
text
@d25 1
a25 4
CFLAGS= \
-Tdp -Q \
$(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG) \
$(OP_BROWSE) $(OP_VMT) $(OP_MTHREAD)
d27 1
a27 1
OBJS= controls.obj ctl_entry.obj ctl_list.obj device.obj event.obj focus.obj map.obj strng.obj test.obj types.obj win.obj
d52 3
a54 1
test.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_controls.h sw_ctl_list.h sw_list_elem.h sw_ctl_entry.h
@


1.11
log
@external array for list
@
text
@d23 6
a28 1
CFLAGS= -Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG) $(OP_BROWSE) $(OP_VMT)
@


1.10
log
@faster map rebuild. not much, though.
@
text
@d44 1
a44 1
ctl_list.obj: sw_ctl_list.h sw_win.h sw_types.h sw_except.h strng.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h
d50 1
a50 1
test.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_controls.h sw_ctl_list.h sw_ctl_entry.h
@


1.9
log
@menus basically done
@
text
@d12 2
a13 2
#LD_PROFILE=/B"/DE /NOE" cppopa3.obj
#OP_PROFILE=-Gh
d19 1
a19 1
#OP_OPTIMIZE=-O
@


1.8
log
@generalized event model
@
text
@d2 1
a2 1
all: mkmf test.exe
d7 4
d15 2
a16 2
OP_BROWSE=-Fb*
LD_BROWSE=/B"/BROWSE"
d20 2
d23 1
a23 2
CFLAGS= -Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG) $(OP_BROWSE)
#vmt placement -Wvft+
d34 1
a34 1
        -mv *.brs bak
d36 2
@


1.7
log
@update
@
text
@d20 1
a20 1
OBJS= controls.obj ctl_entry.obj ctl_list.obj device.obj focus.obj map.obj strng.obj test.obj types.obj win.obj
d39 1
@


1.6
log
@update
@
text
@d11 3
d16 2
a17 1
CFLAGS= -Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG)
d24 1
a24 1
   icc -Fetest.exe $(OP_DEBUG) $(OBJS) $(LD_PROFILE)
@


1.5
log
@mkmf
@
text
@d16 1
a16 1
OBJS= controls.obj device.obj focus.obj map.obj strng.obj test.obj types.obj win.obj
d32 2
d35 1
a35 1
focus.obj: sw_focus.h sw_event.h
d38 1
a38 1
test.obj: sw_device.h sw_types.h sw_except.h strng.h sw_win.h sw_map.h sw_event_handler.h sw_focus.h sw_event.h sw_controls.h
@


1.4
log
@*** empty log message ***
@
text
@d2 4
a5 1
all: test.exe
d13 1
a13 1
CFLAGS=-Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG)
d16 1
a16 3
OBJS= \
focus.obj device.obj win.obj strng.obj test.obj \
map.obj types.obj controls.obj
a29 33


sw_device.h:   sw_types.h sw_win.h

sw_except.h:   strng.h

sw_types.h:    sw_except.h

sw_win.h:      sw_types.h sw_map.h

sw_map.h:      sw_types.h

sw_pen.h:      sw_win.h

sw_controls.h: sw_win.h


device.obj:    device.c sw_device.h

win.obj:       win.c sw_win.h

map.obj:       map.c sw_win.h sw_map.h

strng.obj:     strng.c strng.h sw_except.h

test.obj:      test.c sw_device.h sw_controls.h

types.obj:     types.c sw_types.h 

controls.obj:  controls.c sw_controls.h

focus.obj      focus.c sw_focus.h

d31 8
a38 2


@


1.3
log
@works a little
@
text
@a3 4
clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)
        -mv *.?~ bak
        -mv *.brs bak
d5 2
d8 4
d13 3
a16 2
CFLAGS=-Tdp -Q -Ti
#vmt placement -Wvft+
d18 2
a19 1
OBJS= device.obj win.obj strng.obj test.obj map.obj types.obj controls.obj
d21 4
a24 2
test.exe: $(OBJS)
   icc -Fetest.exe -Ti $(OBJS)
d59 6
@


1.2
log
@update
@
text
@d2 9
d13 1
d15 1
a15 1
OBJS= device.obj win.obj strng.obj test.obj
a20 1
sw_device.h: sw_types.h sw_win.h
a21 1
sw_except.h: strng.h
a22 1
sw_types.h:  sw_except.h
d24 14
a37 1
sw_win.h:    sw_types.h
d40 1
a40 1
device.obj:  device.c sw_device.h
d42 1
a42 1
win.obj:     win.c sw_win.h
d44 1
a44 1
strng.obj:   strng.c strng.h sw_except.h
d46 1
a46 1
test.obj:    test.c sw_device.h
d48 1
d50 1
d52 1
@


1.1
log
@Initial revision
@
text
@d3 1
a3 1
CFLAGS=-Tdp
d5 1
a5 1
device.obj: device.c
d7 20
a26 1
win.obj:    win.c
@
