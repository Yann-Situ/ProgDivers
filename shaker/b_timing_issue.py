
h,m = map(int,input().split(":"))
M = int(input())
H = int(input())


nb_minutes_locale = m + H*h
nb_minutes_terre = nb_minutes_locale/M

heure_terre = int(nb_minutes_terre//60)
minute_terre = int(nb_minutes_terre%60)

print(heure_terre%24,minute_terre,sep=":")

