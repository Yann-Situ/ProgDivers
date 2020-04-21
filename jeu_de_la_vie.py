import numpy as np
from tkinter import *
import time

"""
> Mon mini-projet est une interface graphique permettant de simuler des automates cellulaires.
> Cela a été réalisé en Python avec Tkinter.
>
> On dispose de trois règles d'automates différents :
>
>     Le jeu de la vie classique
>     Le récif corallien 1 :
>     les cellules naissent (cyan) si elles ont exactement 3 cellules vivantes voisines.
>     Les cellules disparaissent (en laissant un fond vert marin qui est juste visuel mais qui est équivalent à une case vide) si elles ont 3 cellules vivantes voisines ou moins.
>     Les cellules vivantes vieillissent (cyan de plus en plus foncé) jusqu'à atteindre leur durée de vie (paramètre ajustable) : alors elle deviennent des cellules sèches (rouge) qui ne peuvent plus changer d'état et qui ne compte pas comme des cellules vivantes.
>     Le récif corallien 2 :
>     Mêmes règles que le précédent pour la naissance et la disparition.
>     Les cellules vivantes vieillissent jusqu'à atteindre leur durée de vie (paramètre ajustable) : alors elle deviennent des cellules sèches (cyan foncé) mais qui peuvent changer d'état et qui compte comme des cellules vivantes.
>
> Il est possible de faire varier la durée de vie des cellules. Il est possible de faire une simulation pas à pas avec la touche Entrée. Il est possible de faire une simulation animée en faisant varier le délai entre deux étapes de l'automate à l'aide du bouton Simulation.
>
> L'état initiale (accessible via le bouton Reset) est un rectangle centrale où l'état des cellules est tiré aléatoirement. La convention pour les bords de la grille est qu'aucune cellule peut naître sur les bords.
>
> Je voulais faire un système pour pouvoir changer l'état des cellules en cliquant mais je n'ai pas eu le temps... (il y a quelques traces dans mon code).
>
> Pour lancer le programme, il suffit de l'interpréter : le code appellera la fonction main qui ouvrira l'interface graphique.

"""
HEIGHT_F = 600
WIDTH_F = 800
NUM_MAX = 255

DUREE_VIE = 42
DUREE_VIE_2 = 10
DELAY = 100

