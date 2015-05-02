#include "picprog_ds30.h"
#include "advanced_delay.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <ftd2xx.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>


MY_EXPORT void picprog_ds30_enter_icsp(PICPROG_HANDLE handle)
{
    picprog_clear_all(handle);
    adv_delaym(10);
    picprog_set_mclr(handle);
    adv_delaym(4);
    picprog_clear_mclr(handle);
    adv_delayu(10);
    picprog_set_mclr(handle);
    adv_delaym(4);

    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP
}

MY_EXPORT void picprog_ds30_exit_icsp(PICPROG_HANDLE handle)
{
}

MY_EXPORT uint16_t picprog_ds30_read_application_id(PICPROG_HANDLE handle)
{
}

MY_EXPORT void picprog_ds30_write32(PICPROG_HANDLE handle, uint32_t data)
{
}

MY_EXPORT void picprog_ds30_write16(PICPROG_HANDLE handle, uint16_t data)
{
}

MY_EXPORT uint8_t picprog_ds30_read_bit(PICPROG_HANDLE handle)
{
}

MY_EXPORT void picprog_ds30_wait_for_recieve_ready(PICPROG_HANDLE handle)
{
}

MY_EXPORT void picprog_ds30_read_buffer(PICPROG_HANDLE handle, void* buffer, uint32_t size)
{
}

MY_EXPORT uint32_t picprog_ds30_sanity_check(PICPROG_HANDLE handle)
{
}

MY_EXPORT void picprog_ds30_write_command(PICPROG_HANDLE handle, uint32_t command)
{
}

MY_EXPORT uint16_t picprog_ds30_read_command(PICPROG_HANDLE handle)
{
}

MY_EXPORT void picprog_ds30_bulk_erase(PICPROG_HANDLE handle)
{
}

MY_EXPORT void picprog_ds30_read_program(PICPROG_HANDLE handle, uint16_t* buffer, uint32_t address, uint32_t no_blocks)
{
}

MY_EXPORT void picprog_ds30_write_program(PICPROG_HANDLE handle, uint16_t *buffer, uint32_t address)
{
}

MY_EXPORT void picprog_ds30_read_config_memory(PICPROG_HANDLE handle, uint16_t *buffer)
{
}

MY_EXPORT void picprog_ds30_write_config_memory(PICPROG_HANDLE handle, uint16_t *buffer)
{
}

MY_EXPORT uint16_t picprog_ds30_read_app_id(PICPROG_HANDLE handle)
{
}
