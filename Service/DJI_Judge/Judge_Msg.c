#include "Judge_Msg.h"



uint8_t* DJI_Judge_Student_Msg_Create(uint16_t Data_Len,uint16_t Student_Cmdid,uint16_t Target_ID)
{
		//结构体后面的空间用于存放数据,数据末尾之后的一个字节用于存放数据长度
		uint16_t Raw_Data_Len = sizeof(Judge_Student_Msg_t)+Data_Len;
		uint8_t* Raw_Data = pvPortMalloc(Raw_Data_Len);
		if(!Raw_Data)
			return NULL;
		Judge_Student_Msg_t* Judge_Student_Msg = (Judge_Student_Msg_t*)Raw_Data;
		Judge_Student_Msg->Student_Msg_Data_Len = Data_Len;
		Judge_Student_Msg->Student_Msg_Header.data_cmd_id = Student_Cmdid;
		Judge_Student_Msg->Student_Msg_Header.receiver_ID = Target_ID;
		Judge_Student_Msg->Student_Msg_Header.sender_ID = DJI_Judge_Msg.DJI_Judge_Info.Judge_Robot_ID;
		return (uint8_t*)((uint32_t)Raw_Data+sizeof(Judge_Student_Msg_t));
}

void DJI_Judge_Student_Msg_Send(uint8_t* Student_Msg_Data)
{
		uint8_t* Raw_Data = Student_Msg_Data -  sizeof(Judge_Student_Msg_t);
		Judge_Student_Msg_t* Judge_Student_Msg = (Judge_Student_Msg_t*)Raw_Data;
		uint8_t Data_Len = Judge_Student_Msg->Student_Msg_Data_Len;
		DJI_Judge_Data_Send((uint8_t*)&Judge_Student_Msg->Student_Msg_Header,Data_Len,0x301);
		vPortFree(Raw_Data);
}

void DJI_Judge_Student_Msg_Receive()
{
		
}

void DJI_Judge_Msg_Init()
{
		DJI_Judge_Msg.Judge_Student_Msg_Receive_Buffer = xMessageBufferCreate(DJI_JUDGE_STUDENT_MSG_DATA_MAX_LEN+DJI_JUDGE_STUDENT_MSG_HEADER_LEN);
		DJI_Judge_Msg.Judge_Student_Msg_Send_Buffer = xMessageBufferCreate(DJI_JUDGE_STUDENT_MSG_DATA_MAX_LEN+DJI_JUDGE_STUDENT_MSG_HEADER_LEN);
}


DJI_Judge_Info_t* Get_Judge_Info(void)
{
	return &DJI_Judge_Msg.DJI_Judge_Info;
}