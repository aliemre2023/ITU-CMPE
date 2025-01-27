import random
import time

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
# Read, Save

def read_pkeys():
    pkeys_f = open("network/keys_public.txt")

    pkeys = []
    for key in pkeys_f.readlines():
        pkeys.append(int(key))
    
    pkeys_f.close()
    # n, e respectively
    return pkeys

def read_encrypted():
    encrypted = []
    file = open("network/message_encrypted.txt", "r")
    for chunk in file.readlines():
        encrypted.append(int(chunk))
        
    file.close()
    return encrypted

def save_decrypted(decrypted_message, n, d):
    file = open(f"eve/bruteForce/messages_possible/{n}_{d}.txt", "w")
    file.write(decrypted_message)
    file.close()

#
# Chunk operations

def chunks_to_message(chunks):
    message_bytes = b""

    for chunk in chunks:
        chunk_bytes = chunk.to_bytes((chunk.bit_length() + 7) // 8, "big")
        message_bytes += chunk_bytes
    
    return message_bytes.decode("utf-8", errors="ignore")

def decrypt_message(encrypted_chunks, d, n):
    decrypted_chunks = []
    for chunk in encrypted_chunks:
        decrypted_chunks.append(pow(chunk, d, n))

    return chunks_to_message(decrypted_chunks)

#
# Main

if __name__ == "__main__":
    start_time = time.time()

    N = 1000000
    prime = SieveOfEratosthenes(N)

    pkeys = read_pkeys()
    real_n = pkeys[0]
    real_e = pkeys[1]
    real_n = int(real_n)
    real_e = int(real_e)

    encrypted = read_encrypted()
    possible_keys = open("eve/bruteForce/keys_possible.txt", "a")

    for i in range(len(prime)):
        for ii in range(i, len(prime)):
            
            num1 = prime[i]
            num2 = prime[ii]

            '''
            if(abs(num2 - num1) < 10):
                continue
            if(abs(num2 - num1) > 1000):
                break
            '''
        
            p = num1
            q = num2
            n = p*q
            if(n != real_n):
                continue
            print(f"N: {n} -- Time: {time.time() - start_time}")
            print()

            t = totient(p, q) 

            d = calculate_d(t, real_e)

            possible_keys.write(f"{p} {q} {n} {t} {real_e} {d}\n")

            decrypted_message = decrypt_message(encrypted, d, n)
            save_decrypted(decrypted_message, n, d)

    possible_keys.close()

    end_time = time.time()
    elapsed_time = end_time - start_time
    print("Elapsed Time: ", elapsed_time)
            


                
                