def couleur(n):
    if n == 0 :
        return 'ivory'
    else :
        #rrvvbb
        x = int(210*5 / (n+4))
        c = [x//16,x%16]
        ct = ['0','0']
        for i in range(2):
            if c[i] < 10:
                ct[i] = str(c[i])
            elif c[i] == 10:
                ct[i] = 'a'
            elif c[i] == 11:
                ct[i] = 'b'
            elif c[i] == 12:
                ct[i] = 'c'
            elif c[i] == 13:
                ct[i] = 'd'
            elif c[i] == 14:
                ct[i] = 'e'
            elif c[i] == 15:
                ct[i] = 'f'
        s = str(ct[0])+str(ct[1])+str(ct[0])+str(ct[1])
        c = [n//16,n%16]
        for i in range(2):
            if c[i] < 10:
                ct[i] = str(c[i])
            elif c[i] == 10:
                ct[i] = 'a'
            elif c[i] == 11:
                ct[i] = 'b'
            elif c[i] == 12:
                ct[i] = 'c'
            elif c[i] == 13:
                ct[i] = 'd'
            elif c[i] == 14:
                ct[i] = 'e'
            elif c[i] == 15:
                ct[i] = 'f'
        return '#'+str(ct[0])+str(ct[1])+s
colors = [couleur(i) for i in range(NUM_MAX)]
colors[100] = 'brown4'
colors[101] = 'medium sea green'

class Interface(Frame):#l'interface est un widget
    def __init__(self, fenetre,wid=WIDTH_F, hei=HEIGHT_F, wg=WIDTH_F//10, hg=HEIGHT_F//10, init = 2, yinit = HEIGHT_F//40, xinit = WIDTH_F//40):
        # On crée une fenêtre, racine de notre interface
        self.w = wid
        self.h = hei
        
        #init
        Frame.__init__(self,fenetre, width=wid, hei=HEIGHT_F)
        self.pack(fill=BOTH)
        self.cadre = LabelFrame(self, width=WIDTH_F, height=HEIGHT_F+20, borderwidth=1, text="Automate cellulaire", relief=GROOVE)
        self.cadre.pack(fill=BOTH)
        
        #Canvas
        self.canvas = Canvas(self.cadre, width=WIDTH_F, height=HEIGHT_F, background='white', relief=RAISED)
        self.canvas.pack(side='right')
        
        self.game = Game(hg,wg, init, yinit, xinit, DV = DUREE_VIE)
        self.rect = np.array([[self.canvas.create_rectangle(x * (self.w/self.game.w), y * (self.h/self.game.h), (x+1) * (self.w/self.game.w), (y+1) * (self.h/self.game.h), fill=colors[int(self.game.tab[self.game.ind_tab][y,x])]) for x in range(self.game.w)] for y in range(self.game.h)])
        
        #Boutonreset :
        self.bouton_reset = Button(self.cadre, text="Reset", command = self.reset)#Attenetion, le bouton interromp seulement le mainloop() et non l'interface
        self.bouton_reset.pack(side = 'bottom')

        #Bouton_démarrer_simu :
        self.bouton_play = Button(self.cadre, text="Démarrer simulation", command = self.play)#Attenetion, le bouton interromp seulement le mainloop() et non l'interface
        self.play_int = 0
        self.bouton_play.pack(side = 'bottom')

        #Choix du mode :
        self.mode_int = IntVar()
        self.bouton_modes = [Radiobutton(self.cadre, text="Jeu de la vie", variable=self.mode_int, value=0,command = self.change_mode), Radiobutton(self.cadre, text="Récif coralien 1", variable=self.mode_int, value=1,command = self.change_mode) , Radiobutton(self.cadre, text="Récif coralien 2", variable=self.mode_int, value=2,command = self.change_mode)]
        for i in range(len(self.bouton_modes)):
            self.bouton_modes[i].pack(side='top')
        self.bouton_modes[0].select()

        #scrollbar durée vie
        self.label_duree_vie = Label(self.cadre, text="-----------------\nAppuyer sur Enter\n pour avancer pas à pas\n-----------------\nDurée de vie des coraux :")
        self.label_duree_vie.pack(side='top')
        self.duree_vie = IntVar()
        self.scroll_duree_vie = Scale(self.cadre, variable=self.duree_vie, resolution = 1, to = 99, command = self.change_duree_vie)
        self.scroll_duree_vie.set(DUREE_VIE)
        self.scroll_duree_vie.pack(side='top')        
        
        #scrollbar delay_simu
        self.label_delay_simu = Label(self.cadre, text="---------------\nDélai entre deux pas\nde simulation :")
        self.label_delay_simu.pack(side='top')
        self.delay_simu = IntVar()
        self.scroll_delay_simu = Scale(self.cadre, variable=self.delay_simu, length = 1000, resolution = 10, to = 2000, command = self.change_delay_simu)
        self.scroll_delay_simu.set(DELAY)
        self.scroll_delay_simu.pack(side='top')      
        
        self.mode = (self.game).maj_jdlv
        
        self.focus_set()
        self.bind("<Return>", self.key_fun)
        self.bind("<KeyRelease>", self.keyRel_fun)
        self.key_press_int = 0
        self.delay_last_maj = time.time()
        self.delay_simu_int = 100
    
    def key_fun(self, event):
        if self.key_press_int == 0:
            self.key_press_int = 1
            if time.time() - self.delay_last_maj > (self.delay_simu_int+1)/1000:
                self.delay_last_maj = time.time()
                self.maj()
            
    def keyRel_fun(self, event):
        if self.key_press_int != 0:
            self.key_press_int = 0
            
    def click(self,event):
        x = event.x - self.canvas.winfo_rootx()
        y = event.y - self.canvas.winfo_rooty()
        if x >= 0 and x < self.canvas.winfo_width():
            if y >= 0 and y < self.canvas.winfo_height():
                yc = int(y // 10)
                xc = int(x // 10)
                if 0 <= xc < self.game.w:
                    if 0 <= yc < self.game.h:
                        self.game.tab[self.game.ind_tab][yc,xc] = max(0, 1-self.game.tab[self.game.ind_tab][yc,xc])
                        self.game.change.append((yc,xc, max(0, 1-self.game.tab[self.game.ind_tab][yc,xc])))
                        print((xc,yc))
                        self.maj_screen()
        
    def change_mode(self):
        if (self.mode_int).get() == 0:
            self.mode = (self.game).maj_jdlv
        elif  (self.mode_int).get() == 1:
            self.mode = (self.game).maj_corail1
            self.scroll_duree_vie.set(DUREE_VIE)
            self.game.duree_vie = DUREE_VIE
        elif  (self.mode_int).get() == 2:
            self.mode = (self.game).maj_corail2
            self.scroll_duree_vie.set(DUREE_VIE_2)
            
            self.game.duree_vie = DUREE_VIE_2
    
    def change_duree_vie(self, v):
        self.game.duree_vie = self.scroll_duree_vie.get()
        self.game.change_col = []
        self.game.change = [(y,x,self.game.tab[self.game.ind_tab][y,x]) for x in range(1,self.game.w-1) for y in range(1,self.game.h-1)]
    def change_delay_simu(self, v):
        self.delay_simu_int = self.delay_simu.get()
    
    def reset(self):
        self.game = Game(self.game.h,self.game.w, self.game.init, self.game.yinit, self.game.xinit, DV = self.scroll_duree_vie.get())
        for x in range(self.game.w):
             for y in range(self.game.h):
                self.canvas.delete(self.rect[y,x])
                self.rect[y,x] = self.canvas.create_rectangle(x * (self.w/self.game.w), y * (self.h/self.game.h), (x+1) * (self.w/self.game.w), (y+1) * (self.h/self.game.h), fill=colors[int(self.game.tab[self.game.ind_tab][y,x])])
        self.change_mode()
        
    def boucle(self):
        if self.play_int== 1 :
            if time.time() - self.delay_last_maj > (self.delay_simu_int+1)/1000:
                self.delay_last_maj = time.time()
                self.maj()
            #int(max(0, self.delay_simu_int/1000- (time.time() - self.delay_last_maj)
            d = int( self.delay_simu_int - 1000*(time.time() - self.delay_last_maj) )+5
            self.after( max(5,d) ,self.boucle)
    def play(self):
        if self.play_int== 0 :
            self.play_int = 1
            self.bouton_play["text"] = "Arrêter simulation"
            self.boucle()
        else :
            self.play_int = 0
            self.bouton_play["text"] = "Démarrer simulation"

    def maj(self):
        self.mode()
        self.maj_screen()
        
    def maj_screen(self):
        """ met à jour l'écran avec le nouveau game """
        #self.canvas.create_rectangle(0,2,3)# x1 y1 x2 y2
        for (y,x,c) in self.game.change:
            self.canvas.delete(self.rect[y,x])
            self.rect[y,x] = self.canvas.create_rectangle(x * (self.w/self.game.w), y * (self.h/self.game.h), (x+1) * (self.w/self.game.w), (y+1) * (self.h/self.game.h), fill=colors[int(c)])
        for (y,x,c) in self.game.change_col:
            self.canvas.delete(self.rect[y,x])
            self.rect[y,x] = self.canvas.create_rectangle(x * (self.w/self.game.w), y * (self.h/self.game.h), (x+1) * (self.w/self.game.w), (y+1) * (self.h/self.game.h), fill=colors[int(c)])
        self.canvas.pack()
        
# # # # # # # # # # # # # # # # # # # # # # # # # # # # 
class Game():
    """ contient le tab d'une partie d'automate cellulaire """
    def __init__(self, h=HEIGHT_F//10, w=WIDTH_F//10, init=2, yinit = HEIGHT_F//40, xinit = WIDTH_F//40, DV = 42):
        self.init = init
        self.xinit = xinit
        self.yinit = yinit
        self.nb_etapes = 0
        self.h = h
        self.w = w
        self.tab = np.zeros((2,self.h,self.w))
        if init != 0:
            self.tab[0] = np.zeros((self.h,self.w))
            for y in range(yinit, self.h-1-yinit):
                for x in range(xinit, self.w-1-xinit):
                    self.tab[0][y,x] = np.random.randint(init)

        self.tab[1] = np.zeros((self.h,self.w))
        self.ind_tab = 0
        self.duree_vie = DV
        
        self.change_col = []
        self.change = [(y,x,self.tab[self.ind_tab][y,x]) for x in range(1,self.w-1) for y in range(1,self.h-1)]
        self.check = {(y,x) for x in range(1,self.w-1) for y in range(1,self.h-1)}
    def maj_jdlv(self):
        """ met à jour tab et ind_tab selon le principe des règles du jeu de la vie. On ajoute la règle suivante pour gérer les bords :  Les bords ne peuvent pas contenir de cellules"""
        self.change = []
        self.change_col = []
        self.nb_etapes += 1
        i = self.ind_tab
        j = 1-i
        for (y,x) in self.check:
            s = self.nb_voisins(y,x)
            if self.tab[i][y,x] != 0:
                if s <= 1 or s > 3:
                    self.tab[j][y,x] = 0
                    self.change.append((y,x,0))
                else :
                    self.tab[j][y,x] = 1
            else :
                if s == 3:
                    self.tab[j][y,x] = 1
                    self.change.append((y,x,1))
                else :
                    self.tab[j][y,x] = 0
        self.ind_tab = j
        self.case_to_check()
    
    def maj_corail1(self):
        """ met à jour tab et ind_tab selon le principe des règles du corail. On ajoute la règle suivante pour gérer les bords :  Les bords ne peuvent pas contenir de cellules"""
        self.change = []
        self.change_col = []
        self.nb_etapes += 1
        i = self.ind_tab
        j = 1-i
        for (y,x) in self.check:

            if self.duree_vie > self.tab[i][y,x] >= 1:
                s = self.nb_voisins_plage(y,x,1,self.duree_vie)
                if s <= 3 :
                    self.tab[j][y,x] = 0
                    self.change.append((y,x,101))#ivory
                else :
                    self.tab[j][y,x] += 1
                    self.change_col.append((y,x,(self.tab[j][y,x]*10)//self.duree_vie))
            elif self.tab[i][y,x] == 0:
                s = self.nb_voisins_plage(y,x,1,self.duree_vie)
                if s == 3:
                    self.tab[j][y,x] = 1
                    self.change.append((y,x,1))
                else :
                    self.tab[j][y,x] = 0
            elif self.tab[i][y,x] == self.duree_vie :
                self.tab[j][y,x] += 1
                self.change.append((y,x,100))
            else :
                self.tab[j][y,x] = self.tab[i][y,x]
        self.ind_tab = j
        self.case_to_check()
        
    def maj_corail2(self):
        """ met à jour tab et ind_tab selon le principe des règles du corail. On ajoute la règle suivante pour gérer les bords :  Les bords ne peuvent pas contenir de cellules"""
        self.change = []
        self.change_col = []
        self.nb_etapes += 1
        i = self.ind_tab
        j = 1-i
        for (y,x) in self.check:
            if self.duree_vie > self.tab[i][y,x] >= 1:
                s = self.nb_voisins(y,x)
                if s <= 3 :
                    self.tab[j][y,x] = 0
                    self.change.append((y,x,101))#sea green
                else :
                    self.tab[j][y,x] += 1
                    self.change_col.append((y,x,1))
            elif self.tab[i][y,x] == 0:
                s = self.nb_voisins(y,x)
                if s == 3:
                    self.tab[j][y,x] = 1
                    self.change.append((y,x,1))
                else :
                    self.tab[j][y,x] = 0
            elif self.tab[i][y,x] == self.duree_vie :
                self.tab[j][y,x] += 1
                self.change_col.append((y,x,5))
            else :
                self.tab[j][y,x] = self.tab[i][y,x]            
        self.ind_tab = j
        self.case_to_check()
    
    def voisins(self, y, x):
        """ renvoie la somme des valeurs des voisins de la case (y,x) """
        return self.tab[self.ind_tab][y-1,x-1] + self.tab[self.ind_tab][y-1,x] + self.tab[self.ind_tab][y-1,x+1] + self.tab[self.ind_tab][y,x-1] + self.tab[self.ind_tab][y,x+1] + self.tab[self.ind_tab][y+1,x-1] + self.tab[self.ind_tab][y+1,x] + self.tab[self.ind_tab][y+1,x+1]
    
    def nb_voisins(self, y, x, exclus=0):
        """ renvoie le nb de voisins de la case (y,x) différent de eclus"""
        s = 0
        for c in [self.tab[self.ind_tab][y-1,x-1], self.tab[self.ind_tab][y-1,x], self.tab[self.ind_tab][y-1,x+1], self.tab[self.ind_tab][y,x-1], self.tab[self.ind_tab][y,x+1], self.tab[self.ind_tab][y+1,x-1], self.tab[self.ind_tab][y+1,x], self.tab[self.ind_tab][y+1,x+1]]:
            if exclus != c:
                s+=1 
        return s
    
    def nb_voisins_plage(self, y, x, mini = 1, seuil=1):
        """ renvoie la somme des valeurs des voisins de la case (y,x) """
        s = 0
        for c in [self.tab[self.ind_tab][y-1,x-1], self.tab[self.ind_tab][y-1,x], self.tab[self.ind_tab][y-1,x+1], self.tab[self.ind_tab][y,x-1], self.tab[self.ind_tab][y,x+1], self.tab[self.ind_tab][y+1,x-1], self.tab[self.ind_tab][y+1,x], self.tab[self.ind_tab][y+1,x+1]]:
            if seuil >= c >= mini:
                s+=1
        return s
    
    def case_to_check(self):
        """ met à jour check (ensemble contenant les cases susceptibles de changer) en fonction de change"""
        self.check = set()
        for (y,x,c) in self.change:
            if y < self.h-2 and y > 1 and x < self.w-2 and x > 1:
                for i in range(-1,2):
                    for j in range(-1,2):
                        self.check.add((y+i,x+j))
            else :
                for i in range(-1,2):
                    for j in range(-1,2):
                        if y+i < self.h-1 and y+i > 0 and x+j < self.w-1 and x+j > 0:
                            self.check.add((y+i,x+j))
        for (y,x,c) in self.change_col:
            self.check.add((y,x))

def main():
    fenetre = Tk()
    interface = Interface(fenetre, init = 2)
    interface.maj_screen()
    interface.mainloop()

main()

# # # # # # # # # # # INFO SUR LES DIMENSIONS DES WIDGETS
# height             : Hauteur du widget.
# width              : Largeur du widget.
# padx, pady         : Espace supplémentaire autour du widget. X pour horizontal et V pour vertical.
# borderwidth        : Taille de la bordure.
# highlightthickness : Largeur du rectangle lorsque le widget a le focus.
# selectborderwidth  : Largeur de la bordure tridimensionnel autour du widget sélectionné.
# wraplength         : Nombre de ligne maximum pour les widget en mode "word wrapp

# # # # # # # # # # # # METHODES GRAPHIQUES POUR LE CANVAS
# create_arc()        :  arc de cercle
# create_bitmap()     :  bitmap
# create_image()      :  image
# create_line()       :  ligne
# create_oval()       :  ovale
# create_polygon()    :  polygone
# create_rectangle()  :  rectangle 
# create_text()       :  texte
# create_window()     :  fenetre
#canvas.delete(élément)
#canvas.coords(élément, x0, y0, x1, y1)