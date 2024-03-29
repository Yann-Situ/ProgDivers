import os.path
import sys
from nodeword import *
from grammar import *
from parser import *

script_path = os.path.abspath(__file__)
directory_path = script_path[:-len("newlogisme.py")]

grammarfile = directory_path+"grammar.txt"
tokenfile = directory_path+"tokens.txt"

def test():
    print("----tests----")
    testexps = ["A1(  papa,oklm,V(pop, ip))", "coucou","sens(sens(token1,sens(token)),token3)","papa()", "sens(missparren(a())), token1)","not(()"]
    for e in testexps:
        print(e+" :")
        node = createNodeFromExp(e)
        #print(node)
        print(node.disp(0))

    print("import the tokens and grammar : ")
    gram = Grammar(grammarfile,tokenfile)
    print(gram)
    print("\nTranslate :")
    testexps = ["V(bio)", "A1(ique,POSS(bio,N(exo)))", "A1(ique,A1(log,N(exo)))", "A1(ero,A1(bio,POSS(phag,N(auto))))", "A1(log,POSS(bio,N(exo)))"]
    for e in testexps:
        print(e+" :")
        node = createNodeFromExp(e)
        print(node)
        print(node.translate(gram.dico))

    print("\n")
    #ruleshape = [['$1', 'log', 'ie'],['*'] ,['*', '$1', 'ie'],['$1','$2','$3','e'],['$1','*','e']]
    words = ["biolog","biologie","erophagologie","bioexologie","egoerophagie", "bioautoique", "bioerotique", "exoter", "biovorphilie"]
    for i in range(min(len(gram.rules),len(words))):
        print("="*30+words[i]+"="*(40-len(words[i])))
        #print(str(gram.rules[i].shape)+" <---> "+words[i])
        #m = constructMatchTree(words[i], gram.rules[i].shape, gram.dico)
        #print(m)
        #print(m.disp())

        #explist = constructExprList(words[i], gram, i)
        #for e in explist:
        #    print(e)
        #print("")

        explist = meaningsExprList(words[i], gram)
        for e in explist:
            print(e)
            print(postprod(createNodeFromExp(e).translate(gram.dico)))
    return 0

DEBUG_NODEWORD = "-debug" in sys.argv[1:] or "-debug-nodeword" in sys.argv[1:]


def main():
    if sys.argv[1:] == [] or "-h" in sys.argv[1:] or "--help" in sys.argv[1:]:
        print("""Usage : pyhton3 newlogisme.py mot1 mot2 ... [-debug] [-debug-nodeword] [-debug-parser] [-rules] [-g "grammarfile"] [-t "tokenfile"]

Traduit les mots donnés en argument à partir des grammaires et dictionnaire du fichier du programme newlogisme.py. On propose plusieurs traductions en fonction de l'étymologie des mots en arguments.

-debug          : active le mode debug.
-debug-nodeword : active le mode debug pour la construction et gestion des nodeword.
-debug-parser   : active le mode debug pour le parser, le match avec les règles et la transformation en expression.
-rules          : affiche les règles de la grammaire avec lesquelles les mots ont matché [x] ou non [ ].
-g "grammarfile": pour utiliser le fichier de grammaire "grammarfile" ( directory_path+"grammar.txt" par default)
-t "tokenfile"  : pour utiliser le fichier de token "tokenfile" ( directory_path+"token.txt" par default)
                
-h ou --help affiche ce message.""")
        return 1
    grammarfile = directory_path+"grammar.txt"
    tokenfile = directory_path+"tokens.txt"
    i = 1
    while i < len(sys.argv) and sys.argv[i] != "-g":
        i += 1
    if i < len(sys.argv)-1 and sys.argv[i] == "-g" :
        s = sys.argv[i+1]
        sys.argv = sys.argv[:i]+sys.argv[i+2:]
        grammarfile = s
        
    i = 1
    while i < len(sys.argv) and sys.argv[i] != "-t":
        i += 1
    if i < len(sys.argv)-1 and sys.argv[i] == "-t" :
        s = sys.argv[i+1]
        sys.argv = sys.argv[:i]+sys.argv[i+2:]
        tokenfile = s

    if "-g2" in sys.argv[1:]:
        grammarfile = directory_path+"grammar2.txt"
        tokenfile = directory_path+"tokens2.txt"
        
    if DEBUG_NODEWORD :
        print("======== IMPORT GRAMMAR AND TOKENS ========")
        print(grammarfile+"   "+tokenfile+"\n")
    gram = Grammar(grammarfile,tokenfile)
    
    for arg in sys.argv[1:]:
        if arg[0] != '-':
            if len(sys.argv[1:]) > 1 : print("="*30+" "+arg+" "+"="*(40-len(arg)))
            explist = meaningsExprList(arg.lower(), gram)
            trads = []
            for e in explist:
                if DEBUG_NODEWORD :
                    print("----------- Create NodeWord from "+e+" :")
                nt = createNodeFromExp(e)
                t = nt.translate(gram.dico)
                if DEBUG_NODEWORD :
                    print(nt.__str__()+" ------> "+t)
                
                if "ERROR" not in t:
                    trads.append(t)

            for trad in list(set(trads)):
                print(postprod(trad))
    

    return 0

main()
