#include "picprog_16f87x.h"
#include "advanced_delay.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


MY_EXPORT void PIC16f87x_WriteCommand(PICPROG_HANDLE handle, uint8_t command)
{
    int bits;

    command <<= 1;

    adv_delayn(100);

    for(bits = 0; bits < 6; bits++)
    {
        picprog_toggle_clock(handle);
        command >>= 1;
        if (command & 0x01)
        {
            picprog_set_data(handle);
        }
        else
        {
            picprog_clear_data(handle);
        }
        adv_delayn(100);

        picprog_toggle_clock(handle);
        adv_delayn(100);
    }

    picprog_clear_data(handle);

    adv_delayu(1);
}

MY_EXPORT void PIC16f87x_WriteData(PICPROG_HANDLE handle, uint16_t data)
{
    int bits;

    data <<= 2;

    for(bits = 0; bits < 16; bits++)
    {
        picprog_toggle_clock(handle);
        data >>= 1;
        if (data & 0x01)
        {
            picprog_set_data(handle);
        }
        else
        {
            picprog_clear_data(handle);
        }
        adv_delayn(100);

        picprog_toggle_clock(handle);
        adv_delayn(100);
    }

    picprog_set_data(handle); // TODO: I thing here should be clear data instead.

    adv_delayu(1);
}

MY_EXPORT uint16_t PIC16f87x_ReadData(PICPROG_HANDLE handle)
{
    int bits;
    uint16_t data=0;

    picprog_set_data(handle);
    adv_delayn(100);
    picprog_set_data_pin_as_input(handle);
    adv_delayn(100);

    for(bits=0;bits<16;bits++)
	{
        picprog_toggle_clock(handle);
        adv_delayn(100);
        data >>= 1;
        if (picprog_read_data_bit(handle)==1)
        {
            data |= 0x8000;
        }

        picprog_toggle_clock(handle);
        adv_delayn(100);
	}

    picprog_set_data_pin_as_output(handle);
    adv_delayu(1);

    return data;
}

MY_EXPORT void PIC16f87x_Load_Configuration(PICPROG_HANDLE handle, uint16_t param)
{
    PIC16f87x_WriteCommand(handle, 0x00);
    PIC16f87x_WriteData(handle, param);
}

MY_EXPORT void PIC16f87x_Load_Data_for_Program_Memory(PICPROG_HANDLE handle, uint16_t param)
{
    PIC16f87x_WriteCommand(handle, 0x02);
    PIC16f87x_WriteData(handle, param);
}

MY_EXPORT uint16_t PIC16f87x_Read_Data_from_Program_Memory(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x04);
    return (uint16_t)((PIC16f87x_ReadData(handle) >> 1) & 0x3FFF);
}

MY_EXPORT void PIC16f87x_Increment_Address(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x06);
}

MY_EXPORT void PIC16f87x_Begin_Erase_Programming_Cycle(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x08);
    adv_delaym(4);
}

MY_EXPORT void PIC16f87x_Begin_Programming_Only_Cycle(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x18);
}

MY_EXPORT void PIC16f87x_Load_Data_for_Data_Memory(PICPROG_HANDLE handle, uint16_t param)
{
    PIC16f87x_WriteCommand(handle, 0x03);
    PIC16f87x_WriteData(handle, param);
}

MY_EXPORT uint8_t PIC16f87x_Read_Data_from_Data_Memory(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x05);
    return (uint8_t)((PIC16f87x_ReadData(handle) >> 1) & 0x00FF);
}

MY_EXPORT void PIC16f87x_Bulk_Erase_Setup1(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x01);
}

MY_EXPORT void PIC16f87x_Bulk_Erase_Setup2(PICPROG_HANDLE handle)
{
    PIC16f87x_WriteCommand(handle, 0x07);
}

MY_EXPORT void PIC16f87x_Erase_Program(PICPROG_HANDLE handle)
{
    PIC16f87x_Load_Data_for_Program_Memory(handle, 0x3FFF);
    PIC16f87x_Bulk_Erase_Setup1(handle);
    PIC16f87x_Bulk_Erase_Setup2(handle);
    PIC16f87x_Begin_Erase_Programming_Cycle(handle);
    adv_delaym(10);

    PIC16f87x_Bulk_Erase_Setup1(handle);
    PIC16f87x_Bulk_Erase_Setup2(handle);
}

MY_EXPORT void PIC16f87x_Erase_Data(PICPROG_HANDLE handle)
{
    PIC16f87x_Load_Data_for_Data_Memory(handle, 0x3FFF);
    PIC16f87x_Bulk_Erase_Setup1(handle);
    PIC16f87x_Bulk_Erase_Setup2(handle);
    PIC16f87x_Begin_Erase_Programming_Cycle(handle);
    adv_delaym(10);

    PIC16f87x_Bulk_Erase_Setup1(handle);
    PIC16f87x_Bulk_Erase_Setup2(handle);
}

MY_EXPORT void PIC16f87x_Erase_Protected(PICPROG_HANDLE handle)
{
    PIC16f87x_Load_Configuration(handle, 0x3FFF);
    PIC16f87x_Increment_Address(handle);
    PIC16f87x_Increment_Address(handle);
    PIC16f87x_Increment_Address(handle);
    PIC16f87x_Increment_Address(handle);
    PIC16f87x_Increment_Address(handle);
    PIC16f87x_Increment_Address(handle);
    PIC16f87x_Increment_Address(handle);
    //PIC16f87x_Increment_Address(handle); // Don't know if it's needed?? By the spec it is needed, by petar's programmer(which works) is not needed..
    PIC16f87x_Bulk_Erase_Setup1(handle);
    PIC16f87x_Bulk_Erase_Setup2(handle);
    PIC16f87x_Begin_Erase_Programming_Cycle(handle);
    adv_delaym(10);

    PIC16f87x_Bulk_Erase_Setup1(handle);
    PIC16f87x_Bulk_Erase_Setup2(handle);
}
