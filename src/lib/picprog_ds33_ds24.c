#include "picprog_ds33_ds24.h"
#include "advanced_delay.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <ftd2xx.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>


MY_EXPORT void picprog_ds33_ds24_enter_icsp(PICPROG_HANDLE handle)
{
    picprog_clear_mclr(handle);
    picprog_set_mclr(handle);
    picprog_clear_all(handle);
    adv_delayn(40);

    picprog_ds33_ds24_write32(handle, 0x4D434851);

    adv_delayn(40);

    picprog_set_mclr(handle);
	
    adv_delaym(25);

    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
}

MY_EXPORT void picprog_ds33_ds24_exit_icsp(PICPROG_HANDLE handle)
{
    picprog_clear_all(handle);
    adv_delaym(10);
    picprog_set_all_as_input(handle);
}

MY_EXPORT uint16_t picprog_ds33_ds24_read_application_id(PICPROG_HANDLE handle)
{
    // Step 1: Exit Reset Vector
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

    // Step 2: Initialize TBLPAG and the read pointer (W0) for TBLRD instruction.
    picprog_ds33_ds24_write_command(handle, 0x200800); // MOV #0x80, W0
    picprog_ds33_ds24_write_command(handle, 0x880190); // MOV W0, TBLPAG
    picprog_ds33_ds24_write_command(handle, 0x207F00); // MOV #0x7FO, W0
    picprog_ds33_ds24_write_command(handle, 0x207841); // MOV #VISI, W1
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0xBA0890); // TBLRDL [W0], [W1]
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

    // Step 3: Output the VISI register using the REGOUT command.
    return picprog_ds33_ds24_read_command(handle);
}

MY_EXPORT void picprog_ds33_ds24_write32(PICPROG_HANDLE handle, uint32_t data)
{
    int c;

    for(c = 0; c < 32; c++)
	{
		if (data & 0x80000000)
            picprog_set_data(handle);
		else
            picprog_clear_data(handle);

        picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);

		data <<= 1;
	}

    picprog_clear_data(handle);
}

MY_EXPORT void picprog_ds33_ds24_write16(PICPROG_HANDLE handle, uint16_t data)
{
    int c;

    for(c = 0; c < 16; c++)
	{
        if (data & 0x8000)
            picprog_set_data(handle);
		else
            picprog_clear_data(handle);

        picprog_toggle_clock(handle);
        picprog_toggle_clock(handle);

		data <<= 1;
	}

    picprog_clear_data(handle);
}

MY_EXPORT void picprog_ds33_ds24_read_buffer(PICPROG_HANDLE handle, void* buffer, uint32_t size)
{
    uint16_t *word = (uint16_t *)buffer;
    uint32_t c;
    int c2;

    picprog_set_data_pin_as_input(handle);
    adv_delayn(100);

    for(c = 0; c < size; c++)
	{
        for(c2 = 15; c2 >= 0; c2--)
		{
            picprog_toggle_clock(handle);
            *word = *word | ((picprog_read_data_bit(handle) & 0x01) << c2);
            picprog_toggle_clock(handle);
		}

        adv_delayn(40);

		word++;
	}

    picprog_set_data_pin_as_output(handle);
    adv_delayn(100);
}

MY_EXPORT uint32_t picprog_ds33_ds24_sanity_check(PICPROG_HANDLE handle)
{
    picprog_ds33_ds24_write_command(handle, 0x0001);

    uint16_t ret1 = picprog_ds33_ds24_read_command(handle);
    uint16_t ret2 = picprog_ds33_ds24_read_command(handle);

    return (uint32_t)(ret1 << 16) | ret2;


    /*uint32_t ret = 0x00000000;

    picprog_ds_write16(handle, 0x1000);

    picprog_ds_wait_for_recieve_ready(handle);
	
    adv_delayu(5);

    picprog_ds_read_buffer(handle, &ret, 2); // 2 words.

    return ret;*/
}

