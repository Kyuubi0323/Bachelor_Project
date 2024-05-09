/**
  ******************************************************************************
  * @file    demo_encrypt.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of encrypt lib operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#include "simcom_api.h"
#include "string.h"
#include "stdio.h"

/**
  * @brief  encrypt demo
  * @param  void
  * @note   this demo will create encrypt based on AES and base64 then decode it
  * @retval ret
  */
void sAPP_EncrypeDemo(void)
{
    UINT8 key[17] = {"0123456789ABCDEF"};
    UINT8 Plaintext[64];
    UINT8 ciphertext[64];

    UINT8 tmpstr[64];
    int inlen = 0,outlen = 0,tmpstrlen = 0;
    int ret = 0;


    UINT8 buffer[128];

        
    memset(Plaintext,0,sizeof(Plaintext));
    memset(ciphertext,0,sizeof(ciphertext));
    memset(tmpstr,0,sizeof(tmpstr));
    
    /*aes test*/    
    inlen = snprintf((INT8*)Plaintext,sizeof(Plaintext),"%s","1122334455667788");
    ret = sAPI_AesEncrypt(key,128,Plaintext,ciphertext);
    if(ret == 0)
    {
        ret = sAPI_AesDecrypt(key,128,ciphertext,tmpstr);
        if(ret == 0)
        {
            if(memcmp( Plaintext, tmpstr, strlen((INT8*)Plaintext) ) == 0)
                sAPI_Debug("aes test sucess!!");
        }
    }

    /*base64 test*/
    

    memset(buffer,0,sizeof(buffer));
    
    ret = sAPI_Base64Encode(buffer,sizeof(buffer),&outlen,(const UINT8 *)"123456789",strlen("123456789"));
    sAPI_Debug("ret[%d] outlen[%d] buffer[%s]",ret,outlen,buffer);
    if(ret != 0)
        return;

    
    memset(tmpstr,0,sizeof(tmpstr));
    tmpstrlen = 0;

    ret = sAPI_Base64Decode(tmpstr,sizeof(tmpstr),&tmpstrlen,buffer,outlen);
    if(ret != 0)
        return;
    
    sAPI_Debug("ret[%d] tmpstrlen[%d] buffer[%s]",ret,tmpstrlen,tmpstr);

    if(memcmp( "123456789", tmpstr, strlen("123456789") ) == 0)
    {
        sAPI_Debug("Base64 test sucess");
    }


    

   
}




