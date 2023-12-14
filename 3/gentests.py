import sys
from random import randint, choice

TESTS = 1000
MAX_T = 10

queries = [line.split() for line in sys.stdin if line.strip()]
queries = list(filter(lambda x: int(x[0]) <= 100 and int(x[1]) <= 100, queries))

for i in range(TESTS):
    tf = open(f"tests/tr_{i}", "w+")
    ta = open(f"tests/ans_tr_{i}", "w+")
    t = randint(1, MAX_T)
    tf.write(f'{t}\n')
    for _ in range(t):
        q = choice(queries)
        tf.write(f'{q[0]} {q[1]}\n')
        ta.write(f'{q[2]}\n')
    
    tf.close()
    ta.close()

