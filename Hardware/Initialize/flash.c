#include "flash.h"
#include "usart.h"
/*准备写入的测试数据*/
#define DATA_32                 ((uint32_t)65534)

#define USART1_DEBUG 0

//#define USART6_printf USART2_printf

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_8   
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_9  
/*
	写流程 ：
	解锁  - > 选择操作位数 - > 擦除扇区 - > 写入数据 - > 上锁
	读流程：
	直接用指针读取然后赋值

*/

/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes */

#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base address of Sector 12, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base address of Sector 13, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base address of Sector 14, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base address of Sector 15, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base address of Sector 16, 64 Kbytes  */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base address of Sector 17, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base address of Sector 18, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base address of Sector 19, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base address of Sector 20, 128 Kbytes */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base address of Sector 21, 128 Kbytes */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base address of Sector 22, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base address of Sector 23, 128 Kbytes */


static uint32_t GetSector(uint32_t Address);
static int ERASE_Sector(uint32_t Strat_Sector,uint32_t End_Sector);
/**
  * @brief  InternalFlash_Test,对内部FLASH进行读写测试
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
	uint32_t uwStartSector = 0;					//开始扇区
	uint32_t uwEndSector = 0;						//结束扇区
	
	uint32_t uwAddress = 0;							//开始地址
	uint32_t uwSectorCounter = 0;				//擦除的扇区

	__IO uint32_t uwData32 = 0;					
	__IO uint32_t uwMemoryProgramStatus = 0;
	
  /* FLASH 解锁 ********************************/
  /* 使能访问FLASH控制寄存器 */
  FLASH_Unlock();
    
  /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
  /* 清除各种FLASH的标志位 */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* 开始擦除操作 */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 以“字”的大小进行操作 */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)	//擦除失败
    { 
      /*擦除出错，返回，实际应用中可加入处理 */
			USART6_printf("所选扇区擦除失败\r\n");
			return -1;
			
    }
    /* 计数器指向下一个扇区 */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;	//进入下个扇区的头地址
    } 
    else 
    {
      uwSectorCounter += 8;		//进入下个扇区的头地址
    }
  }
	USART6_printf("所选扇区擦除完毕\r\n");
  /* 以“字”的大小为单位写入数据 ********************************/
  uwAddress = FLASH_USER_START_ADDR;
	USART6_printf("开始从扇区写数据\r\n");
  while (uwAddress < FLASH_USER_END_ADDR)
  {
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 4;		//进入下个字的头地址  要是半个字 +2 一个字节+1
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			USART6_printf("数据写入失败\r\n");
			return -1;
			
    }
  }
	USART6_printf("数据写入成功\r\n");

  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 

	USART6_printf("开始读数据\r\n");
  /* 从FLASH中读取出数据进行校验***************************************/
  /*  MemoryProgramStatus = 0: 写入的数据正确
      MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数 */
  uwAddress = FLASH_USER_START_ADDR;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < FLASH_USER_END_ADDR)
  {
    uwData32 = *(__IO uint32_t*)uwAddress;
    if (uwData32 != DATA_32)
    {
      uwMemoryProgramStatus++;  
    }

    uwAddress = uwAddress + 4;			//进入下个字的头地址  要是半个字 +2 一个字节+1
  }  
  /* 数据校验不正确 */
  if(uwMemoryProgramStatus)
  {    
		USART6_printf("数据读出错误\r\n");
		return -1;
		
  }
  else /*数据校验正确*/
  { 
		USART6_printf("数据读出成功\r\n");
		return 0;   		
  }
}

