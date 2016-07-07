#ifndef _AES_ECB_H_
#define _AES_ECB_H_


void Aes_Encrypt(unsigned char *inData, 
                 unsigned char* aesKey, 
                 unsigned char* outData);
                 

void Aes_Decrypt(unsigned char *inData, 
                 unsigned char* aesKey, 
                 unsigned char* outData);
#endif
