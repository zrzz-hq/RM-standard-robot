#include "flash.h"
#include "usart.h"
/*׼��д��Ĳ�������*/
#define DATA_32                 ((uint32_t)65534)

#define USART1_DEBUG 0

//#define USART6_printf USART2_printf

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Ҫ�����ڲ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_8   
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_9  
/*
	д���� ��
	����  - > ѡ�����λ�� - > �������� - > д������ - > ����
	�����̣�
	ֱ����ָ���ȡȻ��ֵ

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
  * @brief  InternalFlash_Test,���ڲ�FLASH���ж�д����
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t uwStartSector = 0;					//��ʼ����
	uint32_t uwEndSector = 0;						//��������
	
	uint32_t uwAddress = 0;							//��ʼ��ַ
	uint32_t uwSectorCounter = 0;				//����������

	__IO uint32_t uwData32 = 0;					
	__IO uint32_t uwMemoryProgramStatus = 0;
	
  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* ��ʼ�������� */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)	//����ʧ��
    { 
      /*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			USART6_printf("��ѡ��������ʧ��\r\n");
			return -1;
			
    }
    /* ������ָ����һ������ */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;	//�����¸�������ͷ��ַ
    } 
    else 
    {
      uwSectorCounter += 8;		//�����¸�������ͷ��ַ
    }
  }
	USART6_printf("��ѡ�����������\r\n");
  /* �ԡ��֡��Ĵ�СΪ��λд������ ********************************/
  uwAddress = FLASH_USER_START_ADDR;
	USART6_printf("��ʼ������д����\r\n");
  while (uwAddress < FLASH_USER_END_ADDR)
  {
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 4;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			USART6_printf("����д��ʧ��\r\n");
			return -1;
			
    }
  }
	USART6_printf("����д��ɹ�\r\n");

  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 

	USART6_printf("��ʼ������\r\n");
  /* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
  /*  MemoryProgramStatus = 0: д���������ȷ
      MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
  uwAddress = FLASH_USER_START_ADDR;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < FLASH_USER_END_ADDR)
  {
    uwData32 = *(__IO uint32_t*)uwAddress;
    if (uwData32 != DATA_32)
    {
      uwMemoryProgramStatus++;  
    }

    uwAddress = uwAddress + 4;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
  }  
  /* ����У�鲻��ȷ */
  if(uwMemoryProgramStatus)
  {    
		USART6_printf("���ݶ�������\r\n");
		return -1;
		
  }
  else /*����У����ȷ*/
  { 
		USART6_printf("���ݶ����ɹ�\r\n");
		return 0;   		
  }
}