/**
  * @brief  根据输入的地址给出它所在的sector 扇区
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  

  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }

  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_Sector_23;  
  }
  return sector;
}

#if USART1_DEBUG == 1

/*
擦除扇区函数 擦除都是以一个扇区作为单位的 每次擦除一个扇区  读取可以不用读整个扇区，
只需要读需要数据的扇区即可
参数：Strat_Sector 起始扇区 End_Sector 终始扇区
*/
static int ERASE_Sector(uint32_t Strat_Sector,uint32_t End_Sector)
{
	/*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
	uint32_t uwStartSector = 0;					//开始扇区
	uint32_t uwEndSector = 0;						//结束扇区
	uint32_t uwSectorCounter = 0;				//擦除的扇区

  /* FLASH 解锁 ********************************/
  /* 使能访问FLASH控制寄存器 */
  FLASH_Unlock();
    
  /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
  /* 清除各种FLASH的标志位 */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(Strat_Sector);		//开始的扇区地址
	uwEndSector = GetSector(End_Sector);				//结束的扇区地址

  /* 开始擦除操作 */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 以“字”的大小进行操作 */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)	//擦除失败
    { 
      /*擦除出错，返回，实际应用中可加入处理 */
			USART6_printf("所选扇区擦除失败\r\n");
			return -1;
			
    }
    /* 计数器指向下一个扇区 */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;	//进入下个扇区的头地址
    } 
    else 
    {
      uwSectorCounter += 8;		//进入下个扇区的头地址
    }
  }
	USART6_printf("所选扇区擦除完毕\r\n");
	return 1;
}
/*
写入以字为单位的数据
不同的数据不要写在一个扇区内，因为这次写入的时候会擦除上次写入flash的数据
最好一次写入多个数据,然后连续读出多个数据,因为这次写的时候相同扇区上次的数据被抹掉
*/
int Flash_Wordbyte_Write(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	uint32_t DATA = 0;
	uwAddress = Start_Address;					//开始的扇区地址
	End_Address = uwAddress + 4*DATA_num;				//得到终止地址
	ERASE_Sector(Start_Address,End_Address);
	USART6_printf("开始从扇区写数据\r\n");	
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramWord(uwAddress, DATA) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 4;		//进入下个字的头地址  要是半个字 +2 一个字节+1
USART6_printf("数据为: %d",(int)DATA);
			//			USART6_printf("%x\r\n",DATA);
			Receive_DATA++;								//移动到下一个数据
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			USART6_printf("数据写入失败\r\n");
			return -1;		
    }	
  }
  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	USART6_printf("数据写入成功\r\n");
	return 1;
}
/*
	读以字为单位的数据
	参数：Start_Address 起始地址，DATA_num数据的个数
*/
uint32_t Flash_Wordbyte_Read(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	int num = 0;
	USART6_printf("开始读数据\r\n");
  uwAddress = Start_Address;				//开始读取的地址
	End_Address = Start_Address + 4*DATA_num;		//结束地址
 
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 4;			//进入下个字的头地址  要是半个字 +2 一个字节+1
		USART6_printf("数据为: %d",(int)Receive_DATA[num] );
  }  
	return 1;
}
/*
写入以半字为单位的数据 即两个字节为一个单位
*/
int Flash_HalfWordbyte_Write(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 2*DATA_num;				//得到终止地址
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//开始的扇区地址
	USART6_printf("开始从扇区写数据\r\n");	
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		USART6_printf("数据为: %d\r\n",DATA);
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramHalfWord(uwAddress, DATA) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 2;		//进入下个字的头地址  要是半个字 +2 一个字节+1
			
			Receive_DATA++;								//移动到下一个数据
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			USART6_printf("数据写入失败\r\n");
			return -1;		
    }	
  }
  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	USART6_printf("数据写入成功\r\n");
	return 1;
}
/*
读以半字为单位的数据 即两个字节
*/

uint16_t Flash_HalfWordbyte_Read(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	int num = 0;
	USART6_printf("开始读数据\r\n");
  uwAddress = Start_Address;				//开始读取的地址
	End_Address = Start_Address + 2*DATA_num;		//结束地址
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 2;			//进入下个字的头地址  要是半个字 +2 一个字节+1
		USART6_printf("数据为: %d",(int)Receive_DATA[num] );
  }  
	return 1;
}
/*
写一个字节
*/
int Flash_Byte_Write(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 1*DATA_num;	
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//开始的扇区地址
	End_Address = uwAddress + 1*DATA_num;				//得到终止地址
	USART6_printf("开始从扇区写数据\r\n");	
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramByte(uwAddress, DATA) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 1;		//进入下个字的头地址  要是半个字 +2 一个字节+1
			USART6_printf("%d\r\n",DATA);
			Receive_DATA++;								//移动到下一个数据
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			USART6_printf("数据写入失败\r\n");
			return -1;		
    }	
  }
  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	USART6_printf("数据写入成功\r\n");
	return 1;
}
/*
读一个字节
*/
uint8_t Flash_Byte_Read(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	int num = 0;
	USART6_printf("开始读数据\r\n");
  uwAddress = Start_Address;				//开始读取的地址
	End_Address = Start_Address + 1*DATA_num;		//结束地址
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 1;			//进入下个字的头地址  要是半个字 +2 一个字节+1
  }  
	return 1;
}

#else


