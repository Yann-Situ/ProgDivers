

Q = int(input())
N = int(input())

c1 = []
c2 = []

for _ in range(N):
    l = [int(val) for val in input().split()]
    if l[0] == 1 and l[1] <= Q:
        c1.append(l[1])
    if l[0] == 2 and l[1] <= Q:
        c2.append(l[1])

dico = [ [ [-1 for _ in range(len(c2))] for _ in range(len(c1))] for _ in range(Q+1)]

def val_poss(val_max, i, j):
    if i == len(c1) or j == len(c2):
        return 0
    if val_max < 0:
        return 0
    if dico[val_max][i][j] == -1 :
        t1 = val_poss(val_max, i+1, j)
        t2 = val_poss(val_max, i, j+1)
        t3 = 0
        if c1[i] + c2[j] <= val_max:
            t3 = c1[i] + c2[j] + val_poss(val_max - c1[i] - c2[j], i+1, j+1)
        dico[val_max][i][j] = max(t1, t2, t3)
    return dico[val_max][i][j]

print( val_poss(Q, 0, 0) )

