#ifdef WIN32   // This command checks if the OS is Windows or not. Since some commands differ for Visual Studio 
typedef unsigned __int64 bit64;
typedef unsigned long bit32;
typedef unsigned char bit8;
#else
#include <stdint.h>
#define scanf_s(...) {scanf(__VA_ARGS__);}
typedef uint64_t bit64;
typedef uint32_t bit32;
typedef uint8_t bit8;
#endif
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "small_aes.h"
#define datasize 0x800000
#define data 0x100000000
bit64 integral = 0;
#define ROTR(x,n) (((x)>>(n))|((x)<<(64-(n))))
#define ROTL(x,n) (((x)<<(n))|((x)>>(64-(n))))
#define ROTL16(x,n) (((x)<<(n))|((x)>>(16-(n))))&0xffff
bit64 S[16] = {0x6, 0xb, 0x5, 0x4, 0x2, 0xe, 0x7, 0xa, 0x9, 0xd, 0xf, 0xc, 0x3, 0x1, 0x0, 0x8};
bit64 Sinv[16] = { 0xe, 0xd, 0x4, 0xc, 0x3, 0x2, 0x0, 0x6, 0xf, 0x8, 0x7, 0x1, 0xb, 0x9, 0x5, 0xa };
bit32 Rcon[16] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x13, 0x25, 0x47, 0x89, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a };
bit64 rkey[20] = { 0 };
bit64 antidiagonal1[65536] = { 0 };
bit64 antidiagonal2[65536] = { 0 };
bit64 antidiagonal3[65536] = { 0 };
bit64 antidiagonal4[65536] = { 0 };
bit64 hits[4] = { 0 };
void printAntiDiagonalValues() {
#ifdef WIN32
  printf("Anti-diagonal 1: %I64d\n", hits[0]);
  printf("Anti-diagonal 2: %I64d\n", hits[1]);
  printf("Anti-diagonal 3: %I64d\n", hits[2]);
  printf("Anti-diagonal 4: %I64d\n", hits[3]);
#else
  printf("Anti-diagonal 1: %ld\n", hits[0]);
  printf("Anti-diagonal 2: %ld\n", hits[1]);
  printf("Anti-diagonal 3: %ld\n", hits[2]);
  printf("Anti-diagonal 4: %ld\n", hits[3]);
#endif
}
void printDiagonalValues() {
#ifdef WIN32
	printf("Diagonal 1: %I64d\n", hits[0]);
	printf("Diagonal 2: %I64d\n", hits[1]);
	printf("Diagonal 3: %I64d\n", hits[2]);
	printf("Diagonal 4: %I64d\n", hits[3]);
#else
	printf("Diagonal 1: %ld\n", hits[0]);
	printf("Diagonal 2: %ld\n", hits[1]);
	printf("Diagonal 3: %ld\n", hits[2]);
	printf("Diagonal 4: %ld\n", hits[3]);
#endif
}
void key_schedule(bit64 key) {
	bit64 o[4], temp;
	rkey[0] = key;
	for (int i = 1; i < 16; i++) {
		o[0] = (key >> 48) & 0xffff;
		o[1] = (key >> 32) & 0xffff;
		o[2] = (key >> 16) & 0xffff;
		o[3] = (key >> 0) & 0xffff;
		temp = o[3];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[i] & 0xf)) << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[0] ^= temp;
		o[1] ^= o[0];
		o[2] ^= o[1];
		o[3] ^= o[2];
		key = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		rkey[i] = key;
	}
}
void key_schedule_inverse(bit64 key, int round) {
	bit64 o[4], temp;
	rkey[0] = key;
	for (int i = 1; i < round; i++) {
		o[0] = (key >> 48) & 0xffff;
		o[1] = (key >> 32) & 0xffff;
		o[2] = (key >> 16) & 0xffff;
		o[3] = (key >> 0) & 0xffff;
		o[3] ^= o[2];
		o[2] ^= o[1];
		o[1] ^= o[0];
		temp = o[3];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[round-i] & 0xf)) << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[0] ^= temp;
		key = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		rkey[i] = key;
	}
}
void key_schedule128(bit64 key[2]) {
	bit64 o[4], temp;
	rkey[0] = key[0];
	rkey[1] = key[1];
	for (int i = 2; i < 14; i=i+2) {
		o[0] = (key[0] >> 48) & 0xffff;
		o[1] = (key[0] >> 32) & 0xffff;
		o[2] = (key[0] >> 16) & 0xffff;
		o[3] = (key[0] >> 0) & 0xffff;
		o[4] = (key[1] >> 48) & 0xffff;
		o[5] = (key[1] >> 32) & 0xffff;
		o[6] = (key[1] >> 16) & 0xffff;
		o[7] = (key[1] >> 0) & 0xffff;
		temp = o[7];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[i/2] & 0xf)) << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[0] ^= temp;
		o[1] ^= o[0];
		o[2] ^= o[1];
		o[3] ^= o[2];
		temp = o[3];