/**
  * @brief  ��������ĵ�ַ���������ڵ�sector ����
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
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
������������ ����������һ��������Ϊ��λ�� ÿ�β���һ������  ��ȡ���Բ��ö�����������
ֻ��Ҫ����Ҫ���ݵ���������
������Strat_Sector ��ʼ���� End_Sector ��ʼ����
*/
static int ERASE_Sector(uint32_t Strat_Sector,uint32_t End_Sector)
{
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t uwStartSector = 0;					//��ʼ����
	uint32_t uwEndSector = 0;						//��������
	uint32_t uwSectorCounter = 0;				//����������

  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(Strat_Sector);		//��ʼ��������ַ
	uwEndSector = GetSector(End_Sector);				//������������ַ

  /* ��ʼ�������� */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)	//����ʧ��
    { 
      /*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			USART6_printf("��ѡ��������ʧ��\r\n");
			return -1;
			
    }
    /* ������ָ����һ������ */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;	//�����¸�������ͷ��ַ
    } 
    else 
    {
      uwSectorCounter += 8;		//�����¸�������ͷ��ַ
    }
  }
	USART6_printf("��ѡ�����������\r\n");
	return 1;
}
/*
д������Ϊ��λ������
��ͬ�����ݲ�Ҫд��һ�������ڣ���Ϊ���д���ʱ�������ϴ�д��flash������
���һ��д��������,Ȼ�����������������,��Ϊ���д��ʱ����ͬ�����ϴε����ݱ�Ĩ��
*/
int Flash_Wordbyte_Write(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	uint32_t DATA = 0;
	uwAddress = Start_Address;					//��ʼ��������ַ
	End_Address = uwAddress + 4*DATA_num;				//�õ���ֹ��ַ
	ERASE_Sector(Start_Address,End_Address);
	USART6_printf("��ʼ������д����\r\n");	
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramWord(uwAddress, DATA) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 4;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
USART6_printf("����Ϊ: %d",(int)DATA);
			//			USART6_printf("%x\r\n",DATA);
			Receive_DATA++;								//�ƶ�����һ������
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			USART6_printf("����д��ʧ��\r\n");
			return -1;		
    }	
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	USART6_printf("����д��ɹ�\r\n");
	return 1;
}
/*
	������Ϊ��λ������
	������Start_Address ��ʼ��ַ��DATA_num���ݵĸ���
*/
uint32_t Flash_Wordbyte_Read(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	int num = 0;
	USART6_printf("��ʼ������\r\n");
  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
	End_Address = Start_Address + 4*DATA_num;		//������ַ
 
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 4;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
		USART6_printf("����Ϊ: %d",(int)Receive_DATA[num] );
  }  
	return 1;
}
/*
д���԰���Ϊ��λ������ �������ֽ�Ϊһ����λ
*/
int Flash_HalfWordbyte_Write(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 2*DATA_num;				//�õ���ֹ��ַ
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//��ʼ��������ַ
	USART6_printf("��ʼ������д����\r\n");	
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		USART6_printf("����Ϊ: %d\r\n",DATA);
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramHalfWord(uwAddress, DATA) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 2;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
			
			Receive_DATA++;								//�ƶ�����һ������
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			USART6_printf("����д��ʧ��\r\n");
			return -1;		
    }	
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	USART6_printf("����д��ɹ�\r\n");
	return 1;
}
/*
���԰���Ϊ��λ������ �������ֽ�
*/

uint16_t Flash_HalfWordbyte_Read(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	int num = 0;
	USART6_printf("��ʼ������\r\n");
  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
	End_Address = Start_Address + 2*DATA_num;		//������ַ
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 2;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
		USART6_printf("����Ϊ: %d",(int)Receive_DATA[num] );
  }  
	return 1;
}
/*
дһ���ֽ�
*/
int Flash_Byte_Write(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 1*DATA_num;	
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//��ʼ��������ַ
	End_Address = uwAddress + 1*DATA_num;				//�õ���ֹ��ַ
	USART6_printf("��ʼ������д����\r\n");	
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramByte(uwAddress, DATA) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 1;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
			USART6_printf("%d\r\n",DATA);
			Receive_DATA++;								//�ƶ�����һ������
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			USART6_printf("����д��ʧ��\r\n");
			return -1;		
    }	
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	USART6_printf("����д��ɹ�\r\n");
	return 1;
}
/*
��һ���ֽ�
*/
uint8_t Flash_Byte_Read(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	int num = 0;
	USART6_printf("��ʼ������\r\n");
  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
	End_Address = Start_Address + 1*DATA_num;		//������ַ
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 1;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
  }  
	return 1;
}

#else


