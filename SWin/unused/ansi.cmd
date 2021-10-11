/* */

esc=x2c('1b')


adv= esc'[J'

clear_screen= esc'[J'
home= esc'[H'
adv_keft= esc'[D'
adv_right= esc'[C'
adv_down= esc'[B'
adv_up= esc'[A'

color_code.lighgray=esc'[37m'
color_code.cyan=esc'[36m'
color_code.magenta=esc'[35m'
color_code.blue=esc'[34m'
color_code.brown=esc'[33m'
color_code.green=esc'[32m'
color_code.red=esc'[31m'
color_code.black=esc'[30m'

color_code.invisible=esc'[8m'
color_code.reverse=esc'[7m'
color_code.darkgray_bg=esc'[5m'
color_code.blue1=esc'[4m'
color_code.intense=esc'[1m'
color_code.normal=esc'[0m'

color_code.blink=esc'[5m'


color_code.green_bg=esc'[42m'
color_code.blue_bg=esc'[44m'
color_code.brown_bg=esc'[43m'

go=esc'[2;10H'

'cls'
Say color_code.brown_bg color_code.red color_code.intense color_code.blink go'Te'adv'st'





