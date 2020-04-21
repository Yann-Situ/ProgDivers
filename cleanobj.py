import numpy as np
import matplotlib.pyplot as plt
import os.path
import sys

## 
script_path = os.path.abspath(__file__)
directory_path = script_path[:-len("cleanobj.py")]

if len(sys.argv) < 1:
    print("Arguments : obj extnsion files\nExample : python3 cleanobj.py bimba.obj")
    sys.exit()
##

def main():

    for objfile in sys.argv[1:] :
        objname =  objfile.split(".")[0]
        newfile = open(directory_path+objname+"_clean.obj", "w")
        firstfile = open(directory_path+objfile, "r")
        clean(firstfile, newfile)                
        firstfile.close()
        newfile.close()
    return 0

def clean(f1, f2):
    for l in f1:
        lsp = l.replace("\n","").split(" ")
        if lsp[0] == "f":
            n = len(lsp)
            for i in range(0, n-4+1):
                f2.write("f "+lsp[1]+" "+lsp[2+i]+" "+lsp[3+i]+"\n")
        else :
            f2.write(l)
main()