MY_EXPORT void picprog_ds33_ds24_write_command(PICPROG_HANDLE handle, uint32_t command)
{
    int c;

    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);picprog_toggle_clock(handle);
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

    picprog_clear_data(handle);

    adv_delayn(100);
}

MY_EXPORT uint16_t picprog_ds33_ds24_read_command(PICPROG_HANDLE handle)
{
    uint16_t ret = 0;
    int c;

    picprog_set_data(handle);picprog_toggle_clock(handle);picprog_toggle_clock(handle);
    picprog_clear_data(handle);picprog_toggle_clock(handle);picprog_toggle_clock(handle);
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
    adv_delayn(100);

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

MY_EXPORT void picprog_ds33_ds24_bulk_erase(PICPROG_HANDLE handle)
{
	// Step 1: Exit the Reset vector.
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step 2: Set the NVMCON to erase all program memory.
    picprog_ds33_ds24_write_command(handle, 0x2404FA); // MOV #0x404F, W10
    picprog_ds33_ds24_write_command(handle, 0x883B0A); // MOV W10, NVMCON

	// Step 3: Initiate the erase cycle.
    picprog_ds33_ds24_write_command(handle, 0xA8E761); // BSET NVMCON, #WR
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

    // Step 4: Wait for Bulk Erase operation to complete and make sure WR bit is clear.
    adv_delaym(350);

	for(;;)
	{
        picprog_ds33_ds24_write_command(handle, 0x803B00); // MOV NVMCON, W0
        picprog_ds33_ds24_write_command(handle, 0x883C20); // MOV W0, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        uint16_t NVMCON = picprog_ds33_ds24_read_command(handle);
        picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

		if ((NVMCON & 0x8000) == 0)
			break;
	}

    //adv_delaym(330);
}

/**
 * void picprog_ds33_ds24_read_program(PICPROG_HANDLE handle, uint16_t* buffer, uint32_t address, uint32_t no_blocks)
 *
 * @param handle - PicProg handle
 * @param buffer - Address of the buffer
 * @param address - Address in words
 * @param no_blocks - Number of 12byte blocks
 *
 * @return nothing
 *
 */
MY_EXPORT void picprog_ds33_ds24_read_program(PICPROG_HANDLE handle, uint16_t* buffer, uint32_t address, uint32_t no_blocks)
{
    uint32_t c;

	// Step 1: Exit Reset Vector
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step2: Initialize TBLPAG and the read pointer (W6) for TBLRD instruction.
    picprog_ds33_ds24_write_command(handle, 0x200000 | ((address & 0x00FF0000) >> 12)); // MOV #<SourceAddress23:16>, W0
    picprog_ds33_ds24_write_command(handle, 0x880190); // MOV W0, TBLPAG
    picprog_ds33_ds24_write_command(handle, 0x200006 | ((address & 0x0000FFFF) << 4)); // MOV #<SourceAddress15:0>, W6

    for(c = 0; c < no_blocks; c++)
	{
		// Step 3: Initialize the write pointer (W7) and store the next four locations of code memory to W0:W5.
        picprog_ds33_ds24_write_command(handle, 0xEB0380); // CLR W7
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBA1B96); // TBLRDL [W6], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBADBB6); // TBLRDH.B [W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBADBD6); // TBLRDH.B [++W6], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBA1BB6); // TBLRDL [W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBA1B96); // TBLRDL [W6], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBADBB6); // TBLRDH.B [W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBADBD6); // TBLRDH.B [++W6], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBA0BB6); // TBLRDL [W6++], [W7]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

		// Step 4: Output W0:W5 using the VISI register and REGOUT command.
        picprog_ds33_ds24_write_command(handle, 0x883C20); // MOV W0, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds33_ds24_read_command(handle);buffer++;
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x883C21); // MOV W1, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds33_ds24_read_command(handle);buffer++;
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x883C22); // MOV W2, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds33_ds24_read_command(handle);buffer++;
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x883C23); // MOV W3, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds33_ds24_read_command(handle);buffer++;
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x883C24); // MOV W4, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds33_ds24_read_command(handle);buffer++;
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x883C25); // MOV W5, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        *buffer = picprog_ds33_ds24_read_command(handle);buffer++;
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
	} // Step 5: Repeat step 4 until all desired code memory is read.

	// Step 6: Reset device internal PC.
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
}

