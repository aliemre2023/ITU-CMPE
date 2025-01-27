#
# Chunk operations

def message_to_chunks(message, n):
    max_bytes = (n.bit_length() - 16) // 8
    chunks = []

    for i in range(0, len(message), max_bytes):
        chunk = message[i:i + max_bytes]
        chunk_val = int.from_bytes(chunk.encode(), "big")
        chunks.append(chunk_val)
    
    return chunks

def encrypt_message(message, e, n):
    chunks = message_to_chunks(message, n)
    encrypted_chunks = []
    for chunk in chunks:
        encrypted_chunks.append(pow(chunk, e, n))

    return encrypted_chunks

#
# Save, Read
def read_pkeys():
    pkeys_f = open("network/keys_public.txt")

    pkeys = []
    for key in pkeys_f.readlines():
        pkeys.append(int(key))
    
    pkeys_f.close()
    # n, e respectively
    return pkeys


def save_encrypted(encrypted):
    file = open(f"network/message_encrypted.txt", "w")
    for chunk in encrypted:
        file.write(str(chunk) + "\n")
    file.close()

#
# Main

if __name__ == "__main__":
    message_f = open("bob/box/message.txt", "r")
    message = message_f.read()

    pkeys = read_pkeys()
    n = pkeys[0]
    e = pkeys[1]
    n = int(n)
    e = int(e)

    encrypted = encrypt_message(message, e, n)
    save_encrypted(encrypted)

