#ifndef PICPROG_H
#define PICPROG_H

#include "export.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef WIN32
#include <windows.h>
#endif


typedef void* PICPROG_HANDLE;

extern MY_EXPORT PICPROG_HANDLE picprog_open();
extern MY_EXPORT void picprog_close(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_clear_all(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_set_mclr(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_clear_mclr(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_set_data(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_clear_data(PICPROG_HANDLE handle);
extern MY_EXPORT uint8_t picprog_read_data_bit(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_set_data_pin_as_input(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_set_data_pin_as_output(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_toggle_clock(PICPROG_HANDLE handle);

extern MY_EXPORT void picprog_set_first_commmand(PICPROG_HANDLE handle, bool value);
extern MY_EXPORT bool picprog_get_first_commmand(PICPROG_HANDLE handle);

#endif // PICPROG_H
