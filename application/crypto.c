/*
 * cryptography_module.c
 *
 *  Created on: Mar 30, 2022
 *      Author: wojciech
 */

#include "crypto.h"

#include "aes.h"


const uint8_t g_secret_key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };	//nie udostepniać
//uint8_t g_private_key[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static struct AES_ctx g_ctx_PrivateKey;			// encode/decode module
static struct AES_ctx g_ctx_SecretKey;

static uint8_t secret_key_initiated = 0;
static uint8_t private_key_initiated = 0;


void CRYPTO_initSecretKey()
{
	AES_init_ctx(&g_ctx_SecretKey, g_secret_key);
	secret_key_initiated = 1;
}

void CRYPTO_initPrivateKey(const uint8_t* key)
{
	AES_init_ctx(&g_ctx_PrivateKey, key);
	private_key_initiated = 1;
}

void CRYPTO_deInitPrivateKey()
{
	memset((uint8_t*)&g_ctx_PrivateKey, 0x00, sizeof(struct AES_ctx));
	private_key_initiated = 0;
}

void CRYPTO_decryptBlockWithSecretKey(uint8_t* buf)
{
	if (secret_key_initiated == 1)
	{
		AES_ECB_decrypt(&g_ctx_SecretKey, buf);
	}
}

void CRYPTO_decryptBlockWithPrivateKey(uint8_t* buf)
{
	if (private_key_initiated == 1)
	{
		AES_ECB_decrypt(&g_ctx_PrivateKey, buf);
	}
}

//do testów
void encryptBlockWithSecretKey(uint8_t *buf)
{
	if (secret_key_initiated == 1)
	{
		AES_ECB_encrypt(&g_ctx_SecretKey, buf);
	}
}

void encryptBlockWithPrivateKey(uint8_t *buf)
{
	if (private_key_initiated == 1)
	{
		AES_ECB_encrypt(&g_ctx_PrivateKey, buf);
	}
}