/*
������������ ����������һ��������Ϊ��λ�� ÿ�β���һ������  ��ȡ���Բ��ö�����������
ֻ��Ҫ����Ҫ���ݵ���������
������Strat_Sector ��ʼ���� End_Sector ��ʼ����
*/
static int ERASE_Sector(uint32_t Strat_Sector,uint32_t End_Sector)
{
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t uwStartSector = 0;					//��ʼ����
	uint32_t uwEndSector = 0;						//��������
	uint32_t uwSectorCounter = 0;				//����������

  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(Strat_Sector);		//��ʼ��������ַ
	uwEndSector = GetSector(End_Sector);				//������������ַ

  /* ��ʼ�������� */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)	//����ʧ��
    { 
      /*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
			
    }
    /* ������ָ����һ������ */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;	//�����¸�������ͷ��ַ
    } 
    else 
    {
      uwSectorCounter += 8;		//�����¸�������ͷ��ַ
    }
  }
	return 1;
}
/*
д������Ϊ��λ������
��ͬ�����ݲ�Ҫд��һ�������ڣ���Ϊ���д���ʱ�������ϴ�д��flash������
���һ��д��������,Ȼ�����������������,��Ϊ���д��ʱ����ͬ�����ϴε����ݱ�Ĩ��
*/
int Flash_Wordbyte_Write(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	

	
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	uint32_t DATA = 0;
	uwAddress = Start_Address;					//��ʼ��������ַ
	End_Address = uwAddress + 4*DATA_num;				//�õ���ֹ��ַ
	ERASE_Sector(Start_Address,End_Address);


  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramWord(uwAddress, DATA) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 4;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
			Receive_DATA++;								//�ƶ�����һ������
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;		
    }	
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return 1;
}
/*
	������Ϊ��λ������
	������Start_Address ��ʼ��ַ��DATA_num���ݵĸ���
*/
uint32_t Flash_Wordbyte_Read(uint32_t Start_Address,uint32_t *Receive_DATA,int DATA_num)
{
	
		uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	int num = 0;
  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
	End_Address = Start_Address + 4*DATA_num;		//������ַ
 
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 4;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
  }  
	return 1;
	
	
//	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
//	uint32_t End_Address = 0;
//	int num = 0;
//  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
//	End_Address = Start_Address + 4*DATA_num;		//������ַ
// 
//  while (uwAddress < End_Address)
//  {
//    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
//		num++;
//    uwAddress = uwAddress + 4;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
//  }  
//	return 1;
}
/*
д���԰���Ϊ��λ������ �������ֽ�Ϊһ����λ
*/
int Flash_HalfWordbyte_Write(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 2*DATA_num;				//�õ���ֹ��ַ
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//��ʼ��������ַ
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramHalfWord(uwAddress, DATA) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 2;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
			Receive_DATA++;								//�ƶ�����һ������
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;		
    }	
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return 1;
}
/*
���԰���Ϊ��λ������ �������ֽ�
*/
uint16_t Flash_HalfWordbyte_Read(uint32_t Start_Address,uint16_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	int num = 0;
  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
	End_Address = Start_Address + 2*DATA_num;		//������ַ
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 2;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
  }  
	return 1;
}
/*
дһ���ֽ�
*/
int Flash_Byte_Write(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	uint16_t DATA = 0;
	End_Address = uwAddress + 1*DATA_num;	
	ERASE_Sector(Start_Address,End_Address);
	uwAddress = Start_Address;					//��ʼ��������ַ
	End_Address = uwAddress + 1*DATA_num;				//�õ���ֹ��ַ
  while (uwAddress < End_Address)
  {
		DATA = *Receive_DATA;
		/*FLASH_ProgramWord Ϊдһ���ֵĺ��� д��ɹ����� FLASH_COMPLETE ��д��ɹ�*/
    if (FLASH_ProgramByte(uwAddress, DATA) == FLASH_COMPLETE)		//д��ɹ�
    {
      uwAddress = uwAddress + 1;		//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
			Receive_DATA++;								//�ƶ�����һ������
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;		
    }	
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return 1;
}
/*
��һ���ֽ�
*/
uint8_t Flash_Byte_Read(uint32_t Start_Address,uint8_t *Receive_DATA,int DATA_num)
{
	uint32_t uwAddress = 0;							//��ʼ��ȡ��ʼ��ַ
	uint32_t End_Address = 0;
	int num = 0;
  uwAddress = Start_Address;				//��ʼ��ȡ�ĵ�ַ
	End_Address = Start_Address + 1*DATA_num;		//������ַ
  while (uwAddress < End_Address)
  {
    Receive_DATA[num] = *(__IO uint32_t*)uwAddress;
		num++;
    uwAddress = uwAddress + 1;			//�����¸��ֵ�ͷ��ַ  Ҫ�ǰ���� +2 һ���ֽ�+1
  }  
	return 1;
}

#endif



