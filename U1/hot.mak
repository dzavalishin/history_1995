!include version.mak

ver_mark:
    ci -u -wBuilder -m"Version $(VER)" history
    ci -u -wBuilder -m"Version $(VER)" hot.txt
    echo. >hot.fix
    echo ! U1 Builder: version number changed to $(VER) >>hot.fix
    echo. >>hot.fix
    copy hot.fix+hot.txt hot.new
    del hot.txt
    ren hot.new hot.txt
    del hot.fix
