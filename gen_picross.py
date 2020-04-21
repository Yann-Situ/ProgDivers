import numpy as np
import matplotlib.pyplot as plt
import os.path
import sys

## 
script_path = os.path.abspath(__file__)
directory_path = script_path[:-len("gen_picross.py")]

if len(sys.argv) ==1:
    print("One argument : the image file.\nExample : python3 gen_picross.py example.jpg")
    sys.exit()

imagefile = sys.argv[1]
imagename = imagefile.split(".")[0]
##

def main():
    global directory_path, imagefile, imagename
    
    print("I will treat the image", imagefile)
    Im = plt.imread(directory_path+imagefile)
    
    s1 = len(Im)
    s2 = len(Im[0])

    print("The size of the image is ",s1, "x", s2, sep='')
    
    if s1 > 40 or s2 > 35:
        print("The maximum size to fit in a latex document is about 35x30")
        sys.exit()
    
    mat = np.array([[1 for j in range(s2)] for i in range(s1)])
    counter = 0
    for i in range(s1):
        for j in range(s2):
            if Im[i][j][0] != 0 :
                mat[i][j] = 0
                counter += 1
    
    rate = (1 - counter/(s1*s2))*100
    
    print("There is ", rate, "% of black spots in this picture.\nOptimal picross rate is between 30 and 40.", sep = '')
    
    # print("The image will look like this :")
    # for i in range(s1):
    #     for j in range(s2):
    #         print(mat[i][j], end='')
    #     print('')
        
    col, row = [], []
    for i in range(s1):
        row.append(compute_line(mat, i, s2))
    
    for j in range(s2):
        col.append(compute_column(mat, j, s1))
    
    s = give_latex_source(mat, s1, s2, col, row)

    f = open(directory_path+imagename+".tex", "w")
    f.write(s)
    f.close()
    
    print("Latex source code written, please compile")
    
    

def compute_line(mat, i, n):
    v = []
    temp=0
    for j in range(n):
        if mat[i][j] == 1:
            temp +=1
        elif temp != 0:
            v.append(temp)
            temp = 0
    if temp != 0:
        v.append(temp)
    return v


def compute_column(mat, j, n):
    v = []
    temp=0
    for i in range(n):
        if mat[i][j] == 1:
            temp +=1
        elif temp != 0:
            v.append(temp)
            temp = 0
    if temp != 0:
        v.append(temp)
    return v



def give_latex_source(mat, s1, s2, col, row):
    s = ""
    s += r'''\documentclass[12pt]{article}'''
    s += "\n"
    s += r'''\newcommand{\specialcell}[1]{ \begin{tabular}[b]{@{}c@{}}#1\end{tabular}}'''
    s += "\n" 
    s += r'''\begin{document}'''
    s += "\n" 
    s += r'''\pagestyle{empty}'''
    s += "\n"
    s += r'''\hspace*{-4cm}'''
    s += "\n"
    s += r'''\begin{tabular}{r'''
    for i in range(s2):
        s += "|p{1mm}"
    s += "|}\n"
    s += "& "
    for j in range(s2):
        s += r'''\hspace*{-2mm}\specialcell{'''
        if col[j] == []:
            s += "0}  "
        else:
            for el in col[j]:
                s +=  str(el) + r''' \\ '''
            s = s[:-3]
            s += "} & "
    s = s[:-2]
    s += r'''\\''' + "\n\hline\n"
    for i in range(s1):
        if row[i] == [] :
            s += "0"
        else:
            for el in row[i]:
                s += str(el) + " "
        s += s2*"&" + r'''\\''' + "\n\hline\n"
    s += "\end{tabular} \n\end{document}"
    return s
    

main()