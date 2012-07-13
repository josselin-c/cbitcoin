//
//  testCBBlockHeaders.c
//  cbitcoin
//
//  Created by Matthew Mitchell on 12/07/2012.
//  Copyright (c) 2012 Matthew Mitchell
//  
//  This file is part of cbitcoin.
//
//  cbitcoin is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  cbitcoin is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with cbitcoin.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include "CBBlockHeaders.h"
#include <time.h>
#include "stdarg.h"

void err(CBError a,char * format,...);
void err(CBError a,char * format,...){
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
	printf("\n");
}

int main(){
	unsigned int s = (unsigned int)time(NULL);
	s = 1337544566;
	printf("Session = %ui\n",s);
	srand(s);
	CBEvents events;
	events.onErrorReceived = err;
	// Test deserialisation
	u_int8_t data[165] = {
		0x02, // Two headers
		// 1st header
		0x01,0x00,0x00,0x00, // Version 1
		0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA, // Previous block hash
		0xC8,0x57,0xA1,0xFB,0xCA,0x8F,0x22,0x4B,0x21,0xB5,0xFE,0x94,0x22,0xB0,0x7C,0xBA,0x87,0x2A,0xB2,0xF8,0xCE,0x39,0x56,0xBD,0x99,0x4A,0x44,0x23,0x2F,0x8C,0xA3,0xFF, // Merkle root
		0x57,0x5C,0xFF,0x4F, // Time 1342135383
		0x67,0xFB,0x31,0x1B, // Difficulty
		0xAB,0x2B,0x02,0x9C, // Nounce
		0x00, // Transactions
		0x00, // Null byte
		// 2nd header
		0x02,0x00,0x00,0x00, // Version 2
		0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA, // Previous block hash
		0xE8,0x47,0xA9,0xBB,0xD8,0x4F,0x72,0x9B,0x11,0xB5,0xFC,0x96,0x42,0xE0,0x9C,0xBA,0xA7,0xF9,0xE2,0xFD,0xC8,0x49,0xB6,0xDD,0x49,0x8A,0xE4,0xA3,0x3F,0x0C,0x23,0x1F, // Merkle root
		0x58,0x5C,0xFF,0x4F, // Time 1342135384
		0x69,0xAB,0x21,0x0C, // Difficulty
		0xB2,0xAF,0x31,0x0D, // Nounce
		0x07, // Transactions
		0x00, // Null byte
	};
	CBByteArray * bytes = CBNewByteArrayWithDataCopy(data, 165, &events);
	CBBlockHeaders * blockHeaders = CBNewBlockHeadersFromData(bytes, &events);
	if(CBBlockHeadersDeserialise(blockHeaders) != 165){
		printf("DESERIALISATION LEN FAIL\n");
		return 1;
	}
	if (blockHeaders->headerNum != 2) {
		printf("DESERIALISATION NUM FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[0]->version != 1) {
		printf("DESERIALISATION FIRST VERSION FAIL\n");
		return 1;
	}
	if (memcmp(CBByteArrayGetData(blockHeaders->blockHeaders[0]->prevBlockHash),(u_int8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA},32)) {
		printf("DESERIALISATION FIRST HEADER PREV HASH FAIL\n0x");
		u_int8_t * d = CBByteArrayGetData(blockHeaders->blockHeaders[0]->prevBlockHash);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (u_int8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	if (memcmp(CBByteArrayGetData(blockHeaders->blockHeaders[0]->merkleRoot),(u_int8_t []){0xC8,0x57,0xA1,0xFB,0xCA,0x8F,0x22,0x4B,0x21,0xB5,0xFE,0x94,0x22,0xB0,0x7C,0xBA,0x87,0x2A,0xB2,0xF8,0xCE,0x39,0x56,0xBD,0x99,0x4A,0x44,0x23,0x2F,0x8C,0xA3,0xFF},32)) {
		printf("DESERIALISATION FIRST HEADER MERKLE ROOT FAIL\n0x");
		u_int8_t * d = CBByteArrayGetData(blockHeaders->blockHeaders[0]->merkleRoot);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (u_int8_t []){0xC8,0x57,0xA1,0xFB,0xCA,0x8F,0x22,0x4B,0x21,0xB5,0xFE,0x94,0x22,0xB0,0x7C,0xBA,0x87,0x2A,0xB2,0xF8,0xCE,0x39,0x56,0xBD,0x99,0x4A,0x44,0x23,0x2F,0x8C,0xA3,0xFF};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	if (blockHeaders->blockHeaders[0]->time != 1342135383) {
		printf("DESERIALISATION FIRST HEADER TIME FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[0]->difficulty != 0x1B31FB67) {
		printf("DESERIALISATION FIRST HEADER DIFFICULTY FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[0]->nounce != 0x9C022BAB) {
		printf("DESERIALISATION FIRST HEADER NOUNCE FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[0]->transactionNum != 0) {
		printf("DESERIALISATION FIRST HEADER TRANSACTION NUM FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[1]->version != 2) {
		printf("DESERIALISATION SECOND VERSION FAIL\n");
		return 1;
	}
	if (memcmp(CBByteArrayGetData(blockHeaders->blockHeaders[1]->prevBlockHash),(u_int8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA},32)) {
		printf("DESERIALISATION SECOND HEADER PREV HASH FAIL\n0x");
		u_int8_t * d = CBByteArrayGetData(blockHeaders->blockHeaders[1]->prevBlockHash);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (u_int8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	if (memcmp(CBByteArrayGetData(blockHeaders->blockHeaders[1]->merkleRoot),(u_int8_t []){0xE8,0x47,0xA9,0xBB,0xD8,0x4F,0x72,0x9B,0x11,0xB5,0xFC,0x96,0x42,0xE0,0x9C,0xBA,0xA7,0xF9,0xE2,0xFD,0xC8,0x49,0xB6,0xDD,0x49,0x8A,0xE4,0xA3,0x3F,0x0C,0x23,0x1F},32)) {
		printf("DESERIALISATION SECOND HEADER MERKLE ROOT FAIL\n0x");
		u_int8_t * d = CBByteArrayGetData(blockHeaders->blockHeaders[1]->merkleRoot);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (u_int8_t []){0xE8,0x47,0xA9,0xBB,0xD8,0x4F,0x72,0x9B,0x11,0xB5,0xFC,0x96,0x42,0xE0,0x9C,0xBA,0xA7,0xF9,0xE2,0xFD,0xC8,0x49,0xB6,0xDD,0x49,0x8A,0xE4,0xA3,0x3F,0x0C,0x23,0x1F};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	if (blockHeaders->blockHeaders[1]->time != 1342135384) {
		printf("DESERIALISATION SECOND HEADER TIME FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[1]->difficulty != 0x0C21AB69) {
		printf("DESERIALISATION SECOND HEADER DIFFICULTY FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[1]->nounce != 0x0D31AFB2) {
		printf("DESERIALISATION SECOND HEADER NOUNCE FAIL\n");
		return 1;
	}
	if (blockHeaders->blockHeaders[1]->transactionNum != 7) {
		printf("DESERIALISATION SECOND HEADER TRANSACTION NUM FAIL\n");
		return 1;
	}
	// Test serialisation with timestamps
	memset(CBByteArrayGetData(bytes), 0, 165);
	CBReleaseObject(&blockHeaders->blockHeaders[0]->prevBlockHash);
	blockHeaders->blockHeaders[0]->prevBlockHash = CBNewByteArrayWithDataCopy((u_int8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA}, 32, &events);
	CBReleaseObject(&blockHeaders->blockHeaders[0]->merkleRoot);
	blockHeaders->blockHeaders[0]->merkleRoot = CBNewByteArrayWithDataCopy((u_int8_t []){0xC8,0x57,0xA1,0xFB,0xCA,0x8F,0x22,0x4B,0x21,0xB5,0xFE,0x94,0x22,0xB0,0x7C,0xBA,0x87,0x2A,0xB2,0xF8,0xCE,0x39,0x56,0xBD,0x99,0x4A,0x44,0x23,0x2F,0x8C,0xA3,0xFF}, 32, &events);
	CBReleaseObject(&blockHeaders->blockHeaders[1]->prevBlockHash);
	blockHeaders->blockHeaders[1]->prevBlockHash = CBNewByteArrayWithDataCopy((u_int8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA}, 32, &events);
	CBReleaseObject(&blockHeaders->blockHeaders[1]->merkleRoot);
	blockHeaders->blockHeaders[1]->merkleRoot = CBNewByteArrayWithDataCopy((u_int8_t []){0xE8,0x47,0xA9,0xBB,0xD8,0x4F,0x72,0x9B,0x11,0xB5,0xFC,0x96,0x42,0xE0,0x9C,0xBA,0xA7,0xF9,0xE2,0xFD,0xC8,0x49,0xB6,0xDD,0x49,0x8A,0xE4,0xA3,0x3F,0x0C,0x23,0x1F}, 32, &events);
	if(CBBlockHeadersSerialise(blockHeaders) != 165){
		printf("SERIALISATION LEN FAIL\n");
		return 1;
	}
	if (memcmp(data, CBByteArrayGetData(bytes), 165)) {
		printf("SERIALISATION FAIL\n0x");
		u_int8_t * d = CBByteArrayGetData(bytes);
		for (int x = 0; x < 165; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		for (int x = 0; x < 165; x++) {
			printf("%.2X",data[x]);
		}
		return 1;
	}
	CBReleaseObject(&blockHeaders);
	CBReleaseObject(&bytes);
	return 0;
}