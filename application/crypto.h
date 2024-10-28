/*
 * cryptography_module.h
 *
 *  Created on: Mar 30, 2022
 *      Author: wojciech
 */

#ifndef CRYPTOGRAPHY_MODULE_CRYPTOGRAPHY_MODULE_H_
#define CRYPTOGRAPHY_MODULE_CRYPTOGRAPHY_MODULE_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#endif
#include <string.h>
#include <stdint.h>


void CRYPTO_initSecretKey();
void CRYPTO_initPrivateKey(const uint8_t* key);
void CRYPTO_deInitPrivateKey();

void CRYPTO_decryptBlockWithSecretKey(uint8_t* buf);
void CRYPTO_decryptBlockWithPrivateKey(uint8_t* buf);


#ifdef __cplusplus
}
#endif
#endif /* CRYPTOGRAPHY_MODULE_CRYPTOGRAPHY_MODULE_H_ */
