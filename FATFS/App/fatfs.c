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

#include "string.h"

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




FIL file;
int ret;
char currfile[100];
uint8_t fbuf[512];
uint32_t nRead;
uint32_t readtotal=0;
uint32_t tickstart;

void file_write_test()
{
  for(int i=0;i<512;i++)
  {
    fbuf[i] = i%0xff;
  }

  strcpy(currfile, "0:/");
  strcat(currfile, "file0.bin");

  printf("write to file %s\n", currfile);

  ret = f_open(&file, currfile, FA_CREATE_ALWAYS | FA_WRITE);

  if(ret != FR_OK)
  {
    printf("%s open failed\n", currfile);
    f_close(&file);
    DO_ASSERT();
    return;
  }

  tickstart = HAL_GetTick();

  for(int i=0;i<2048;i++)
  {
    if(f_write(&file, fbuf, 512, (UINT*)&nRead) != FR_OK)
    {
      printf("f_write Error\n");
      DO_ASSERT();
      break;
    }
    if(nRead != 512)
    {
      printf("nRead is %lu\n", nRead);
      break;
    }
    readtotal+=512;
  }
  f_close(&file);

  printf("writed %lu bytes\n", readtotal);

  printf("Elapsed %lu\n", HAL_GetTick()-tickstart);



  printf("nRead from file %s\n", currfile);

  ret = f_open(&file, currfile, FA_READ);

  if(ret != FR_OK)
  {
    printf("%s open failed\n", currfile);
    f_close(&file);
    DO_ASSERT();
    return;
  }

  tickstart = HAL_GetTick();

  readtotal=0;

  while(1)
  {
    ret = f_read(&file, fbuf, sizeof(fbuf), (UINT*)&nRead);
    if(ret != FR_OK) break;
    if(nRead == 0) break;
    readtotal += nRead;
  }
  f_close(&file);

  printf("Readed %lu bytes\n", readtotal);

  printf("Elapsed %lu\n", HAL_GetTick()-tickstart);

}


/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  FATFS_Init_Check();

  file_write_test();
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
