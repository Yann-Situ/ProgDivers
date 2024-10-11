import os.path
import sys

Nbsens = 5
_TOKEN = 0
_NOUN = 2
_AND = 100
_ACTION1 = 1
_ACTION2 = 6
_NOUN2 = 7
_VERBE = 4
_POSSESSIF = 3
_ADJECTIF = 5

DEBUG = False
if "-debug" in sys.argv[1:] or "-debug-nodeword" in sys.argv[1:] :
    print("-debug-nodeword activé")
    DEBUG = True


class NotGoodParren(Exception):
    def __init__(self, value):
        self.string = repr(value)
    def __str__(self):
        return self.string

class NotGoodNode(Exception):
    def __init__(self, node):
        self.node = node
    def __str__(self):
        return str(self.node)

    
class NodeWord():
    """
    Class contenant une strcture d'arbre pour les significations des mots
    """
    def __init__(self, sens = 0, args = [], token="_NOT_A_TOKEN_"):
        self.token = token #string
        self.sens = sens # int code (ex : _NOUN ...)
        self.args = args.copy() # NodeWord list (children)
    def disp(self, prof = 0, spacebegin = "\n", spaceend = "", spaceprof = "| ", spacebetween = ""):
        """
        Cree une string de l'arbre de NodeWord ainsi :
        prof = k ; op(a1,a2) -> spaceprof^k 'op' spacebegin disp('a1') spacebetween disp('a2') spaceend
        pour obtenir l'exp initiale : disp(0,"(",")","",","))
        """
        temp = spaceprof*prof
        if self.sens == _NOUN:
            temp += "Nom"
        elif self.sens == _ACTION1:
            temp += "Action1"
        elif self.sens == _ACTION2:
            temp += "Action2"
        elif self.sens == _VERBE:
            temp += "Verbe"
        elif self.sens == _POSSESSIF:
            temp += "Possessif"
        elif self.sens == _AND:
            temp += "And"
        elif self.sens == _ADJECTIF:
            temp += "Adjectif"
        elif self.sens == _TOKEN:
            temp += self.token.lower()
        else :# cas des tokens, $i et * ou autre
            temp += "UNKNOWN SENSE '"+repr(self.sens)+"'"
        temp+=spacebegin
        #print(temp)
        for n in self.args:
            temp += n.disp(prof+1, spacebegin, spaceend, spaceprof, spacebetween)
            temp += spacebetween
        if len(self.args) > 0 and len(spacebetween) > 0:
            temp = temp[:-len(spacebetween)] #on enleve l'éventuel spacebetween en trop
        temp+=spaceend
        return temp
    def __str__(self):
        return self.disp(0,"(",")","",",")

    
    def translate(self, dico):
        """
        traduit le nodeword récursivement à l'aide du dictionnaire des token (+sens) dico.
        La sortie est une string non nétoyer (on peut avoir encore des 'de le' ou 'à le'...).
        """
        sens = self.sens
        s = ""
        try:
            if sens == _TOKEN or len(self.args) == 0 :# cas des tokens ou des fonctions sans arguments donc ne necessitant pas de tokens
                raise NotGoodNode(self) # on n'est pas censé traduire les tokens, les fonctions les appelant le font directement avec le sens voulu.               
            else :
                tok = self.args[0]
                kids = self.args[1:]

                ###
                # Test de validités
                ###
                if tok.token not in dico:
                    if DEBUG : print(tok.token+" is not in the dico.")
                    raise NotGoodNode(self)

                trad = dico[tok.token][sens-1] # -1 car le dico commence ces sens à 0 mais le sens 0 correspond au token... on met donc -1
                if trad == '': # cas de certains token n'ayant pas de sens pour tous
                    if DEBUG : print(tok.token+" doesn't have the meaning "+str(sens))
                    raise NotGoodNode(self)
                
                nbkids = 0
                if trad.count("$") > len(kids):
                    if DEBUG : print(trad+" have too many $ and tree doesn't have enough kids.")
                    raise NotGoodNode(self)
                else:
                    nbkids = trad.count("$")

                ###
                #début de la traduction !
                ###
                tradtab = trad.split('$')
                s += tradtab[0] # existe car trad non vide
                for tradpiece in tradtab[1:]:
                    numkid = [int(i) for i in tradpiece.split() if i.isdigit()][0]# technique ultra shlag pour avoir le premier nombre isolé de la string tradpiece
                    numkid -= 1 # car on compte dans la grammaire avec $1, $2, ... $0 rpz le token
                    assert(numkid < nbkids)
                    s += kids[numkid].translate(dico)
                    s += tradpiece[len(str(numkid+1)):]
        except NotGoodNode as exc:
            if DEBUG : print("ERROR : expr "+str(exc)+" is not a traducable nodeword. put 'ERROR' instead")
            return "ERROR" #solution chelou TODO : gérer c'est cas et permettre l'utilisation des A2 etc.
        return s

            
