typedef unsigned long uint32_t;
typedef unsigned int  uint16_t;
typedef unsigned char uint8_t;
typedef long int32_t;
typedef int  int16_t;
typedef char int8_t;

#define RC5_32_P	0xB7E15163
#define RC5_32_Q	0x9E3779B9
#define RC5_32_MASK	0xffffffff
#define RC5_ROUNDS 12
#define rotl32(a,b) fastrol32((a), (b))
#define rotr32(a,b) fastror32((a), (b))
#define SUCCESS 0
#define KEYS_ADDRESS (char *)0x1000
#define KEYTABLE_SIZE 2 * (RC5_ROUNDS +1)

#include "rc5-crypto-primitives.h"
#include <msp430.h>

unsigned long S[2*(RC5_ROUNDS+1)];

/**
 * Performs the key expansion on the real secret.
 *
 * @param secret key
 */
int setupKey (uint8_t * key)
{
    // toggle bit CCIS0 to switch the capture signal
    //TACCTL0 = CCIS0 + TACCTL0;
    //asm("XOR #CCIS0, &TACCTL0");
    //timer_start = TAR;

    uint32_t *L,l,A,B,k,*s;
    uint8_t ii,jj, m;
    int8_t i;
    uint8_t tmp[12];

    c2l(key,l);
    L = (uint32_t *) tmp;
    L[0]=l;
    key += 4;
    c2l(key,l);
    L[1]=l;
    // what about rest of bytes?
    key += 4;
    c2l(key,l);
    L[2]=l;
    key += 4;
    c2l(key,l);
    L[3]=l;

    *S = RC5_32_P;

    for (i=1; i< 2 * RC5_ROUNDS + 2; i++) {
        *(S + i) = *(S + (i-1)) + RC5_32_Q;
    }

    int offset =0;
    ii=jj=0;
    A=B=0;

    for (i=3*(2*RC5_ROUNDS + 2) - 1; i>=0; i--) {

        k=(*(S+offset)+A+B)&RC5_32_MASK;
        rotl32((k), (3));
        A=*(S+offset) = k;

        offset++;
        m=((char)(A+B)) & 0x1f;

        k=(*L+A+B)&RC5_32_MASK;
        rotl32((k), (m));
        B=*L= k;

        if (++ii >= 2*RC5_ROUNDS+2) {ii=0; offset = 0;}
        // Wny ^=4?????? (bitwise XOR)
        //jj ^= 4;
        if ( (jj += 4) >= 16) {jj=0; }
        L = (uint32_t *) (&tmp[jj]);
    }

    // toggle bit CCIS0 to switch the capture signal
    //TACCTL0 = CCIS0 + TACCTL0;
    //asm("XOR #CCIS0, &TACCTL0");
    //timer_end = TAR;
    return SUCCESS;
}

/**
 * Encrypts a single block (of blockSize) using the key in the keySize.
 *
 * PROLOGUE: 24 cycles
 * INIT:     48 cycles
 * LOOP:   1680 cycles (12 + fastrol [= 42] + 16) * 2 * RC5_ROUNDS
 * CLOSE:    24 cycles
 * =====================
 *         1776 cycles (avg case)
 *
 * @param plainBlock a plaintext block of blockSize
 * @param cipherBlock the resulting ciphertext block of blockSize
 *
 * @return Whether the encryption was successful. Possible failure reasons
 *         include not calling init().
 */
int encrypt(uint8_t * block, uint8_t * cipherBlock)
{
    register uint32_t l=0, r=0;
    uint8_t i, tmp;
    int offset =0;

    c2l(block, l);
    block += 4;
    c2l(block, r);

    l += S[offset++];
    r += S[offset++];

    for (i = RC5_ROUNDS; i > 0; i--) {
        l ^= r;   tmp = r; tmp &= 0x1f; rotl32(l, tmp);
        l += *(S + offset++);
        r ^= l;   tmp = l; tmp &= 0x1f; rotl32(r, tmp);
        r += *(S + offset++);
    }

    l2c(l, cipherBlock);
    cipherBlock += 4;
    l2c(r, cipherBlock);

    return SUCCESS;
}

/**
 * Decrypts a single block (of blockSize) using the key in the keySize. Not
 * all ciphers will implement this function (since providing encryption
 * is a useful primitive).
 *
 * @param cipherBlock a ciphertext block of blockSize
 * @param plainBlock the resulting plaintext block of blockSize
 *
 * @return Whether the decryption was successful. Possible failure reasons
 *         include not calling init() or an unimplimented decrypt function.
 */
int decrypt(uint8_t * cipherBlock, uint8_t * plainBlock)
{
    register uint32_t l;
    register uint32_t r;
    uint8_t i, tmp;
    int offset = KEYTABLE_SIZE-1;

    c2l(cipherBlock, l);
    cipherBlock += 4;
    c2l(cipherBlock, r);
    for (i = RC5_ROUNDS; i> 0; i--) {
        r -= *(S + offset--) ;
        tmp = l;  tmp &= 0x1f; rotr32(r, tmp);   r ^= l;

        l -= *(S + offset--);
        tmp = r;  tmp &= 0x1f; rotr32(l, tmp);   l ^= r;
    }

    r -= *(S + offset--);
    l -= *(S + offset);

    l2c(l, plainBlock);
    plainBlock += 4;
    l2c(r, plainBlock);
    return SUCCESS;
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stop WDT

    uint8_t cipher[8];      // ciphertext buffer
    uint8_t key[] = {0x52, 0x69, 0xF1, 0x49, 0xD4, 0x1B, 0xA0, 0x15, 0x24,
        0x97, 0x57, 0x4D, 0x7F, 0x15, 0x31, 0x25};
    uint8_t plain[] = {0x65, 0xC1, 0x78, 0xB2, 0x84, 0xD1, 0x97, 0xCC};
    uint8_t decryptedResult[8];
    setupKey(key);
    encrypt(plain, cipher);
    decrypt(cipher, decryptedResult);
}
