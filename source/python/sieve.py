from math import *

def sieve(N):
    primes = set()
    for i in range(2,N):
        primes.add(i)

    for i in range(2,ceil(sqrt(N))):
        if i in primes:
            for j in range(i*i,N,i):
                primes.discard(j)

    return primes
