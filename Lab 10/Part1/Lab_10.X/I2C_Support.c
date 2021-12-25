#include <stdio.h>

#include <p18f4620.h>
#include "I2C_Support.h"
#include "I2C.h"

#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02
#define ACK             1
#define NAK             0


extern unsigned char second, minute, hour, dow, day, month, year;
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;
extern unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;


void DS1621_Init()
{
 char Device = 0x48;
    I2C_Write_Cmd_Write_Data (Device, ACCESS_CFG, CONT_CONV);
    I2C_Write_Cmd_Only(Device, START_CONV);            
}


char DS1621_Read_Temp()             // With I2C, you always write first, even to read, you need to write a command to read
{                                   // After, the sequence will restart, and then Read, Hence the I2CRestart();
 char Device = 0x48;
 char Cmd = READ_TEMP; 
 char Data_Ret; 
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // Device address
  I2C_Write(Cmd);                   // Send register address
  I2C_ReStart();                    // Restart I2C
  I2C_Write((Device << 1) | 1);     // Initialize data read
  Data_Ret = I2C_Read(NAK);         //
  I2C_Stop(); 
  return Data_Ret;
}

void DS3231_Read_Time()
{
 char Device = 0x68; 
 char Address = 0x00; 
 char Data_Ret;
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // DS3231 address Write mode
  I2C_Write(Address);               // Send register address
  I2C_ReStart();                    // Restart I2C
  I2C_Write((Device << 1) | 1);     // Initialize data read
  
  second = I2C_Read(ACK);
  minute = I2C_Read(ACK);
  hour = I2C_Read(ACK);
  dow = I2C_Read(ACK);
  day = I2C_Read(ACK);
  month = I2C_Read(ACK);
  year = I2C_Read(NAK);
  
  I2C_Stop(); 
  
                                  
}

void DS3231_Setup_Time()
{
char Data_Ret;
char Device = 0x68;
char Address = 0x00;

    second =    0x00;
    minute =    0x00;
    hour   =    0x19;
    dow    =    0x05;
    day    =    0x28;
    month  =    0x10;
    year   =    0x21;
    
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // Device address Write mode
  I2C_Write(Address);               // Send register address
  
  I2C_Write(second);                // Initialize data read 
  I2C_Write(minute);                // Initialize data read 
  I2C_Write(hour);                  // Initialize data read 
  I2C_Write(dow);                   // Initialize data read 
  I2C_Write(day);                   // Initialize data read 
  I2C_Write(month);                 // Initialize data read 
  I2C_Write(year);                  // Initialize data read 
  
  I2C_Stop(); 
}

