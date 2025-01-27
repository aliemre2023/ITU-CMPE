import random

# RSA (https://en.wikipedia.org/wiki/RSA_(cryptosystem))

#
# Sieve Algorithm
def SieveOfEratosthenes(n):
    prime = [True] * (n + 1)
    prime[0] = prime[1] = False
    
    p = 2
    while(p*p <= n):
        if(prime[p] == True):
            for i in range(p*p, n+1, p):
                prime[i] = False
        p += 1
    
    just_prime = []
    for i in range(len(prime)):
        if(prime[i] == True):
            just_prime.append(i)

    return just_prime
 
  
#
# Encryption Operations

def random_2_prime(prime):
    random_idx1 = random.randint(0, len(prime))

    num1 = prime[random_idx1]

    '''
    num2 = num1
    # close numbers, but not too close
    while(abs(num2 - num1) < 10 or abs(num2 - num1) > 1000):
        random_idx2 = random.randint(0, len(prime))
        num2 = prime[random_idx2]

    '''
    # normal numbers, being close no matter
    random_idx2 = random.randint(0, len(prime))

    num2 = prime[random_idx2]

    return num1, num2

def totient(num1, num2):
    return (num1-1) * (num2-1)
    
def __gcd(a, b):
    '''
    if (a == 0 or b == 0): return 0
    
    if (a == b): return a
    
    if (a > b): 
        return __gcd(a - b, b)
            
    return __gcd(a, b - a)
    '''
    while b != 0:
        a, b = b, a % b 
    return a  

def totient_coprime(T):
    coprime = random.randint(2, T)
    while(__gcd(T, coprime) != 1):
        coprime = random.randint(2, T)
    
    return coprime

def extended_gcd(a, b):  
    if b == 0:
        return a, 1, 0
    gcd, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return gcd, x, y

def calculate_d(T, coprime):
    gcd, x, _ = extended_gcd(coprime, T)
    d = (x + T) % T
    return d

#
# Save Send Keys

def send_pkeys(n, e):
    file = open(f"network/keys_public.txt", "w")
    file.write(f"{n}\n{e}")
    file.close()

def save_keys(p, q, n, t, e ,d):
    file = open("alice/box/keys_all.txt", "w")
    file.write(f"{p}\n{q}\n{n}\n{t}\n{e}\n{d}")
    file.close()

#
# Main

if __name__ == "__main__":
    # Primes
    N = 1000000
    prime = SieveOfEratosthenes(N)

    # Keys
    p, q = random_2_prime(prime)
    n = p*q
    t = totient(p, q)
    e = totient_coprime(t)
    d = calculate_d(t, e)

    #print(f"Secret Keys: {n} {d}")
    #print(f"Public Keys: {n} {e}")

    save_keys(p, q, n, t, e, d)
    send_pkeys(n, e)