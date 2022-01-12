/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */


#include "stdio.h"

extern void DO_ASSERT();

uint32_t sdcard_bytes;

DIR dir;
FILINFO fno;
FRESULT retSD;

char str_fattrib[6][7] = {
    "Rdonly",
    "Hidden",
    "System",
    "Dirtry",
    "Archiv",
    "invald",
};

char* get_string_fattrib(BYTE attr)
{
  if(attr == AM_RDO)
    return str_fattrib[0];
  else if(attr == AM_HID)
    return str_fattrib[1];
  else if(attr == AM_HID)
    return str_fattrib[1];
  else if(attr == AM_SYS)
    return str_fattrib[2];
  else if(attr == AM_DIR)
    return str_fattrib[3];
  else if(attr == AM_ARC)
    return str_fattrib[4];
  else
    return str_fattrib[5];
}


void FATFS_Init_Check()
{

  retSD = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);

  if(retSD == FR_OK)
  {
    printf("Opening %s\n", SDPath);
    retSD = f_opendir(&dir, SDPath);

    if(retSD == FR_OK) {
      while(1)
      {
        retSD = f_readdir(&dir, &fno);
        if(retSD != FR_OK) break;
        if(fno.fname[0] == 0) break;

        printf("%s\t%s\n", get_string_fattrib(fno.fattrib), fno.fname);

      }

    }
    else
    {
      printf("%s Open Failed\n", SDPath);

      DO_ASSERT();
    }
  }
  else
  {
    printf("mount Failed\n");

    DO_ASSERT();
  }

}




/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  FATFS_Init_Check();
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
