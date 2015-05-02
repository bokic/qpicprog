#ifndef PICPROG_DSPIC33_DS24_H
#define PICPROG_DSPIC33_DS24_H

#include "advanced_delay.h"
#include "picprog.h"
#include "export.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern MY_EXPORT void picprog_ds33_ds24_enter_icsp(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_exit_icsp(PICPROG_HANDLE handle);
extern MY_EXPORT uint16_t picprog_ds33_ds24_read_application_id(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_write32(PICPROG_HANDLE handle, uint32_t data);
extern MY_EXPORT void picprog_ds33_ds24_write16(PICPROG_HANDLE handle, uint16_t data);
extern MY_EXPORT uint8_t picprog_ds33_ds24_read_bit(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_wait_for_recieve_ready(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_read_buffer(PICPROG_HANDLE handle, void* buffer, uint32_t size);
extern MY_EXPORT uint32_t picprog_ds33_ds24_sanity_check(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_write_command(PICPROG_HANDLE handle, uint32_t command);
extern MY_EXPORT uint16_t picprog_ds33_ds24_read_command(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_bulk_erase(PICPROG_HANDLE handle);
extern MY_EXPORT void picprog_ds33_ds24_read_program(PICPROG_HANDLE handle, uint16_t *buffer, uint32_t address, uint32_t no_blocks);
extern MY_EXPORT void picprog_ds33_ds24_write_program(PICPROG_HANDLE handle, uint8_t *buffer, uint32_t address);
extern MY_EXPORT void picprog_ds33_ds24_read_config_memory(PICPROG_HANDLE handle, uint16_t *buffer);
extern MY_EXPORT void picprog_ds33_ds24_write_config_memory(PICPROG_HANDLE handle, uint16_t *buffer);
extern MY_EXPORT uint16_t picprog_ds33_ds24_read_app_id(PICPROG_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif // PICPROG_DSPIC33_DS24_H
