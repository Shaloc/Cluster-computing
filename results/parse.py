#!/usr/bin/env python3
import sys

print("parsing " + sys.argv[1] + " ---> " + sys.argv[2])
f = open(sys.argv[1])
g = open(sys.argv[2], 'w')

for line in f.readlines():
    if line.startswith('Use') or (line.startswith('*') and line.find('node') > 0):
        line = line.replace('*','=')
        line = line.replace('Use', '')
        line = line.replace('ms', '')
        line = line.strip()
        print(line, file=g)
        print("prasedline: " + line)
print("finished!")
f.close()
g.close()

