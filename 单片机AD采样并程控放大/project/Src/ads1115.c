/*****************************************************************************

STM32F405RCT6

By��Z.L.Y

******************************************************************************/

#include "ads1115.h"

char Channel_Value;
char ShowChannel[] = "No.- Channel:";

short D_ADS; 		//ת����������

unsigned int Config;
unsigned char Writebuff[4],Readbuff[3];
unsigned char Result[2]; 

int  Format[5]={0};  //ת����������ת����ʮ����

/*******************************************
Function name              :ADS1115_I2C_Start
Function use               :Start the I2C Bus of ADS1115
Function parameter         :None
Function return            :None
********************************************/
void ADS1115_I2C_Start(void)
{
	ADS1115_SCL_SET;       //SCL=1;
	ADS1115_SDA_SET;       //SDA=1;
	HAL_Delay(1);
	ADS1115_SDA_CLR;       //SDA=0;
	HAL_Delay(1);
	ADS1115_SCL_CLR;       //SCL=0;
	HAL_Delay(1);
}


/*******************************************
Function name              :ADS1115_I2C_Stop
Function use               :Stop the I2C Bus of ADS1115
Function parameter         :None
Function return            :None
********************************************/
void ADS1115_I2C_Stop(void)
{ 
    ADS1115_SDA_CLR;       //SDA=0;
    HAL_Delay(1);
    ADS1115_SCL_SET;       //SCL=1;
    HAL_Delay(1);
	ADS1115_SDA_SET;       //SDA=1;
    HAL_Delay(1);
}
/*******************************************
Function name              :ADS1115_I2C_Ack
Function use               :The answer of the I2C master
Function parameter         :None
Function return            :None
********************************************/
void ADS1115_I2C_Ack(void)
{
    ADS1115_SDA_CLR;       //SDA=0;
	HAL_Delay(1);
    ADS1115_SCL_SET;       //SCL=1;
    HAL_Delay(1);
    ADS1115_SCL_CLR;       //SCL=0;
	HAL_Delay(1);
	ADS1115_SDA_SET;       //SDA=1;
    HAL_Delay(1);
}


/*******************************************
Function name              :ADS1115_I2C_Nack
Function use               :The I2C master has no answer
Function parameter         :None
Function return            :None
********************************************/
void ADS1115_I2C_Nack(void)
{
	ADS1115_SDA_SET;       //SDA=1;
	HAL_Delay(1);
	ADS1115_SCL_SET;       //SCL=1;
	HAL_Delay(1);
	ADS1115_SCL_CLR;       //SCL=0;
	HAL_Delay(1);
	ADS1115_SDA_CLR;       //SDA=0;   
	HAL_Delay(1);
}



/*******************************************
Function name              :ADS1115_I2C_Check
Function use               :Check the answer of I2C slave
Function parameter         :None
Function return            :TRUE (1)--Slave has answer;
                            FALSE(0)--Slave has no answer
********************************************/
unsigned char ADS1115_I2C_Check(void)
{
    unsigned char slaveack;
    
    ADS1115_SDA_SET;       //SDA=1;
    HAL_Delay(1);
    ADS1115_SCL_SET;       //SCL=1;
    ADS1115_SDA_IN;	                    //Set SDA port as input.
    HAL_Delay(1);
    slaveack = SDA_IN;   		                //Read the status of SDA port.
    ADS1115_SCL_CLR;       //SCL=0;
    HAL_Delay(1);
	ADS1115_SDA_OUT;	                    //Set SDA port as output.
	
    if(slaveack)    return FALSE;
    else            return TRUE;
}

/*******************************************
Function name              :ADS1115_Write_1_Byte
Function use               :Write 1 byte to ADS1117
Function parameter         :DataByte--The byte you want to write to ADS1115
Function return            :TRUE (1)--Slave has answer;
                            FALSE(0)--Slave has no answer
********************************************/
void ADS1115_Write_1_Byte(unsigned char DataByte)
{
	int i;

	for(i=0;i<8;i++)
	{
		if(DataByte&0x80)  //if((DataByte<<i)&0x80)
		  ADS1115_SDA_SET;       //SDA=1;
		else
		  ADS1115_SDA_CLR;       //SDA=0;
        HAL_Delay(1);
		ADS1115_SCL_SET;         //SCL=1;
        HAL_Delay(1);
        ADS1115_SCL_CLR;         //SCL=0;
        HAL_Delay(1);
        DataByte <<= 1;
	}
    ADS1115_SDA_SET;             //SDA=1;
    HAL_Delay(1);
}



/*******************************************
Function name              :ADS1115_Write_N_Byte
Function use               :Write N bytes to ADS1117
Function parameter         :*writebuffer--The point of the array(unsigned char) you want to write to ADS1115
                                     num--The number of the bytes
Function return            :TRUE (1)--Slave has answer;
                            FALSE(0)--Slave has no answer
********************************************/
unsigned char ADS1115_Write_N_Byte(unsigned char *writebuffer,unsigned char num)
{
	int i;

	for(i=0;i<num;i++)
	{
		ADS1115_Write_1_Byte(*writebuffer);
		if(ADS1115_I2C_Check())
		{
			writebuffer ++;
		}
		else
		{
			ADS1115_I2C_Stop();
		    return FALSE;
		}
	}
	ADS1115_I2C_Stop();
	return TRUE;
}



