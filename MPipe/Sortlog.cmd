rm sorted\*.log
gawk -f Sortlog.awk --assign=out=call   <call_log
gawk -f Sortlog.awk --assign=out=answer <answer_log