//		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ (S[(temp >> 12) & 0xf] << 12);
		if (temp>0xffff) printf("ERROR\n");
		o[4] ^= temp;
		o[5] ^= o[4];
		o[6] ^= o[5];
		o[7] ^= o[6];
		key[0] = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		key[1] = (o[4] << 48) ^ (o[5] << 32) ^ (o[6] << 16) ^ (o[7] << 0);
		rkey[i] = key[0];
		rkey[i+1] = key[1];
	}
}
void key_schedule128_inverse(bit64 key[2], int round) {
	bit64 o[4], temp;
	rkey[0] = key[0];
	rkey[1] = key[1];
	for (int i = 2; i < round; i = i + 2) {
		o[0] = (key[0] >> 48) & 0xffff;
		o[1] = (key[0] >> 32) & 0xffff;
		o[2] = (key[0] >> 16) & 0xffff;
		o[3] = (key[0] >> 0) & 0xffff;
		o[4] = (key[1] >> 48) & 0xffff;
		o[5] = (key[1] >> 32) & 0xffff;
		o[6] = (key[1] >> 16) & 0xffff;
		o[7] = (key[1] >> 0) & 0xffff;
		o[7] ^= o[6];
		o[6] ^= o[5];
		o[5] ^= o[4];
		temp = o[3];
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ (S[(temp >> 12) & 0xf] << 12);
		o[4] ^= temp;
		if (temp>0xffff) printf("ERROR\n");
		o[3] ^= o[2];
		o[2] ^= o[1];
		o[1] ^= o[0];
		temp = o[7];
		temp = ROTL16(temp, 4);
		temp = (S[(temp >> 0) & 0xf] << 0) ^ (S[(temp >> 4) & 0xf] << 4) ^ (S[(temp >> 8) & 0xf] << 8) ^ ((S[(temp >> 12) & 0xf] ^ (Rcon[(round-i)/2] & 0xf)) << 12);
		o[0] ^= temp;
		if (temp>0xffff) printf("ERROR\n");
		key[0] = (o[0] << 48) ^ (o[1] << 32) ^ (o[2] << 16) ^ (o[3] << 0);
		key[1] = (o[4] << 48) ^ (o[5] << 32) ^ (o[6] << 16) ^ (o[7] << 0);
		rkey[i] = key[0];
		rkey[i + 1] = key[1];
	}
}
bit64 matrix_multiplication(bit64 input) {
	bit64 i[4], o[4], output;
	i[0] = (input >> 12) & 0xf;
	i[1] = (input >> 8) & 0xf;
	i[2] = (input >> 4) & 0xf;
	i[3] = (input >> 0) & 0xf;
	// first row
	o[0] = i[0] << 1; if (o[0] >> 4) o[0] = (o[0] & 0xf) ^ 0x3;
	o[1] = i[1] << 1; if (o[1] >> 4) o[1] = (o[1] & 0xf) ^ 0x3; o[1] ^= i[1];
	o[2] = i[2];
	o[3] = i[3];
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output = o[0] << 12;
	// second row
	o[0] = i[0];
	o[1] = i[1] << 1; if (o[1] >> 4) o[1] = (o[1] & 0xf) ^ 0x3;
	o[2] = i[2] << 1; if (o[2] >> 4) o[2] = (o[2] & 0xf) ^ 0x3; o[2] ^= i[2];
	o[3] = i[3];
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output ^= (o[0] << 8);
	// third row
	o[0] = i[0];
	o[1] = i[1];
	o[2] = i[2] << 1; if (o[2] >> 4) o[2] = (o[2] & 0xf) ^ 0x3;
	o[3] = i[3] << 1; if (o[3] >> 4) o[3] = (o[3] & 0xf) ^ 0x3; o[3] ^= i[3];
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output ^= (o[0] << 4);
	// fourth row
	o[0] = i[0] << 1; if (o[0] >> 4) o[0] = (o[0] & 0xf) ^ 0x3; o[0] ^= i[0];
	o[1] = i[1];
	o[2] = i[2];
	o[3] = i[3] << 1; if (o[3] >> 4) o[3] = (o[3] & 0xf) ^ 0x3;
	o[0] = o[0] ^ o[1] ^ o[2] ^ o[3];
	if (o[0] > 0xf) printf("Error\n");
	output ^= o[0];
	if (output > 0xffff) printf("Error\n");
	return output;
}
bit64 encrypt(bit64 plaintext, int round) {
	bit64 state=plaintext, temp[4];
	for (int i = 0; i < round-1; i++) {
		// key addition
		state ^= rkey[i];
		// S-box Layer
		state = S[state & 0xf] ^ (S[(state >> 4) & 0xf] << 4) ^ (S[(state >> 8) & 0xf] << 8) ^ (S[(state >> 12) & 0xf] << 12) ^ (S[(state >> 16) & 0xf] << 16) ^ (S[(state >> 20) & 0xf] << 20) ^ (S[(state >> 24) & 0xf] << 24) ^ (S[(state >> 28) & 0xf] << 28) ^ (S[(state >> 32) & 0xf] << 32) ^ (S[(state >> 36) & 0xf] << 36) ^ (S[(state >> 40) & 0xf] << 40) ^ (S[(state >> 44) & 0xf] << 44) ^ (S[(state >> 48) & 0xf] << 48) ^ (S[(state >> 52) & 0xf] << 52) ^ (S[(state >> 56) & 0xf] << 56) ^ (S[(state >> 60) & 0xf] << 60);
		// Shift row
		temp[0] = (state & 0xf000f000f000f000);
		temp[1] = (state & 0x0f000f000f000f00);
		temp[2] = (state & 0x00f000f000f000f0);
		temp[3] = (state & 0x000f000f000f000f);
		temp[1] = ROTL(temp[1], 16);
		temp[2] = ROTL(temp[2], 32);
		temp[3] = ROTL(temp[3], 48);
		state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
		// Matrix multiplication
		temp[0] = (state >> 48) & 0xffff;
		temp[1] = (state >> 32) & 0xffff;
		temp[2] = (state >> 16) & 0xffff;
		temp[3] = (state >> 0) & 0xffff;
		temp[0] = mc[temp[0]];
		temp[1] = mc[temp[1]];
		temp[2] = mc[temp[2]];
		temp[3] = mc[temp[3]];
		state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
	}
	// Last round without Mixcolumn
	// key addition
	state ^= rkey[round-1];
	// S-box Layer
	state = S[state & 0xf] ^ (S[(state >> 4) & 0xf] << 4) ^ (S[(state >> 8) & 0xf] << 8) ^ (S[(state >> 12) & 0xf] << 12) ^ (S[(state >> 16) & 0xf] << 16) ^ (S[(state >> 20) & 0xf] << 20) ^ (S[(state >> 24) & 0xf] << 24) ^ (S[(state >> 28) & 0xf] << 28) ^ (S[(state >> 32) & 0xf] << 32) ^ (S[(state >> 36) & 0xf] << 36) ^ (S[(state >> 40) & 0xf] << 40) ^ (S[(state >> 44) & 0xf] << 44) ^ (S[(state >> 48) & 0xf] << 48) ^ (S[(state >> 52) & 0xf] << 52) ^ (S[(state >> 56) & 0xf] << 56) ^ (S[(state >> 60) & 0xf] << 60);
	// Shift row
	temp[0] = (state & 0xf000f000f000f000);
	temp[1] = (state & 0x0f000f000f000f00);
	temp[2] = (state & 0x00f000f000f000f0);
	temp[3] = (state & 0x000f000f000f000f);
	temp[1] = ROTL(temp[1], 16);
	temp[2] = ROTL(temp[2], 32);
	temp[3] = ROTL(temp[3], 48);
	state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
	return state;
}
bit64 decrypt(bit64 plaintext, int round) {
	bit64 state = plaintext, temp[4];
	// FIRST AND LAST ROUND HAVE MIXCOLUMNS!!!!!!
	// key addition
	state ^= rkey[0];
	// Matrix multiplication
	temp[0] = (state >> 48) & 0xffff;
	temp[1] = (state >> 32) & 0xffff;
	temp[2] = (state >> 16) & 0xffff;
	temp[3] = (state >> 0) & 0xffff;
	temp[0] = imc[temp[0]];
	temp[1] = imc[temp[1]];
	temp[2] = imc[temp[2]];
	temp[3] = imc[temp[3]];
	state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
	// Shift row
	temp[0] = (state & 0xf000f000f000f000);
	temp[1] = (state & 0x0f000f000f000f00);
	temp[2] = (state & 0x00f000f000f000f0);
	temp[3] = (state & 0x000f000f000f000f);
	temp[1] = ROTR(temp[1], 16);
	temp[2] = ROTR(temp[2], 32);
	temp[3] = ROTR(temp[3], 48);
	state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
	// S-box Layer
	state = Sinv[state & 0xf] ^ (Sinv[(state >> 4) & 0xf] << 4) ^ (Sinv[(state >> 8) & 0xf] << 8) ^ (Sinv[(state >> 12) & 0xf] << 12) ^ (Sinv[(state >> 16) & 0xf] << 16) ^ (Sinv[(state >> 20) & 0xf] << 20) ^ (Sinv[(state >> 24) & 0xf] << 24) ^ (Sinv[(state >> 28) & 0xf] << 28) ^ (Sinv[(state >> 32) & 0xf] << 32) ^ (Sinv[(state >> 36) & 0xf] << 36) ^ (Sinv[(state >> 40) & 0xf] << 40) ^ (Sinv[(state >> 44) & 0xf] << 44) ^ (Sinv[(state >> 48) & 0xf] << 48) ^ (Sinv[(state >> 52) & 0xf] << 52) ^ (Sinv[(state >> 56) & 0xf] << 56) ^ (Sinv[(state >> 60) & 0xf] << 60);
	for (int i = 2; i < round; i++) {
		// key addition
		state ^= rkey[i];
		// Matrix multiplication
		temp[0] = (state >> 48) & 0xffff;
		temp[1] = (state >> 32) & 0xffff;
		temp[2] = (state >> 16) & 0xffff;
		temp[3] = (state >> 0) & 0xffff;
		temp[0] = imc[temp[0]];
		temp[1] = imc[temp[1]];
		temp[2] = imc[temp[2]];
		temp[3] = imc[temp[3]];
		state = (temp[0] << 48) ^ (temp[1] << 32) ^ (temp[2] << 16) ^ (temp[3] << 0);
		// Shift row
		temp[0] = (state & 0xf000f000f000f000);
		temp[1] = (state & 0x0f000f000f000f00);
		temp[2] = (state & 0x00f000f000f000f0);
		temp[3] = (state & 0x000f000f000f000f);
		temp[1] = ROTR(temp[1], 16);
		temp[2] = ROTR(temp[2], 32);
		temp[3] = ROTR(temp[3], 48);
		state = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
		// S-box Layer
		state = Sinv[state & 0xf] ^ (Sinv[(state >> 4) & 0xf] << 4) ^ (Sinv[(state >> 8) & 0xf] << 8) ^ (Sinv[(state >> 12) & 0xf] << 12) ^ (Sinv[(state >> 16) & 0xf] << 16) ^ (Sinv[(state >> 20) & 0xf] << 20) ^ (Sinv[(state >> 24) & 0xf] << 24) ^ (Sinv[(state >> 28) & 0xf] << 28) ^ (Sinv[(state >> 32) & 0xf] << 32) ^ (Sinv[(state >> 36) & 0xf] << 36) ^ (Sinv[(state >> 40) & 0xf] << 40) ^ (Sinv[(state >> 44) & 0xf] << 44) ^ (Sinv[(state >> 48) & 0xf] << 48) ^ (Sinv[(state >> 52) & 0xf] << 52) ^ (Sinv[(state >> 56) & 0xf] << 56) ^ (Sinv[(state >> 60) & 0xf] << 60);
	}
	return state;
}
void on_the_fly(bit64 key, int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp[14] = { 0 }, input;
	key_schedule(key);
	for (bit64 i = 0; i <= 0xffffffff; i++) {
//		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = encrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		temp[0] = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		temp[1] = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		temp[2] = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		temp[3] = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal1[temp[0]]++;
		antidiagonal2[temp[1]]++;
		antidiagonal3[temp[2]]++;
		antidiagonal4[temp[3]]++;
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
//	printf("\b\b\b\b\b\b\b\b");
  printAntiDiagonalValues();
//	printf("Integral: %016I64x\n", integral);
}
void on_the_fly_decryption(bit64 key, int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp, input;
	key_schedule_inverse(key,round);
	for (bit64 i = 0; i <= 0xffffffff; i++) {
//		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = decrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		// We are now checking diagonal //
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal4[temp]++; 
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
//	printf("\b\b\b\b\b\b\b\b");
  printDiagonalValues();
//	printf("Integral: %016I64x\n", integral);
}
void on_the_fly128(bit64 key[2], int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp, input;
	key_schedule128(key);
	for (bit64 i = 0; i <= 0xffffffff; i++) {
//		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = encrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal4[temp]++;
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
//	printf("\b\b\b\b\b\b\b\b");
  printAntiDiagonalValues();
//	printf("Integral: %016I64x\n", integral);
}
void on_the_fly128_decryption(bit64 key[2], int round) {
	bit64 plaintext = 0x0000000000000000, ciphertext, temp, input;
	key_schedule128_inverse(key,round);
	for (bit64 i = 0; i <= 0xffffffff; i++) {
//		if ((i % 0x1000000) == 0) { printf("\b\b\b\b\b\b\b\b"); printf("Step: %I64x", i / 0x1000000); }
		ciphertext = decrypt(plaintext, round);
		integral ^= ciphertext;
		input = ciphertext;
		// We are now checking diagonal //
		temp = ((input & 0xf000) >> 12) ^ ((input & 0xf0000) >> 12) ^ ((input & 0xf000000000) >> 28) ^ ((input & 0xf00000000000000) >> 44);
		antidiagonal1[temp]++;
		temp = ((input & 0xf) >> 0) ^ ((input & 0xf00000) >> 16) ^ ((input & 0xf0000000000) >> 32) ^ ((input & 0xf000000000000000) >> 48);
		antidiagonal2[temp]++;
		temp = ((input & 0xf0) >> 4) ^ ((input & 0xf000000) >> 20) ^ ((input & 0xf00000000000) >> 36) ^ ((input & 0xf000000000000) >> 36);
		antidiagonal3[temp]++;
		temp = ((input & 0xf00) >> 8) ^ ((input & 0xf0000000) >> 24) ^ ((input & 0xf00000000) >> 24) ^ ((input & 0xf0000000000000) >> 40);
		antidiagonal4[temp]++;
		plaintext += 0x0000000100000001;
	}
	for (int i = 0; i < 65536; i++) {
		hits[0] = hits[0] + antidiagonal1[i] * (antidiagonal1[i] - 1) / 2;
		hits[1] = hits[1] + antidiagonal2[i] * (antidiagonal2[i] - 1) / 2;
		hits[2] = hits[2] + antidiagonal3[i] * (antidiagonal3[i] - 1) / 2;
		hits[3] = hits[3] + antidiagonal4[i] * (antidiagonal4[i] - 1) / 2;
	}
//	printf("\b\b\b\b\b\b\b\b");
  printDiagonalValues();
//	printf("Integral: %016I64x\n", integral);
}
void user_input() {
	int choice, keyselection=-1, rounds;
	bit64 key = 0x357a357a357a357a, key128[2] = { 0 };
	bit64 keys[5] = { 0x357a357a357a357a, 0xf903f903f903f903, 0x0000000000000000, 0x5555555555555555 };
	bit32 AESkey[4] = { 0 };
	printf(""
		"(1) Small AES Invariant Subspace - Encryption Experiment\n"
		"(2) Small AES Invariant Subspace - Encryption Experiment (128-bit key)\n"
		"(3) Small AES Invariant Subspace - Decryption Experiment\n"
		"(4) Small AES Invariant Subspace - Decryption Experiment (128-bit key)\n"
		"Choice: ");
	scanf_s("%d", &choice);
#ifdef WIN32
		if (choice == 1) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
			scanf_s("%d", &keyselection);
			if (keyselection == 4) {
				printf("Enter key (e.g. 0123456789abcdef): ");
				scanf_s("%016I64x", &keys[keyselection]);
				printf("Selected key is: %016I64x\n", keys[keyselection]);
			}
			on_the_fly(keys[keyselection], rounds);
		}
		else if (choice == 2) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("Enter key part 1 (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &key128[0]);
			printf("Enter key part 2 (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &key128[1]);
			printf("Selected key is: %016I64x %016I64x\n", key128[0], key128[1]);
			on_the_fly128(key128, rounds);
		}
		else if (choice == 3) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("(0) %I64x\n(1) %I64x\n(2) %I64x\n(3) %I64x\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
			scanf_s("%d", &keyselection);
			if (keyselection == 4) {
				printf("Enter key (e.g. 0123456789abcdef): ");
				scanf_s("%016I64x", &keys[keyselection]);
				printf("Selected key is: %016I64x\n", keys[keyselection]);
			}
			on_the_fly_decryption(keys[keyselection], rounds);
		}
		else if (choice == 4) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("Enter key part 1 (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &key128[0]);
			printf("Enter key part 2 (e.g. 0123456789abcdef): ");
			scanf_s("%016I64x", &key128[1]);
			printf("Selected key is: %016I64x %016I64x\n", key128[0], key128[1]);
			on_the_fly128_decryption(key128, rounds);
		}
#else
		if (choice == 1) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("(0) %lx\n(1) %lx\n(2) %lx\n(3) %lx\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
			scanf_s("%d", &keyselection);
			if (keyselection == 4) {
				printf("Enter key (e.g. 0123456789abcdef): ");
				scanf_s("%016lx", &keys[keyselection]);
				printf("Selected key is: %016lx\n", keys[keyselection]);
			}
			on_the_fly(keys[keyselection], rounds);
		}
		else if (choice == 2) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("Enter key part 1 (e.g. 0123456789abcdef): ");
			scanf_s("%016lx", &key128[0]);
			printf("Enter key part 2 (e.g. 0123456789abcdef): ");
			scanf_s("%016lx", &key128[1]);
			printf("Selected key is: %016lx %016lx\n", key128[0], key128[1]);
			on_the_fly128(key128, rounds);
		}
		else if (choice == 3) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("(0) %lx\n(1) %lx\n(2) %lx\n(3) %lx\n(4) Enter manually\nSelect key: ", keys[0], keys[1], keys[2], keys[3]);
			scanf_s("%d", &keyselection);
			if (keyselection == 4) {
				printf("Enter key (e.g. 0123456789abcdef): ");
				scanf_s("%016lx", &keys[keyselection]);
				printf("Selected key is: %016lx\n", keys[keyselection]);
			}
			on_the_fly_decryption(keys[keyselection], rounds);
		}
		else if (choice == 4) {
			printf("Number of rounds: ");
			scanf_s("%d", &rounds);
			printf("Enter key part 1 (e.g. 0123456789abcdef): ");
			scanf_s("%016lx", &key128[0]);
			printf("Enter key part 2 (e.g. 0123456789abcdef): ");
			scanf_s("%016lx", &key128[1]);
			printf("Selected key is: %016lx %016lx\n", key128[0], key128[1]);
			on_the_fly128_decryption(key128, rounds);
		}
#endif
	printf("Time: %u seconds\n", clock() / CLOCKS_PER_SEC);
}
void main() {
	user_input();
}
