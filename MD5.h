#pragma once
#include <stdio.h>
#ifndef MD5_H  
#define MD5_H  

typedef struct MD5_CTX
{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];
}MD5_CTX;


#define F(x,y,z) ((x & y) | (~x & z))  
#define G(x,y,z) ((x & z) | (y & ~z))  
#define H(x,y,z) (x^y^z)  
#define I(x,y,z) (y ^ (x | ~z))  
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))  
#define FF(a,b,c,d,x,s,ac)  do {  \
		                        a += F(b, c, d) + x + ac; \
		                        a = ROTATE_LEFT(a, s); \
		                        a += b; \
                               } while (0)
#define GG(a,b,c,d,x,s,ac) do { \
			                    a += G(b, c, d) + x + ac; \
			                    a = ROTATE_LEFT(a, s); \
			                    a += b; \
			                   } while (0)
#define HH(a,b,c,d,x,s,ac) do {  \
				a += H(b, c, d) + x + ac; \
				a = ROTATE_LEFT(a, s); \
				a += b; \
			    } while (0)
#define II(a,b,c,d,x,s,ac) do {   \
				               a += I(b, c, d) + x + ac; \
				               a = ROTATE_LEFT(a, s); \
				               a += b; \
			                  } while (0)




void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen);
void MD5Final(MD5_CTX *context, unsigned char digest[16]);
void MD5Transform(unsigned int state[4], unsigned char block[64]);
void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len);
void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len);
char *MD5(char * str, char *decrypt);

#endif  
