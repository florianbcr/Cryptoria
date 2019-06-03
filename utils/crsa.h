typedef struct RSA RSA;
RSA newkey(int size);
RSA setRSAFromKey(char* privateKeyBase64, char* publicKeyBase64);
void getPublicKey(RSA rsa, char* publicKey);
void getPrivateKey(RSA rsa, char* privateKey);