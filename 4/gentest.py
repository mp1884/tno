from random import randint, seed

MAX_N = 20
MAX_XS = 1000
MAX_HS = 10

n = randint(1, MAX_N)
xs = [randint(1, MAX_XS) for _ in range(n)]
hs = [randint(1, MAX_HS) for _ in range(n)]
xs.sort()

print(n)
print(*xs, sep=' ')
print(*hs, sep=' ')

