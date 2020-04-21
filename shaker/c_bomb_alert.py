
xb,yb = map(float,input().split())
N,T = map(int,input().split())

def dist(pos1,pos2):
    (x1,y1) = pos1
    (x2,y2) = pos2
    return ((x1-x2)**2+(y1-y2)**2)**0.5

def is_possible(pos1,pos2):
    return dist(pos1,(xb,yb))+dist(pos2,(xb,yb)) <= 100

def check(moves):
    for i in range(len(moves)-1):
        if is_possible(moves[i],moves[i+1]):
            return True
    return False
        
suspects = []
for i in range(N):
    name = input()
    moves = []
    for i in range(T):
        a,b = map(float,input().split())
        moves.append((a,b))
    if check(moves):
        suspects.append(name)
print(" ".join(suspects))


