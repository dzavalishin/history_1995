!include version.mak

alpha:
   copy $(DIR)\$(FILE) $(DIR)\AU1_$(VER).Zip
   N:\Net\NEF\nef -cN:\Net\NEF\NEF.CFG match $(DIR)\AU1_$(VER).Zip UU2 "U1 $(VER) Alpha"