/*******************************************
Function name              :ADS1115_Read_1_Byte
Function use               :Read a byte from ADS1117
Function parameter         :None
Function return            :data_Value -- The byte you read from ADS1115
********************************************/
unsigned char ADS1115_Read_1_Byte(void)
{
    unsigned char data_Value = 0, FLAG, i;
	
    for(i=0;i<8;i++)
    { 
        
        ADS1115_SDA_SET;         //SDA=1;
        HAL_Delay(1);
        ADS1115_SCL_SET;         //SCL=1;
        ADS1115_SDA_IN;
        HAL_Delay(1);
        FLAG=SDA_IN;
        data_Value <<= 1;
	    if(FLAG)
            data_Value |= 0x01;
        ADS1115_SDA_OUT;
        ADS1115_SCL_CLR;         //SCL=0;
        HAL_Delay(1);
    }
    return data_Value;
}

/*******************************************
Function name              :ADS1115_Read_N_Byte
Function use               :Read N bytes from ADS1117
Function parameter         :*readbuff -- The point of the array(unsigned char) where you'd like to store the bytes you read.
Function return            :data_Value -- The byte you read from ADS1115
********************************************/
void ADS1115_Read_N_Byte(unsigned char*readbuff, unsigned char num)
{
	unsigned char i;
	
	for(i=0;i<num;i++)
	{
		readbuff[i]=ADS1115_Read_1_Byte();
		if(i==num-1)
			ADS1115_I2C_Nack();  //read bytes uncontinously
		else 
			ADS1115_I2C_Ack();   //read bytes continously
	}
	ADS1115_I2C_Stop();

}


/*******************************************
Function name              :ADS1115_Init
Function use               :Init the ADS1115
Function parameter         :S_MUX_0,S_MUX_1  --  choose a single channel
Function return            :None
********************************************/
void ADS1115_Init(u8 S_MUX_0, u8 S_MUX_1)
{

	
	if (S_MUX_0 == 0 && S_MUX_1 == 0)
	Config = OS+MUX_A0+PGA+DR+COMP_QUE+MODE;
	if (S_MUX_0 == 0 && S_MUX_1 == 1) 
	Config = OS+MUX_A1+PGA+DR+COMP_QUE+MODE;
	if (S_MUX_0 == 1 && S_MUX_1 == 0)
	Config = OS+MUX_A2+PGA+DR+COMP_QUE+MODE;
	if (S_MUX_0 == 1 && S_MUX_1 == 1)
	Config = OS+MUX_A3+PGA+DR+COMP_QUE+MODE;
    
    Writebuff[0]=ADDRESS_W;
    Writebuff[1]=Pointer_1;
    Writebuff[2]=Config/256;
    Writebuff[3]=Config%256;
        
    Readbuff[0]=ADDRESS_W;
    Readbuff[1]=Pointer_0;
        
    Readbuff[2]=ADDRESS_R;
}



/*******************************************
Function name              :ADS1115_WriteWord
Function use               :Write words which is set in Writebuff to ADS1115.
Function parameter         :None
Function return            :None
********************************************/
void ADS1115_WriteWord(void)
{
     int t;
     ADS1115_I2C_Start();   
     do
     {
          t=ADS1115_Write_N_Byte(Writebuff,4);
     }while(t==0);                                  //Write 4 bytes in Writebuff to ADS1115.
}


/*******************************************
Function name              :ADS1115_ReadWord
Function use               :Read word and stored in Result from ADS1115.
Function parameter         :None
Function return            :None
********************************************/
void ADS1115_ReadWord(void)
{
    int t;
    ADS1115_I2C_Start();                           //Write 2 bytes.
    do
    {
      t=ADS1115_Write_N_Byte(Readbuff,2);
    }while(t==0);

    ADS1115_I2C_Start();                           //Write 2 bytes.
    do
    {
      t=ADS1115_Write_N_Byte(&Readbuff[2],1);
    }while(t==0);
    
    ADS1115_Read_N_Byte(Result,2);                 //Read 2 bytes.

}


/*******************************************
Function name              :ADS1115_Getdata
Function use               :Get data from ADS1115 in single channel mode.
Function parameter         :Channel              -- A single channel
Function return            :short D_ADS -- The data from ADS1115
********************************************/
short ADS1115_Getdata(u8 Channel)
{
	u8 S_MUX_0, S_MUX_1;

	switch(Channel)
	{
		case 0:
			S_MUX_0 = 0;
			S_MUX_1 = 0;
			Channel_Value = Channel + '0';
			break;
		case 1:
			S_MUX_0 = 0;
			S_MUX_1 = 1;
			Channel_Value = Channel + '0';
			break;
		case 2:
			S_MUX_0 = 1;
			S_MUX_1 = 0;
			Channel_Value = Channel + '0';
			break;
		case 3:
			S_MUX_0 = 1;
			S_MUX_1 = 1;
			Channel_Value = Channel + '0';
			break;
		default :
			break;
	}
    AD_START;
    HAL_Delay(1);
    ADS1115_Init(S_MUX_0, S_MUX_1);
    ADS1115_WriteWord();
    HAL_Delay(1); 
    ADS1115_ReadWord();
    HAL_Delay(1);
	AD_END;
    D_ADS=Result[0]*256+Result[1];  //ת����������

  	return D_ADS;
}





