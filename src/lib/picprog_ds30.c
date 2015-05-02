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
    picprog_set_first_commmand(handle, true);

    picprog_clear_all(handle);
    adv_delaym(100);
    picprog_set_mclr(handle);
    adv_delaym(4);
    picprog_clear_mclr(handle);
    adv_delayu(10);
    picprog_set_mclr(handle);
    adv_delaym(4);

    picprog_ds30_write_command(handle, 0x000000); // NOP
    //picprog_ds30_write_command(handle, 0x000000); // NOP
}

MY_EXPORT void picprog_ds30_exit_icsp(PICPROG_HANDLE handle)
{
    picprog_clear_all(handle);
    adv_delaym(10);
    picprog_set_all_as_input(handle);
}

MY_EXPORT uint16_t picprog_ds30_read_application_id(PICPROG_HANDLE handle)
{
    uint16_t ret;

    // Step 1: Exit Reset Vector
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x000000); // NOP

    // Step 2: Initialize TBLPAG and the read pointer (W0) for TBLRDinstruction.
    picprog_ds30_write_command(handle, 0x200800); // MOV #0x80, W0
    picprog_ds30_write_command(handle, 0x880190); // MOV W0, TBLPAG
    picprog_ds30_write_command(handle, 0x205BE0); // MOV #0x5BE, W0
    picprog_ds30_write_command(handle, 0x207841); // MOV VISI, W1
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0xBA0890); // TBLRDL [W0], [W1]
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP

    // Step 3: Output the VISI register using the REGOUT command.
    ret = picprog_ds30_read_command(handle);
    picprog_ds30_write_command(handle, 0x000000); // NOP

    return ret;
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
    int c;

    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);

    if (picprog_get_first_commmand(handle) == true)
    {
        picprog_toggle_clock(handle);picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);picprog_toggle_clock(handle);
        picprog_set_first_commmand(handle, false);
    }

    adv_delayn(100);

    for(c = 0; c < 24; c++)
    {
        if ((command & 1) == 0)
        {
            picprog_clear_data(handle);
        }
        else
        {
            picprog_set_data(handle);
        }

        picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);

        command >>= 1;
    }

    adv_delayn(20);
}

MY_EXPORT uint16_t picprog_ds30_read_command(PICPROG_HANDLE handle)
{
    uint16_t ret = 0;
    int c;

    picprog_set_data(handle);picprog_toggle_clock(handle);picprog_clear_data(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    adv_delayn(100);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    adv_delayn(100);
    picprog_set_data_pin_as_input(handle);

    for(c = 0; c < 16; c++)
    {
        ret >>= 1;

        picprog_toggle_clock(handle);

        if (picprog_read_data_bit(handle) != 0)
            ret |= 0x8000;

        picprog_toggle_clock(handle);
    }

    picprog_set_data_pin_as_output(handle);
    adv_delayn(100);

    return ret;
}

MY_EXPORT void picprog_ds30_bulk_erase(PICPROG_HANDLE handle)
{
    // Step 1: Exit the Reset vector.
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x000000); // NOP

    // TODO: Implement bulk erase for dsPIC30F5011/5013
    // Step 2: Set NVMCON to program the FBS Configuration register.
    //picprog_ds30_write_command(handle, 0x24008A); // MOV #0x4008, W10
    //picprog_ds30_write_command(handle, 0x883B0A); // MOV W10, NVMCON

    // Step 3: Initiate the erase cycle.
    //picprog_ds30_write_command(handle, 0xA8E761); // BSET NVMCON, #WR
    //picprog_ds30_write_command(handle, 0x000000); // NOP
    //picprog_ds30_write_command(handle, 0x000000); // NOP
    //picprog_ds30_write_command(handle, 0x000000); // NOP
    //picprog_ds30_write_command(handle, 0x000000); // NOP

    // ......


    // Step 9: Set the NVMCON to erase all Program Memory.
    picprog_ds30_write_command(handle, 0x2407FA); // MOV #0x407F, W10
    picprog_ds30_write_command(handle, 0x883B0A); // MOV W10, NVMCON

    // Step 10: Unlock the NVMCON for programming.
    picprog_ds30_write_command(handle, 0x200558); // MOV #0x55, W8
    picprog_ds30_write_command(handle, 0x883B38); // MOV W8, NVMKEY
    picprog_ds30_write_command(handle, 0x200AA9); // MOV #0xAA, W9
    picprog_ds30_write_command(handle, 0x883B39); // MOV W9, NVMKEY

    // Step 11: Initiate the erase cycle.
    picprog_ds30_write_command(handle, 0xA8E761); // BSET NVMCON, #WR
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP
    adv_delayu(1000);
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0xA9E761); // BCLR NVMCON, #WR
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP

    adv_delaym(330);
}

