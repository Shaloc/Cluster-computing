import sys
import os
#make data easy to be copied to matlab
os.remove(sys.argv[2])
lines = []
i = 0
with open(sys.argv[1], 'r') as f_in:
    for line in f_in:
        if line[0] != '*':
            i = i + 1
            line = line[0:-1] + ';' + '\n'
            lines.append(line)
            if i == 12:
                i = 0
                lines.append('\n=================\n')

        
 
with open(sys.argv[2], 'w') as f_out:
    f_out.writelines(lines)