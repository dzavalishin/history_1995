all: uu2.set uu2.inf uu2.txt

zip: uu2doc-p.zip uu2doc-t.zip uu2doc-2.zip

uu2doc-p.zip: uu2.set
    zip uu2doc-p.zip uu2.set uu2.hdt uu2key.db uu2lnk.db uu2top.db uu2key.px uu2lnk.px uu2top.px helpeng.exe runme.bat

uu2doc-t.zip: uu2.txt uu2.gls uu2.toc
    zip uu2doc-t.zip uu2.txt uu2.gls uu2.toc

uu2doc-2.zip: uu2.inf
    zip uu2doc-2.zip uu2.inf

uu2.set: uu2.hdf
    helpc /px+ /os2 /txt UU2.hdf 

View:
    helpeng.exe UU2 

uu2.ipf: uu2.hdf
    helpc /OS2 UU2.hdf

uu2.inf: uu2.ipf
    ipfc /inf UU2.ipf

uu2.txt: uu2.hdf
    helpc /txt UU2.hdf

