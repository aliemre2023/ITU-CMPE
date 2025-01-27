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
# Save, Read

def read_encrypted():
    encrypted = []
    file = open("network/message_encrypted.txt", "r")
    for chunk in file.readlines():
        encrypted.append(int(chunk))
        
    file.close()
    return encrypted

def read_skeys():
    skeys_f = open("alice/box/keys_all.txt")
    skeys = []
    for key in skeys_f.readlines():
        skeys.append(key)

    skeys_f.close()
    return skeys

def save_decrypted(decrypted_message):
    file = open("alice/box/message_decrypted.txt", "w")
    file.write(decrypted_message)
    file.close()

#
# Main

if __name__ == "__main__":
    skeys = read_skeys()

    n = skeys[2]
    d = skeys[5]
    n = int(n)
    d = int(d)

    encrypted = read_encrypted()

    decrypted_message = decrypt_message(encrypted, d, n)
    save_decrypted(decrypted_message)
