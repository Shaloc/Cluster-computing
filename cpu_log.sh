#! /bin/bash
sed -i '1d' ~/cpu.log
vmstat>>~/cpu.log
linenum=`cat ~/cpu.log | wc -l`
linenum_last2=`expr $linenum - 2`
sed -i ''"$linenum_last2"'d' ~/cpu.log
sed -i ''"$linenum_last2"'d' ~/cpu.log



