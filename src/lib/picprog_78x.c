#include "picprog_78x.h"
#include "advanced_delay.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


MY_EXPORT bool initport()
{
    return true;
}

MY_EXPORT void PIC_Set_PGM()
{
}

MY_EXPORT void PIC_Clr_PGM()
{
}

MY_EXPORT void PIC_Clr_All()
{
}

MY_EXPORT void TolegateClock()
{
}

void SetDATA(uint8_t param)
{
}

MY_EXPORT uint8_t GetDATA()
{
    return 0;
}

MY_EXPORT void WriteCommand(uint8_t command)
{
    int bits;

    command <<= 1;

    adv_delayn(100);

    for(bits = 0; bits < 6; bits++)
    {
        TolegateClock();
        command >>= 1;
        SetDATA(command);
        adv_delayn(100);

        TolegateClock();
        adv_delayn(100);
    }

    SetDATA(0);

    adv_delayu(1);
}

void WriteData(uint16_t data)
{
    int bits;

    data <<= 2;

    for(bits = 0; bits < 16; bits++)
    {
        TolegateClock();        
        data >>= 1;        
        SetDATA((uint8_t)data);        
        adv_delayn(100);

        TolegateClock();        
        adv_delayn(100);
    }

    SetDATA(0);

    adv_delayu(1);
}

MY_EXPORT uint16_t ReadData()
{
    int bits;
    uint16_t data=0;

    SetDATA(1);
    adv_delayn(100);

    for(bits=0;bits<16;bits++)
	{
        TolegateClock();
        adv_delayn(100);
        data >>= 1;
        if (GetDATA()==1)
        {
            data |= 0x8000;
        }

        TolegateClock();
        adv_delayn(100);
	}

    SetDATA(0);
    adv_delayu(1);

    return data;
}

MY_EXPORT void PIC_Load_Configuration(uint16_t Param)
{
    WriteCommand(0x00);
    WriteData(Param);
}

MY_EXPORT void PIC_Load_Data_for_Program_Memory(uint16_t Param)
{
    WriteCommand(0x02);
    WriteData(Param);
}


MY_EXPORT uint16_t PIC_Read_Data_from_Program_Memory()
{
    WriteCommand(0x04);
    return (uint16_t)((ReadData() >> 1) & 0x3FFF);
}

MY_EXPORT void PIC_Increment_Address()
{
    WriteCommand(0x06);
}

MY_EXPORT void PIC_Begin_Erase_Programming_Cycle()
{
    WriteCommand(0x08);
    adv_delaym(4);
}

MY_EXPORT void PIC_Begin_Programming_Only_Cycle()
{
    WriteCommand(0x18);
}

MY_EXPORT void PIC_Load_Data_for_Data_Memory(uint16_t Param)
{
    WriteCommand(0x03);
    WriteData(Param);
}

MY_EXPORT uint8_t PIC_Read_Data_from_Data_Memory()
{
    WriteCommand(0x05);
    return (uint8_t)((ReadData() >> 1) & 0x00FF);
}

void PIC_Bulk_Erase_Setup1()
{
    WriteCommand(0x01);
}

void PIC_Bulk_Erase_Setup2()
{
    WriteCommand(0x07);
}

void PIC_Erase_Program()
{
	PIC_Load_Data_for_Program_Memory(0x3FFF);
	PIC_Bulk_Erase_Setup1();
	PIC_Bulk_Erase_Setup2();
	PIC_Begin_Erase_Programming_Cycle();
    adv_delaym(10);

	PIC_Bulk_Erase_Setup1();
	PIC_Bulk_Erase_Setup2();
}

void PIC_Erase_Data()
{
	PIC_Load_Data_for_Data_Memory(0x3FFF);
	PIC_Bulk_Erase_Setup1();
	PIC_Bulk_Erase_Setup2();
	PIC_Begin_Erase_Programming_Cycle();
    adv_delaym(10);

	PIC_Bulk_Erase_Setup1();
	PIC_Bulk_Erase_Setup2();
}

void PIC_Erase_Protected()
{
	PIC_Load_Configuration(0x3FFF);
	PIC_Increment_Address();
	PIC_Increment_Address();
	PIC_Increment_Address();
	PIC_Increment_Address();
	PIC_Increment_Address();
	PIC_Increment_Address();
	PIC_Increment_Address();
    //PIC_Increment_Address(); // Don't know if it's needed?? By the spec it is needed, by petar's programmer(which works) is not needed..
	PIC_Bulk_Erase_Setup1();
	PIC_Bulk_Erase_Setup2();
	PIC_Begin_Erase_Programming_Cycle();
    adv_delaym(10);

	PIC_Bulk_Erase_Setup1();
	PIC_Bulk_Erase_Setup2();
}