/**
 *
 *    byteoffset 192 = 0x200807
 *    byteoffset 384 = 0x201007
 *    and so on.
 *
 */
MY_EXPORT void picprog_ds33_ds24_write_program(PICPROG_HANDLE handle, uint8_t *buffer, uint32_t address)
{
    int c;

	// Step 1: Exit Reset Vector
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step 2: Set the NVMCON to program 64 instruction words.
    picprog_ds33_ds24_write_command(handle, 0x24001A); // MOV #0x4001, W10
    picprog_ds33_ds24_write_command(handle, 0x883B0A); // MOV W10, NVMCON

	// Step 3: Initialize the write pointer (W7) for TBLWT instruction.
    picprog_ds33_ds24_write_command(handle, 0x200000 | ((address & 0x00FF0000) >> 12)); // MOV #<DestinationAddress23:16>, W0
    picprog_ds33_ds24_write_command(handle, 0x880190); // W0, TBLPAG
    picprog_ds33_ds24_write_command(handle, 0x200007 | ((address & 0x0000FFFF) << 4)); // MOV #<DestinationAddress15:0>, W7

    for(c = 0; c < 16; c++)
	{
		// Step 4: Initialize the read pointer (W6) and load W0:W5 with the next 4 instruction words to program.
        picprog_ds33_ds24_write_command(handle, 0x200000 | (buffer[0] << 4) | (buffer[1] << 12)); // MOV #<LSW0>, W0
        picprog_ds33_ds24_write_command(handle, 0x200001 | (buffer[2] << 4) | (buffer[6] << 12)); // MOV #<MSB1:MSB0>, W1
        picprog_ds33_ds24_write_command(handle, 0x200002 | (buffer[4] << 4) | (buffer[5] << 12)); // MOV #<LSW1>, W2
        picprog_ds33_ds24_write_command(handle, 0x200003 | (buffer[8] << 4) | (buffer[9] << 12)); // MOV #<LSW2>, W3
        picprog_ds33_ds24_write_command(handle, 0x200004 | (buffer[10] << 4) | (buffer[14] << 12)); // MOV #<MSB3:MSB2>, W4
        picprog_ds33_ds24_write_command(handle, 0x200005 | (buffer[12] << 4) | (buffer[13] << 12)); // MOV #<LSW3>, W5

		// Step 5: Set the read pointer (W6) and load the (next set of) write latches.
        picprog_ds33_ds24_write_command(handle, 0xEB0300); // CLR W6
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBB0BB6); // TBLWTL[W6++], [W7]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBBDBB6); // TBLWTH.B[W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBBEBB6); // TBLWTH.B[W6++], [++W7]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBB1BB6); // TBLWTL[W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBB0BB6); // TBLWTL[W6++], [W7]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBBDBB6); // TBLWTH.B[W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBBEBB6); // TBLWTH.B[W6++], [++W7]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xBB1BB6); // TBLWTL[W6++], [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

        buffer += (8 * 2);
	} // Step 6: Repeat steps 4-5 sixteen times to load the write latches for 64 instructions.

    picprog_ds33_ds24_write_command(handle, 0xA8E761); // BSET NVMCON, #WR
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

    adv_delaym(2);

	for(;;) // Step 8: Wait for Row Program operation to complete and make sure WR bit is clear.
	{
        picprog_ds33_ds24_write_command(handle, 0x803B00); // MOV NVMCON, W0
        picprog_ds33_ds24_write_command(handle, 0x883C20); // MOV W0, VISI
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        uint16_t NVMCON = picprog_ds33_ds24_read_command(handle);
        picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

		if ((NVMCON & 0x8000) == 0)
        {
			break;
        }
	}
}

