/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sdio.h"

/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) 
	{
			case DEV_MMC:
				if(SD_GetState()==SD_CARD_ERROR)
					return STA_NOINIT;
				return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) 
	{
			case DEV_MMC :
				if(SD_GetState()==SD_CARD_ERROR)
					return STA_NOINIT;
				return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	
	switch (pdrv) {
			case DEV_MMC :
			
				SD_Error SD_State;
				if(count ==  1)
					SD_State = SD_ReadBlock(buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize);
				else
					SD_State=SD_ReadMultiBlocks(buff,sector*SDCardInfo.CardBlockSize,SDCardInfo.CardBlockSize,count);

				if(SD_State!=SD_OK)
					return RES_ERROR;
				
				while(SD_WaitReadOperation()!=SD_OK);
				return RES_OK;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{

	switch (pdrv) {
			case DEV_MMC :
				if (!count) 
					return RES_PARERR;
				
				SD_Error SD_State;
				if(count ==  1)
					SD_State = SD_WriteBlock((uint8_t *)buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize);
				else
					SD_State=SD_WriteMultiBlocks((uint8_t *)buff,sector*SDCardInfo.CardBlockSize,SDCardInfo.CardBlockSize,count);
				
				if(SD_State!=SD_OK)
					return RES_ERROR;
				
				while(SD_WaitWriteOperation()!=SD_OK);
				return RES_OK;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{

	switch (pdrv) {
			case DEV_MMC :
				switch (cmd) 
				{
					// Get R/W sector size (WORD) 
					case GET_SECTOR_SIZE :    
						*(WORD * )buff = SDCardInfo.CardBlockSize;
						break;
					// Get erase block size in unit of sector (DWORD)
					case GET_BLOCK_SIZE :      
						*(DWORD * )buff = 1;
						break;

					case GET_SECTOR_COUNT:
						*(DWORD * )buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
						break;
					case CTRL_SYNC :
						break;
				}
				return RES_OK;

	}

	return RES_PARERR;
}

