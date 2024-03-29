import os.path
import sys

from nodeword import *

class Rule():
    """
    Classe contenant une règle.

    self.shape contient la liste de l'input voulut de la règle
    self.tree contient le node de l'output
    self.expr contient l'expression du node de l'output
    """
    def __init__(self, inputstr, outputstr):
        self.shape = [x for x in inputstr.split(" ") if x != ""] # on split selon les espaces et on enlève les éventuels caractères vide dans la liste
        self.tree = createNodeFromExp(outputstr.replace(" ",""))
        self.expr = outputstr.replace(" ","")
        
    def __str__(self):
        return str(self.shape)+" -> "+self.tree.__str__()
    
class Grammar():
    """
    Contient la grammaire et les fonctions de traductions des neologismes
    
    self.dico content le dictionnaire des tokens avec leur différents sens
    self.rules contient la liste des règles
    """
    def __init__(self, grammarF, tokensF):
        """ Appel les fonctions create pour initialiser la grammaire """
        self.createTokens(tokensF)
        self.createGrammar(grammarF)
        
    def __str__(self):
        s = "Dictionaire :\n"+self.dico.__str__()+"\nRules :\n["
        for r in self.rules:
            s+= r.__str__()+", "
        if len(self.rules) > 0:
            s = s[:-len(", ")]
        s+="]"
        return s

    def createGrammar(self, grammarFilename):
        """ Crée la grammaire à partir d'un fichier texte la contenant
        bio $1 logique -> [bio, $1, logique]
        exo * -> [exo, *]
        ainsi que le tableau de sens correspondant
        bio*logique -> Sens(Bio,Action(logique, Nom(*)) etc...
        """
        grammarfile =  open(grammarFilename, "r")
        self.rules = []
        for l in grammarfile:
            l = l.replace("\n","")
            l = l.replace("\t"," ")
            ltab = l.split("->")
            if len(ltab) > 0 and len(ltab[0]) > 0 and ltab[0].replace(" ","")[0] != "#" :
                # morphème - Groupe nominale - Action1($1) - Action2([$i]) - Verbe([$i]) - Possessif([])
                self.rules.append(Rule(ltab[0], ltab[1]))
        grammarfile.close()

    def createTokens(self, tokensFilename):
        """ Crée le tableau/dictionnaire de tous les tokens avec leurs différents sens/noms...
        bio : ["la vie", "Le vie de "]
        """
        tokfile =  open(tokensFilename, "r")
        self.dico = dict([])
        for l in tokfile:
            l = l.replace("\n","")
            l = l.replace("\t"," ")
            ltab = l.split("|")
            if len(ltab) > 0 and len(ltab[0]) > 0 and ltab[0].replace(" ","")[0] != "#" :
                # morphème - Groupe nominale - Action1($1) - Action2([$i]) - Verbe([$i]) - Possessif([])
                toktab = []
                for sense in ltab[1:]:
                    sense = sense.strip() #enlever les espaces au début et à la fin
                    toktab.append(sense)
                self.dico[ltab[0].replace(" ","")] = toktab.copy()
        tokfile.close()