MY_EXPORT void picprog_ds33_ds24_read_config_memory(PICPROG_HANDLE handle, uint16_t *buffer)
{
    int c;

	// Step 1: Exit Reset Vector
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step 2: Initialize TBLPAG, the read pointer (W6) and the write pointer (W7) for TBLRD instruction.
    picprog_ds33_ds24_write_command(handle, 0x200F80); // MOV #0xF8, W0
    picprog_ds33_ds24_write_command(handle, 0x880190); // MOV W0, TBLPAG
    picprog_ds33_ds24_write_command(handle, 0xEB0300); // CLR W6
    picprog_ds33_ds24_write_command(handle, 0x207847); // MOV #VISI, W7
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step 3: Read the Configuration register and write it to the VISI register (located at 0x784) and clock out the VISI register using the REGOUT command.
    for(c = 0; c < 12; c++)
	{
        picprog_ds33_ds24_write_command(handle, 0xBA0BB6); // TBLRDL [W6++], [W7]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

        *buffer = picprog_ds33_ds24_read_command(handle);
        buffer++;
        *buffer = 0;
        buffer++;

	}// Step 4: Repeat step 3 twelve times to read all the Configuration registers.

	// Step 5: Reset device internal PC.
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
}

MY_EXPORT void picprog_ds33_ds24_write_config_memory(PICPROG_HANDLE handle, uint16_t *buffer)
{
    int c;

	// Step 1: Exit Reset Vector
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step 2: Initialize the write pointer (W7) for the TBLWT instruction.
    picprog_ds33_ds24_write_command(handle, 0x200007); // MOV #0x0000, W7

	// Step 3: Set the NVMCON register to program one Configuration register.
    picprog_ds33_ds24_write_command(handle, 0x24000A); // MOV #0x4000, W10
    picprog_ds33_ds24_write_command(handle, 0x883B0A); // MOV W10, NVMCON

	// Step 4: Initialize the TBLPAG register.
    picprog_ds33_ds24_write_command(handle, 0x200F80); // MOV #0xF8, W0
    picprog_ds33_ds24_write_command(handle, 0x880190); // MOV W0, TBLPAG

    for(c = 0; c < 12; c++)
	{
		// Step 5: Load the Configuration register data to W6.
        picprog_ds33_ds24_write_command(handle, 0x200000 | (*buffer << 4)); // MOV #<CONFIG_VALUE>, W0

		// Step 6: Write the Configuration register data to the write latch and increment the write pointer.
        picprog_ds33_ds24_write_command(handle, 0xBB1B80); // TBLWTL W0, [W7++]
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

		// Step 7: Initiate the write cycle.
        picprog_ds33_ds24_write_command(handle, 0xA8E761); // BSET NVMCON, #WR
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

		// Step 8: Wait for the Configuration Register Write operation to complete and make sure WR bit is clear.
        adv_delaym(25);
		for(;;) // Step 8: Wait for Row Program operation to complete and make sure WR bit is clear.
		{
            picprog_ds33_ds24_write_command(handle, 0x803B00); // MOV NVMCON, W0
            picprog_ds33_ds24_write_command(handle, 0x883C20); // MOV W0, VISI
            picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
            uint16_t NVMCON = picprog_ds33_ds24_read_command(handle);
            picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
            picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

			if ((NVMCON & 0x8000) == 0)
				break;
		}

        buffer += 2;
	}
}

MY_EXPORT uint16_t picprog_ds33_ds24_read_app_id(PICPROG_HANDLE handle)
{
	// Step 1: Exit Reset Vector
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

	// Step 2: Initialize TBLPAG and the read pointer (W0) for TBLRD instruction.

    picprog_ds33_ds24_write_command(handle, 0x200800); // MOV #0x80, W0
    picprog_ds33_ds24_write_command(handle, 0x880190); // MOV W0, TBLPAG
    picprog_ds33_ds24_write_command(handle, 0x205FE0); // MOV #0x5BE, W0
    picprog_ds33_ds24_write_command(handle, 0x207841); // MOV #VISI, W1
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0xBA0890); // TBLRDL [W0], [W1]
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
    picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

    return picprog_ds33_ds24_read_command(handle);
}
