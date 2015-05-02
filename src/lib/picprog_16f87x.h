#ifndef PICPROG_16F87X_H
#define PICPROG_16F87X_H

#include "advanced_delay.h"
#include "picprog.h"
#include "export.h"

#include <stdint.h>

extern MY_EXPORT void PIC16f87x_Bulk_Erase_Setup1(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Bulk_Erase_Setup2(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Begin_Erase_Programming_Cycle(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Load_Data_for_Program_Memory(PICPROG_HANDLE handle, uint16_t param);
extern MY_EXPORT uint16_t PIC16f87x_Read_Data_from_Program_Memory(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Load_Configuration(PICPROG_HANDLE handle, uint16_t param);
extern MY_EXPORT void PIC16f87x_Increment_Address(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Begin_Programming_Only_Cycle(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Load_Data_for_Data_Memory(PICPROG_HANDLE handle, uint16_t param);
extern MY_EXPORT uint8_t PIC16f87x_Read_Data_from_Data_Memory(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Erase_Program(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Erase_Data(PICPROG_HANDLE handle);
extern MY_EXPORT void PIC16f87x_Erase_Protected(PICPROG_HANDLE handle);

#endif // PICPROG_16F87X_H
