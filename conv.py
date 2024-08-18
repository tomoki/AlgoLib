#!/usr/bnn/env python3
#coding:utf-8

import sys

with open(sys.argv[1]) as f:
    lines = f.readlines()
    incode = False
    for line in lines:
        line = line.rstrip()
        if incode:
            if line.startswith("~~~~"):
                if "include=" in line:
                    pass
                else:
                    incode = False
            else:
                print(("    " + line).rstrip())
        else:
            if line.startswith("~~~~"):
                incode = True
                if "include=" in line:
                    fname = line.split('"')[1]
                    print(".. literalinclude:: " +  fname)
                else:
                    print(".. code-block:: cpp")
                    print("")
            else:
                if line.startswith("###"):
                    print(line[3:].strip())
                    print("====================")
                elif line.startswith("##"):
                    print("*********************")
                    print(line[2:].strip())
                    print("*********************")
                elif line.startswith("#"):
                    print("######################")
                    print(line[1:].strip())
                    print("######################")
                else:
                    print(line)