def sensFromChars(exp):
    cut = exp.split('(')
    if cut[0] == "N":
        return _NOUN
    elif cut[0] == "A1":
        return _ACTION1
    elif cut[0] == "A2":
        return _ACTION2
    elif cut[0] == "V":
        return _VERBE
    elif cut[0] == "POSS":
        return _POSSESSIF
    elif cut[0] == "AND":
        return _AND
    elif cut[0] == "ADJ":
        return _ADJECTIF
    else :# cas des tokens, $i et * ou autre
        return _TOKEN
        
def createNodeFromExp(e, prof = 0):
    """
    Crée un NodeWord depuis une expression du type "OP(arg1,arg2,...)"
    """
    e = e.replace(" ","")
    i = 0
    while i < len(e) and e[i] != '(':
        i+=1
    
    j = len(e)-1
    while j > 0 and e[j] != ')':
        j-=1
    
    if j != 0 and i != len(e)-1: # e(i) est  '(' et e(j) est ')'
        argexp = e[i+1:j]
        args = []
        begin = 0
        try:
            lpar = 0
            rpar = 0
            for end in range(len(argexp)):
                if argexp[end] == '(':
                    lpar = lpar + 1
                elif argexp[end] == ')':
                    rpar = rpar + 1

                if lpar < rpar:
                    raise NotGoodParren(argexp)
                elif lpar == rpar and argexp[end] == ',':
                    args.append(createNodeFromExp(argexp[begin:end], prof+1))
                    begin = end+1
            if len(argexp) != 0:
                args.append(createNodeFromExp(argexp[begin:len(argexp)], prof+1))

            if lpar != rpar:
                raise NotGoodParren(argexp)
        except NotGoodParren as exc:
            if DEBUG : print("ERROR : expression "+str(exc)+" has wrong parenthesis")
        nw = NodeWord(sensFromChars(e), args.copy())
        
    else : # cas d'un token
        nw = NodeWord(_TOKEN,[],e)

    if DEBUG : print(" | "*prof+"'"+e+"' -> "+nw.__str__())
    return nw


###--------------------------------------------------------------------------###

def postprod(s):
    """
    nettoie la string s
    """
    s = s.replace(" de le ", " du ")
    s = s.replace(" de les ", " de ")
    s = s.replace(" à le ", " au ")
    s = s.replace(" à les ", " aux ")
    s = s.replace(" de qui ", " de ce qui ")
    s = s.replace(" à qui ", " à ce qui ")
    s = s.replace(" par qui ", " par ce qui ")
    s = s.replace(" de un ", " d'un ")
    s = s.replace(" de une ", " d'une ")

    for pref in ["des", "plusieurs", "deux", "trois", "quatres"]:
    	s = s.replace(" "+pref+" l'", " "+pref+" ")
    	s = s.replace(" "+pref+" les ", " "+pref+" ")
    	s = s.replace(" "+pref+" le ", " "+pref+" ")
    	s = s.replace(" "+pref+" la ", " "+pref+" ")
    	s = s.replace(" "+pref+" une ", " "+pref+" ")
    	s = s.replace(" "+pref+" un ", " "+pref+" ")
    	s = s.replace(" "+pref+" qui est ", " "+pref+" choses qui sont ")
    return s.capitalize()
