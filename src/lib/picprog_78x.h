#ifndef _LOWLEVEL_H
#define _LOWLEVEL_H

#include "advanced_delay.h"
#include "export.h"

#include <stdbool.h>
#include <stdint.h>

extern MY_EXPORT bool initport();

extern MY_EXPORT void PIC_Set_PGM();
extern MY_EXPORT void PIC_Clr_PGM();
extern MY_EXPORT void PIC_Clr_All();

extern MY_EXPORT void TolegateClock();

extern MY_EXPORT void PIC_Bulk_Erase_Setup1();
extern MY_EXPORT void PIC_Bulk_Erase_Setup2();
extern MY_EXPORT void PIC_Begin_Erase_Programming_Cycle();

extern MY_EXPORT void PIC_Load_Data_for_Program_Memory(uint16_t Param);
extern MY_EXPORT uint16_t PIC_Read_Data_from_Program_Memory();
extern MY_EXPORT void PIC_Load_Configuration(uint16_t Param);
extern MY_EXPORT void PIC_Increment_Address();
extern MY_EXPORT void PIC_Begin_Programming_Only_Cycle();
extern MY_EXPORT void PIC_Load_Data_for_Data_Memory(uint16_t Param);
extern MY_EXPORT uint8_t PIC_Read_Data_from_Data_Memory();
extern MY_EXPORT void PIC_Erase_Program();
extern MY_EXPORT void PIC_Erase_Data();
extern MY_EXPORT void PIC_Erase_Protected();

#endif
