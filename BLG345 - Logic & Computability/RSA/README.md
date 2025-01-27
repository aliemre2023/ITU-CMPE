This is a fundamental implementation of the RSA algorithm, along with a brute-force approach for deciphering the encrypted message.

RSA (Rivest–Shamir–Adleman) is a cryptographic method based on the principle of trapdoor one-way functions (TDOWF). 
In this system, Alice shares her public key with Bob, allowing him to encrypt a message before sending it back to her. 
However, a malicious entity like Eve could attempt to intercept and decipher the message. 
To break the encryption, Eve might employ algorithms such as Shor’s algorithm or brute-force methods, aiming to compromise the security of the communication.

<div align="center">
<p>eavesdropper's Estimated Brute-Force Decipher Time</p>
  
| N, prime limit | time (n^2) |
|----------|----------|
| 100000 | 3s |
| 1000000 | 279s |
| 10000000 | 30000s ? |

</div>



<p align="center">
  <img width="415" alt="Screen Shot 2025-01-27 at 14 23 53" src="https://github.com/user-attachments/assets/dd1cc403-c308-4e60-8afc-93d04c54a2e4" />
</p>
