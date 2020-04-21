import os.path
import sys

from grammar import *
from nodeword import *

DEBUG = False
PRINT_RULES_MATCH = False

if "-debug" in sys.argv[1:] or "-debug-parser" in sys.argv[1:] :
    print("-debug-parser activé")
    DEBUG = True

if "-rules" in sys.argv[1:] :
    print("-rules activé")
    PRINT_RULES_MATCH = True

class MatchTree():
    """
    classe arborescente contenant à chaque noeud un préfixe du mot correspondant au niveau h de l'input d'une règle. Les filles sont les différentes interprétation des suffixes
    """
    def __init__(self, string, kids, pref = ""):
        self.s = string
        self.kids = kids
        self.pref = pref # il faut toujours avoir pref+s= mot de base

    def disp(self, prof = 0, spacebegin = "\n", spaceend = "", spaceprof = "| ", spacebetween = ""):
        temp = spaceprof*prof
        temp += self.pref+"_"+self.s
        temp +=spacebegin
        for n in self.kids:
            temp += n.disp(prof+1, spacebegin, spaceend, spaceprof, spacebetween)
            temp += spacebetween
        if len(self.kids) > 0 and len(spacebetween) > 0:
            temp = temp[:-len(spacebetween)] #on enlève l'éventuel spacebetween en trop
        temp+=spaceend
        return temp
    
    def __str__(self):
        return self.disp(0,"(",")","",",")
    
def match(s, symbol, d):
    """
    input : s une string, symbol une string d'un input d'une grammaire, d le dico des tokens
    retourne [] si incompatible, sinon retourne la liste des prefixes matchant avec symbol (pouvant être $1, * ...)
    """
    l = []
    
    if symbol == '*':
        l += list(range(len(s)+1))
    elif '+' in symbol :
        orlist = symbol.split('+')
        for symbol2 in orlist:
            #            if symbol2 == '':
            #                l.append(0)
            #            else:
            l += match(s, symbol2, d)
    elif symbol != '' and symbol[0] == '$':
        for i in range(len(s)):
            pref = s[:i+1]
            if pref in d:
                l.append(i+1)
        
    else : #cas des tokens et caractères
        if symbol == s[:len(symbol)]:
            l.append(len(symbol))
    return l


def constructMatchTreeAux(s, rshape, d, pref = ""):    
    if s == "" and rshape == [] :# or (len(rshape) == 1 and '' in rshape[0].split('+'))) : # si jamais onfini par un + contenant un ''
            return [], True # sinon, rshape peut être un truc du genre blabla++bla ou bla+
    elif rshape == []:
            return [], False # false

    symbol = rshape[0]
    m = match(s, symbol, d)

    matchlist = []
    b = False
    for i in m:
        ki, boolmatch = constructMatchTreeAux(s[i:], rshape[1:], d, pref+s[:i])
        if boolmatch : # true, on match au moins avec une règle !
            matchlist.append(MatchTree(s[i:], ki, pref+s[:i]))
            b = True
        else: # false, s[i:] ne match pas rshape[1:]
            pass
    return matchlist, b

def constructMatchTree(s, rshape, d):
    """
    Construit un MatchTree de s avec l'input de règle rshape et le dico d
    """
    ml, boolmatch = constructMatchTreeAux(s, rshape, d)
    if boolmatch :
        return MatchTree(s, ml)
    else :
        return MatchTree("NOMATCH",[])
    
###--------------------------------------------------------------------------###

def constructExprListAux(m, shape, expr, d, g, prof = 0):
    """
    retourne la liste des expr correspondant au matchTree m avec la règle d'input shape et d'output expr, en utilisant le dico d. On remplace les $i et les * et on fait des appels récursifs. la grammaire g est nécessaire pour les appels récursifs utilisant *.
    """
    if m.s == "" and shape == []: # or (len(shape) == 1 and '' in shape[0].split('+'))) : # si jamais onfini par un + contenant un ''
        return [expr]
    elif shape == []: # or m.s == "":
        print('ERROR in constructExprListAux : shape == [] and m.s != "" or the contrary... The matchTree is not correct. m='+m.disp(0,"(",")","",","))
        return [expr]
         
    elist = []
    c = shape[0]
    if DEBUG: print(" | "*prof+m.pref+"_"+m.s+" ; shape="+shape.__str__())
        
    if c == '*':
        for k in m.kids:
            
            instar = m.s if len(k.s) == 0 else m.s[:-len(k.s)] #if ternaire
            if DEBUG : print(" | "*prof+"'*'='"+instar+"'")
            if instar != '':
                instarExprList = ["N("+instar+")"] if instar in d else meaningsExprList(instar, g, prof+1) # on choisi le sens nominal du token si c'en est un
                if DEBUG : print(" | "*(prof+1)+"*List="+instarExprList.__str__())
                
                for starreplace in instarExprList:
                    e = expr.replace(c, starreplace)
                    elist += constructExprListAux(k, shape[1:], e, d, g, prof+1)

    elif '+' in c:# plus compliqué si l'on considère les +*+$i...
        for k in m.kids:
            #tok = m.s[:-len(k.s)] if len(k.s) != 0 else m.s
            elist += constructExprListAux(k, shape[1:], expr, d, g, prof+1)
        
    elif c[0] == '$':
        for k in m.kids:
            tok = m.s[:-len(k.s)] if len(k.s) != 0 else m.s
            e = expr.replace(c,tok) # on remplace les $i par le token
            elist += constructExprListAux(k, shape[1:], e, d, g, prof+1)
    else : # token ou lettres
        for k in m.kids:
            elist += constructExprListAux(k, shape[1:], expr, d, g, prof+1)
    return elist

def constructExprList(s, g, i, prof = 0):
    """
    retourne la liste des expr correspondant à la string s avec la règle i de la grammaire g, en utilisant le dico g.dico
    la fonction fait appel à sa fonction auxiliaire récursive, qui fait elle même appel (dans le cas de *) à la fonction vérifiant un mot avec toutes les règles de grammaires, qui appelle cette fonction... attention aux recursions.

    la fonction fait aussi appel à constructMatchTree
    """
    r = g.rules[i]
    d = g.dico
    matcht = constructMatchTree(s, r.shape, d)
    if matcht.s == "NOMATCH":
        return []
    else :
        if DEBUG : print(" | "*prof+matcht.__str__())
        return constructExprListAux(matcht, r.shape, r.expr, d, g, prof)

###--------------------------------------------------------------------------###    
    
def meaningsExprList(s, g, prof = 0):
    """
    renvoie une liste d'expr associé aux sens de s avec la grammaire g
    """
    elist = []
    for i in range(len(g.rules)-1): # la dernière règle est ultra générique, on l'utilise en dernier secours
        e = constructExprList(s, g, i, prof)
        if DEBUG or (PRINT_RULES_MATCH and prof == 0):
            if e != []:
                print(" | "*prof+"[x] : '"+s+"' <---> "+g.rules[i].__str__())
            else:
                print(" | "*prof+"[ ] : '"+s+"' <---> "+g.rules[i].__str__())
        elist+=e
    if elist == [] or prof > 0 : # on utilise la dernière règles
        i = len(g.rules)-1
        e = constructExprList(s, g, i, prof)
        if DEBUG or (PRINT_RULES_MATCH and prof == 0):
            if e != []:
                print(" | "*prof+"[x] : '"+s+"' <---> "+g.rules[i].__str__())
            else:
                print(" | "*prof+"[ ] : '"+s+"' <---> "+g.rules[i].__str__())
        elist+=e
    return list(set(elist)) # pour éliminer les doublons
