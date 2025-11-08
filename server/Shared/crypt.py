from bcrypt import hashpw, gensalt
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from django.conf import settings
from hashlib import sha3_512
import hmac

def encrypt_text(text: str, aes_key: bytes) -> tuple[bytes, bytes]:
    
    iv = getattr(settings, 'AES_256_IV', None)
    cipher = Cipher(algorithms.AES(aes_key), modes.GCM(iv))
    encryptor = cipher.encryptor()
    
    # Encrypt the plaintext directly without padding
    cyphered_text = encryptor.update(text.encode()) + encryptor.finalize()
    
    # Return ciphertext and GCM authentication tag
    return cyphered_text, encryptor.tag

def decrypt_text(cyphered_block: bytes, aes_key: bytes, cypher_tag: bytes) -> str:
    iv = getattr(settings, 'AES_256_IV', None)
    cipher = Cipher(algorithms.AES(aes_key), modes.GCM(iv, cypher_tag))
    decryptor = cipher.decryptor()
    
    # Decrypt directly without unpadding
    plaintext = decryptor.update(cyphered_block)
    return plaintext.decode(errors='ignore')

def digest_text(text: str) -> bytes:
    secret_key = settings.HMAC_BASE_KEY.encode("utf-8")  # Use Django's SECRET_KEY
    hash_input = text.encode("utf-8")  # Convert data to bytes
    return bytes.fromhex(hmac.new(secret_key, hash_input, sha3_512).hexdigest())

def gen_password(text: str, rounds=12) -> bytes:
    salt = gensalt(rounds)
    btext = text.encode('utf-8')
    return hashpw(btext, salt)
