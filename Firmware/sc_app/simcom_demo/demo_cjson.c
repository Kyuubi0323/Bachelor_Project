/**
  ******************************************************************************
  * @file    demo_cjson.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of cjson lib operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#include "cJSON.h"
#include "simcom_api.h"

/**
  * @brief  Cjson parse operation,
  * @param  void
  * @note   cJSON_InitHooks must be called first,And assign malloc_fn a value of sAPI_Malloc,
			assign free_fn a value of sAPI_Free,Otherwise, the module may be abnormal
  * @retval 0
  */
INT32 CjsonParseTest(void)
{
    INT8* data = "{\"company\":\"simcom\",\"module\":\"A7600E\"}";
    cJSON* root = NULL;
    cJSON* itemName = NULL;
    cJSON* item_module = NULL;



    cJSON_Hooks hooks;
    hooks.malloc_fn = sAPI_Malloc;
    hooks.free_fn = sAPI_Free;
    cJSON_InitHooks(&hooks);

    root = cJSON_Parse(data);
    if(root == NULL)
    {
        sAPI_Debug("json parse fail");
        return -1;
    }

    itemName = cJSON_GetObjectItem(root, "company");

    if(itemName)
        sAPI_Debug("company[%s]",itemName->valuestring);


    item_module = cJSON_GetObjectItem(root, "module");
    if(item_module)
        sAPI_Debug("module[%s]",item_module->valuestring);

    cJSON_Delete(root);

    return 0;
}

/**
  * @brief  Cjson create operation,
  * @param  void
  * @note   create a json item as following
  
	{
    "company":simcom,
    "module":A7600E
	}
	
  * @retval 0
  */
INT32 CjsonCreateTest(void)
{
    cJSON * root =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "company", cJSON_CreateString("simcom"));
    cJSON_AddItemToObject(root, "module", cJSON_CreateString("A7600E"));
    //cJSON_AddItemToObject(root, "number", cJSON_CreateNumber(0xFFFFFFFF));

    sAPI_Debug("%s\n", cJSON_Print(root));

    cJSON_Delete(root);

    return 0;
}

/**
  * @brief  Cjson demo create
  * @param  void
  * @note   create a json item and parse it.
  * @retval 0
  */
void sAPP_CjsonTest(void)
{
    INT32 ret = -1;

    ret = CjsonParseTest();
    if(ret == 0)
        sAPI_Debug("cJson parse test sucess");

    ret = CjsonCreateTest();
    if(ret == 0)
        sAPI_Debug("cJson create test sucess");
}