/*
擦除扇区函数 擦除都是以一个扇区作为单位的 每次擦除一个扇区  读取可以不用读整个扇区，
只需要读需要数据的扇区即可
参数：Strat_Sector 起始扇区 End_Sector 终始扇区
*/
static int ERASE_Sector(uint32_t Strat_Sector,uint32_t End_Sector)
{
	/*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
	uint32_t uwStartSector = 0;					//开始扇区
	uint32_t uwEndSector = 0;						//结束扇区
	uint32_t uwSectorCounter = 0;				//擦除的扇区

  /* FLASH 解锁 ********************************/
  /* 使能访问FLASH控制寄存器 */
  FLASH_Unlock();
    
  /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
  /* 清除各种FLASH的标志位 */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(Strat_Sector);		//开始的扇区地址
	uwEndSector = GetSector(End_Sector);				//结束的扇区地址

  /* 开始擦除操作 */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 以“字”的大小进行操作 */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)	//擦除失败
    { 
      /*擦除出错，返回，实际应用中可加入处理 */
			return -1;
			
    }
    /* 计数器指向下一个扇区 */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;	//进入下个扇区的头地址
    } 
    else 
    {
      uwSectorCounter += 8;		//进入下个扇区的头地址
    }
  }
	return 1;
}
/*
写入以字为单位的数据
不同的数据不要写在一个扇区内，因为这次写入的时候会擦除上次写入flash的数据
最好一次写入多个数据,然后连续读出多个数据,因为这次写的时候相同扇区上次的数据被抹掉
*/
int Flash_Wordbyte_Write(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	

	
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	uint32_t DATA = 0;
	uwAddress = Start_Address;					//开始的扇区地址
	End_Address = uwAddress + 4*DATA_num;				//得到终止地址
	ERASE_Sector(Start_Address,End_Address);


  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramWord(uwAddress, DATA) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 4;		//进入下个字的头地址  要是半个字 +2 一个字节+1
			Receive_DATA++;								//移动到下一个数据
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			return -1;		
    }	
  }
  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	return 1;
}
/*
	读以字为单位的数据
	参数：Start_Address 起始地址，DATA_num数据的个数
*/
uint32_t Flash_Wordbyte_Read(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	
		uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	int num = 0;
  uwAddress = Start_Address;				//开始读取的地址
	End_Address = Start_Address + 4*DATA_num;		//结束地址
 
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 4;			//进入下个字的头地址  要是半个字 +2 一个字节+1
  }  
	return 1;
	
	
//	uint32_t uwAddress = 0;							//开始读取开始地址
//	uint32_t End_Address = 0;
//	int num = 0;
//  uwAddress = Start_Address;				//开始读取的地址
//	End_Address = Start_Address + 4*DATA_num;		//结束地址
// 
//  while (uwAddress < End_Address)
//  {
//    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
//		num++;
//    uwAddress = uwAddress + 4;			//进入下个字的头地址  要是半个字 +2 一个字节+1
//  }  
//	return 1;
}
/*
写入以半字为单位的数据 即两个字节为一个单位
*/
int Flash_HalfWordbyte_Write(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 2*DATA_num;				//得到终止地址
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//开始的扇区地址
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramHalfWord(uwAddress, DATA) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 2;		//进入下个字的头地址  要是半个字 +2 一个字节+1
			Receive_DATA++;								//移动到下一个数据
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			return -1;		
    }	
  }
  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	return 1;
}
/*
读以半字为单位的数据 即两个字节
*/
uint16_t Flash_HalfWordbyte_Read(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	int num = 0;
  uwAddress = Start_Address;				//开始读取的地址
	End_Address = Start_Address + 2*DATA_num;		//结束地址
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 2;			//进入下个字的头地址  要是半个字 +2 一个字节+1
  }  
	return 1;
}
/*
写一个字节
*/
int Flash_Byte_Write(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 1*DATA_num;	
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//开始的扇区地址
	End_Address = uwAddress + 1*DATA_num;				//得到终止地址
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord 为写一个字的函数 写入成功返回 FLASH_COMPLETE 即写入成功*/
    if (FLASH_ProgramByte(uwAddress, DATA) == FLASH_COMPLETE)		//写入成功
    {
      uwAddress = uwAddress + 1;		//进入下个字的头地址  要是半个字 +2 一个字节+1
			Receive_DATA++;								//移动到下一个数据
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			return -1;		
    }	
  }
  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	return 1;
}
/*
读一个字节
*/
uint8_t Flash_Byte_Read(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//开始读取开始地址
	uint32_t End_Address = 0;
	int num = 0;
  uwAddress = Start_Address;				//开始读取的地址
	End_Address = Start_Address + 1*DATA_num;		//结束地址
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 1;			//进入下个字的头地址  要是半个字 +2 一个字节+1
  }  
	return 1;
}

#endif