MY_EXPORT void picprog_ds30_read_program(PICPROG_HANDLE handle, uint16_t* buffer, uint32_t address, uint32_t no_blocks)
{
    int block;

    // Step 1: Exit Reset Vector
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x000000); // NOP

    // Step 2: Initialize TBLPAG and the read pointer (W6) for TBLRDinstruction.
    picprog_ds30_write_command(handle, 0x200000 | ((address & 0x00FF0000) >> 12)); // MOV #<DestinationAddress23:16>, W0
    picprog_ds30_write_command(handle, 0x880190); // W0, TBLPAG
    picprog_ds30_write_command(handle, 0x200006 | ((address & 0x0000FFFF) << 4)); // MOV #<DestinationAddress15:0>, W6

    for (block = 0; block < no_blocks; block++)
    {
        // Step 3: Initialize the write pointer (W7)and store the next four locations of code memory to W0:W5.
        picprog_ds30_write_command(handle, 0xEB0380); // CLR W7
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBA1B96); // TBLRDL [W6], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBADBB6); // TBLRDH.B [W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBADBD6); // TBLRDH.B [++W6], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBA1BB6); // TBLRDL [W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBA1B96); // TBLRDL [W6], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBADBB6); // TBLRDH.B [W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBADBD6); // TBLRDH.B [++W6], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBA0BB6); // TBLRDL [W6++], [W7]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP

        // Step 4: Output W0:W5 using the VISI register and REGOUT command.
        picprog_ds30_write_command(handle, 0x883C20); // MOV W0, VISI
        picprog_ds30_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds30_read_command(handle); buffer++;
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x883C21); // MOV W1, VISI
        picprog_ds30_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds30_read_command(handle); buffer++;
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x883C22); // MOV W2, VISI
        picprog_ds30_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds30_read_command(handle); buffer++;
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x883C23); // MOV W3, VISI
        picprog_ds30_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds30_read_command(handle); buffer++;
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x883C24); // MOV W4, VISI
        picprog_ds30_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds30_read_command(handle); buffer++;
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x883C25); // MOV W5, VISI
        picprog_ds30_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds30_read_command(handle); buffer++;
        picprog_ds30_write_command(handle, 0x000000); // NOP

        // Step 5: Reset device internal PC.
        picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
        picprog_ds30_write_command(handle, 0x000000); // NOP
    }
}

MY_EXPORT void picprog_ds30_write_program(PICPROG_HANDLE handle, uint16_t *buffer, uint32_t address)
{
    int c;

    // Step 1: Exit Reset Vector
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x000000); // NOP

    // Step 2: Set the NVMCON to program 32 instruction words.
    picprog_ds30_write_command(handle, 0x24001A); // MOV #0x4001, W10
    picprog_ds30_write_command(handle, 0x883B0A); // MOV W10, NVMCON

    // Step 3: Initialize the write pointer (W7) for TBLWT instruction.
    picprog_ds30_write_command(handle, 0x200000 | ((address & 0x00FF0000) >> 12)); // MOV #<DestinationAddress23:16>, W0
    picprog_ds30_write_command(handle, 0x880190); // W0, TBLPAG
    picprog_ds30_write_command(handle, 0x200007 | ((address & 0x0000FFFF) << 4)); // MOV #<DestinationAddress15:0>, W7

    for(c = 0; c < 8; c++)
    {
        // Step 4: Initialize the read pointer (W6) and load W0:W5 with the next 4 instruction words to program.
        uint32_t tt;
        tt = 0x200000 | (*buffer << 4);
        picprog_ds30_write_command(handle, 0x200000 | (*buffer << 4)); // MOV #<LSW0>, W0
        buffer++;
        tt = 0x200001 | (*buffer << 4);
        picprog_ds30_write_command(handle, 0x200001 | (*buffer << 4)); // MOV #<MSB1:MSB0>, W1
        buffer++;
        tt = 0x200002 | (*buffer << 4);
        picprog_ds30_write_command(handle, 0x200002 | (*buffer << 4)); // MOV #<LSW1>, W2
        buffer++;
        tt = 0x200003 | (*buffer << 4);
        picprog_ds30_write_command(handle, 0x200003 | (*buffer << 4)); // MOV #<LSW2>, W3
        buffer++;
        tt = 0x200004 | (*buffer << 4);
        picprog_ds30_write_command(handle, 0x200004 | (*buffer << 4)); // MOV #<MSB3:MSB2>, W4
        buffer++;
        tt = 0x200005 | (*buffer << 4);
        picprog_ds30_write_command(handle, 0x200005 | (*buffer << 4)); // MOV #<LSW3>, W5
        buffer++;

        // Step 5: Set the read pointer (W6) and load the (next set of) write latches.
        picprog_ds30_write_command(handle, 0xEB0300); // CLR W6
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBB0BB6); // TBLWTL[W6++], [W7]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBBDBB6); // TBLWTH.B[W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBBEBB6); // TBLWTH.B[W6++], [++W7]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBB1BB6); // TBLWTL[W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBB0BB6); // TBLWTL[W6++], [W7]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBBDBB6); // TBLWTH.B[W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBBEBB6); // TBLWTH.B[W6++], [++W7]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xBB1BB6); // TBLWTL[W6++], [W7++]
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
    } // Step 6: Repeat steps 4-5 eight times to load the write latches for 32 instructions.

    // Step 7: Unlock the NVMCON for writing.
    picprog_ds30_write_command(handle, 0x200558); // MOV #0x55, W8
    picprog_ds30_write_command(handle, 0x883B38); // MOV W8, NVMKEY
    picprog_ds30_write_command(handle, 0x200AA9); // MOV #0xAA, W9
    picprog_ds30_write_command(handle, 0x883B39); // MOV W9, NVMKEY

    // Step 8: Initiate the write cycle.
    picprog_ds30_write_command(handle, 0xA8E761); // BSET NVMCON, #WR
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP
    adv_delayu(1000);
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0xA9E761); // BCLR NVMCON, #WR
    picprog_ds30_write_command(handle, 0x000000); // NOP
    picprog_ds30_write_command(handle, 0x000000); // NOP

    // Step 9: Reset device internal PC.
    picprog_ds30_write_command(handle, 0x040100); // GOTO 0x100
    picprog_ds30_write_command(handle, 0x000000); // NOP
}

MY_EXPORT void picprog_ds30_read_config_memory(PICPROG_HANDLE handle, uint16_t *buffer)
{
}

MY_EXPORT void picprog_ds30_write_config_memory(PICPROG_HANDLE handle, uint16_t *buffer)
{
}
