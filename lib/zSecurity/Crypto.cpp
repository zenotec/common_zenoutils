/*
 * Copyright (c) 2014-2019 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <iostream>
#include <string.h>

//Openssl Libraries
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#include <zutils/zLog.h>

#include <zutils/zSecurity.h>


ZLOG_MODULE_INIT(zLog::Log::MODULE_SECURITY);


namespace zUtils
{
namespace zSecurity
{

// The SHA_1 algorithm outputs 160 bits at a time
#define SHA_1_OUT_LEN 20

#define AES_KEY_BITS 128
// CCMP-128 has a total length of 384 bits


static
int cmpArr(uint8_t *lft, uint8_t *rgt, size_t length)
{
  uint8_t* l = lft;
  uint8_t* r = rgt;
  int cntr = 0;

  if(length == 0)
  {
    return(0);
  }
  for(; cntr < length; ++cntr)
  {
    if(*l != *r)
      break;
  }
  if(cntr == length)
  {
    return(0);
  }
  return(*l - *r);
}

const std::string Crypto::PtkLabel("Pairwise key expansion");
const std::string Crypto::GtkLabel("Group key expansion");
const std::string Crypto::GtkInitLabel("Init Counter");
const std::string Crypto::GMK("ThisIsBigSecret");

Crypto::Crypto()
{
  this->_pmk_exp_data = PtkLabel.length() + ETH_ALEN * 2 + NOUNCE_LEN * 2 + 1;
  this->_gmk_exp_data = GtkLabel.length() + ETH_ALEN + NOUNCE_LEN;
  this->_gtkId = 0x0001;
}

Crypto::~Crypto()
{

}

void
Crypto::CreateNounce(uint8_t *nounce)
{
  // Algorithm taken from hostap function os_get_random
  std::ifstream rndF("/dev/urandom", std::ifstream::binary);
  if(rndF)
  {
    rndF.read((char*)nounce, NOUNCE_LEN);
    if(!rndF)
    {
      ZLOG_ERR("Failed to read all the random values needed");
    }
  }
}

// Method is used to wrap the key data section of the 4 way handshake in message 3
bool
Crypto::AesWrap(uint8_t *kek, uint8_t *NonEnData, uint8_t *EnData, uint8_t data_len)
{

  bool status = true;
  EVP_CIPHER_CTX *ctx;
  // This is the IV to use according to RFC 3394
  unsigned char iv[] = {'A',6,'A',6,'A',6,'A',6,'A',6,'A',6,'A',6,'A',6};
  int len;
  int en_size;
  int res;
  if((ctx = EVP_CIPHER_CTX_new()))
  {
    EVP_CIPHER_CTX_set_flags(ctx, EVP_CIPHER_CTX_FLAG_WRAP_ALLOW);

    if(1 == (res = EVP_EncryptInit_ex(ctx, EVP_aes_128_wrap(), NULL, kek, iv)))
    {

      if(1 == (res = EVP_EncryptUpdate(ctx, EnData, &en_size, NonEnData, data_len)))
      {
        if(1 != EVP_EncryptFinal_ex(ctx, EnData + en_size, &len))
        {
          ZLOG_ERR("Failed final encryption");
          status = false;
        }
      }
      else
      {
        ZLOG_ERR("Failed to encrypt data");
        status = false;
      }

    }
    else
    {
      ZLOG_ERR("Failed to initialize encryption");
      status = false;
    }
    EVP_CIPHER_CTX_free(ctx);
  }
  return(status);
}

// Method is used to unwrap the key data section of the 4 way handshake in message 3
bool
Crypto::AesUnwrap(uint8_t *kek, uint8_t *NonEnData, uint8_t *EnData, uint8_t data_len)
{
  bool status = true;
  EVP_CIPHER_CTX *ctx;
  // This is the IV to use according to RFC 3394
  unsigned char iv[] = {'A',6,'A',6,'A',6,'A',6,'A',6,'A',6,'A',6,'A',6};
  int len;
  int en_size;
  int res;
  if((ctx = EVP_CIPHER_CTX_new()))
  {
    EVP_CIPHER_CTX_set_flags(ctx, EVP_CIPHER_CTX_FLAG_WRAP_ALLOW);

    if(1 == (res = EVP_DecryptInit_ex(ctx, EVP_aes_128_wrap(), NULL, kek, iv)))
    {

      if(1 == (res = EVP_DecryptUpdate(ctx, NonEnData, &en_size, EnData, data_len)))
      {
        if(1 != EVP_DecryptFinal_ex(ctx, NonEnData + en_size, &len))
        {
          ZLOG_ERR("Failed final encryption");
          status = false;
        }
      }
      else
      {
        ZLOG_ERR("Failed to encrypt data");
        status = false;
      }

    }
    else
    {
      ZLOG_ERR("Failed to initialize encryption");
      status = false;
    }
    EVP_CIPHER_CTX_free(ctx);
  }
  return(status);
}

// Method to expand the PMK into the KCK KEK and PTK
bool
Crypto::PairwiseExp(uint8_t *key, uint8_t *anounce, uint8_t *snounce,
                    uint8_t *amac, uint8_t *smac, uint8_t *retKey)
{
  bool status = true;
  size_t pos, loopPos = 0;
  uint8_t tmpHash[SHA_1_OUT_LEN] = {0};
  uint8_t data[this->_pmk_exp_data];
  //First put the label into the data
  memcpy(data, (uint8_t*)PtkLabel.c_str(), PtkLabel.length());
  pos += PtkLabel.length();
  //Determine which mac address is lower
  if( 0 > cmpArr(amac, smac, ETH_ALEN) )
  {
    memcpy(data + pos, smac, ETH_ALEN);
    memcpy(data + pos + ETH_ALEN, amac, ETH_ALEN);
  }
  else
  {
    memcpy(data + pos, amac, ETH_ALEN);
    memcpy(data + pos + ETH_ALEN, smac, ETH_ALEN);
  }
  pos += 2 * ETH_ALEN;

  //Determine min max of Nounce values
  if( 0 > cmpArr(anounce, snounce, NOUNCE_LEN))
  {
    memcpy(data + pos, snounce, NOUNCE_LEN);
    memcpy(data + pos + NOUNCE_LEN, anounce, NOUNCE_LEN);
  }
  else
  {
    memcpy(data + pos, anounce, NOUNCE_LEN);
    memcpy(data + pos  + NOUNCE_LEN, snounce, NOUNCE_LEN);
  }
  pos += 2 * NOUNCE_LEN;

  // Counter variable that is required
  data[pos] = 0;

  uint8_t len[3] = {CCMP_PTK_LEN, (uint8_t)this->_pmk_exp_data};
  while(loopPos < CCMP_PTK_LEN)
  {
    if( SHA_1_OUT_LEN < (CCMP_PTK_LEN - loopPos))
    {
      status &= Sha_1_Prf(key, data, len, &retKey[loopPos]);
    }
    else
    {
      status &= Sha_1_Prf(key, data, len, tmpHash);
      memcpy(&retKey[loopPos], tmpHash, (CCMP_PTK_LEN - loopPos));
    }
    data[pos] += 1;
    loopPos += SHA_1_OUT_LEN;
  }

  return(status);
}

// Method to make the GTK
bool
Crypto::MakeGtk(uint8_t *amac, uint8_t *gtk)
{
  bool status = true;

  uint8_t tmpHash[SHA_1_OUT_LEN];
  uint8_t tmpNounce[NOUNCE_LEN];
  this->CreateNounce(tmpNounce);
  uint8_t len[] = {GMK_LEN, (uint8_t)this->_gmk_exp_data};
  uint8_t data[this->_gmk_exp_data] = {};
  uint8_t *gmk = (uint8_t*)this->GMK.c_str();

  memcpy(data, (uint8_t*)this->GtkLabel.c_str(), this->GtkLabel.length());
  memcpy(data+this->GMK.length(), amac, ETH_ALEN);
  memcpy(data+this->GMK.length()+ETH_ALEN, tmpNounce, NOUNCE_LEN);

  if(true == (status &= this->Sha_1_Prf(gmk, data, len, tmpHash)))
  {
    memcpy(gtk, tmpHash, GMK_LEN);
  }
  else
  {
    ZLOG_ERR("Failed to create gtk");
  }

  return(status);
}

// Method for generating a MIC hash value for the message
bool
Crypto::MicGenerateSha1(uint8_t *kck, uint8_t *data, uint8_t *len, uint8_t *mic)
{
  bool status = true;
  uint8_t tmpHash[SHA_1_OUT_LEN];
  if(true == (status &= Sha_1_Prf(kck, data, len, tmpHash)))
  {
    memcpy(mic, tmpHash, len[2]);
  }
  else
  {
    ZLOG_ERR("Failed to generate MIC");
  }
  return(status);
}

// HMAC_SHA1 Psuedorandom Function method.
bool
Crypto::Sha_1_Prf(uint8_t *key, uint8_t *data, uint8_t *len, uint8_t *retHash)
{
  bool status = true;
  const EVP_MD *sha_1 = EVP_sha1();
  HMAC_CTX *ctx;
  unsigned int sha_1_out = SHA_1_OUT_LEN;
  if(!(ctx = HMAC_CTX_new()))
  {
    ZLOG_ERR("Failed to create a new CTX in the heap");
    return(false);
  }

  status &= HMAC_Init_ex(ctx, key, len[0], sha_1, NULL);
  if(status)
  {
    status &= HMAC_Update(ctx, data, len[1]);
    status &= HMAC_Final(ctx, retHash, &sha_1_out);
  }

  HMAC_CTX_free(ctx);

  return(status);
}

}
}
