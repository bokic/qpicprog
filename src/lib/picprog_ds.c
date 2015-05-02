#include "picprog_ds.h"
#include "advanced_delay.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <stdint.h>
#include <stdbool.h>


void PICds_Clr_All()
{
}

void PICds_Set_MCLR()
{
}

void PICds_Set_Data()
{
}

void PICds_Clr_Data()
{
}

bool FirstCommand;

void PICds_EnterICSP()
{
	PICds_ExitICSP();
	PICds_Set_MCLR();
	PICds_Clr_All();
    adv_delayn(40);

	PICds_write32(0x4D434851);

    adv_delayn(40);

	PICds_Set_MCLR();
	
    adv_delaym(25);

	FirstCommand = true;
}

void PICds_ExitICSP()
{
	PICds_Set_MCLR();
    adv_delayn(100);
	PICds_Clr_All();
    adv_delaym(25);
}

void dsTolegateClock()
{
}

void PICds_write32(uint32_t data)
{
    int c;

    for(c = 0; c < 32; c++)
	{
		if (data & 0x80000000)
			PICds_Set_Data();
		else
			PICds_Clr_Data();

		dsTolegateClock();
		dsTolegateClock();

		data <<= 1;
	}
}

void PICds_write16(uint16_t data)
{
    int c;

    for(c = 0; c < 16; c++)
	{
        if (data & 0x8000)
			PICds_Set_Data();
		else
			PICds_Clr_Data();

		dsTolegateClock();
		dsTolegateClock();

		data <<= 1;
	}
}

uint8_t PICds_recieveBit()
{
    return 0;
}

void PICds_WaitForRecieveReady()
{
	for(;;)
	{
		//if (PICds_recieveBit() == 0)
			break;
	}
}

void PICds_readbuffer(void* buf, uint32_t size)
{
    uint16_t *word = (uint16_t *)buf;
    uint32_t c;
    int c2;

    for(c = 0; c < size; c++)
	{
        for(c2 = 0; c2 < 16; c2++)
		{
			dsTolegateClock();
			*word = *word | (PICds_recieveBit() << (15 - c2));
			dsTolegateClock();
		}

        adv_delayn(40);

		word++;
	}
}


uint32_t PICds_SanityCheck()
{
    uint32_t ret = 0x00000000;

	PICds_write16(0x1000);

	PICds_WaitForRecieveReady();
	
    adv_delayu(5);

	PICds_readbuffer(&ret, 2); // 2 words.

	return ret;
}

void PICds_WriteCommand(uint32_t command)
{
    int c;

	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
    adv_delayn(100);

	if (FirstCommand == true)
	{
		dsTolegateClock();dsTolegateClock();
		dsTolegateClock();dsTolegateClock();
		dsTolegateClock();dsTolegateClock();
		dsTolegateClock();dsTolegateClock();
		dsTolegateClock();dsTolegateClock();	
        adv_delayn(100);
		
		FirstCommand = false;
	}
	
    for(c = 0; c < 24; c++)
	{

	if ((command & 1) == 0)
		PICds_Clr_Data();
	else
		PICds_Set_Data();

	dsTolegateClock();
	dsTolegateClock();

	command >>= 1;
	}
    adv_delayn(100);
}

uint16_t PICds_ReadCommand()
{
    uint16_t ret = 0;
    int c;

	PICds_Set_Data();dsTolegateClock();PICds_Clr_Data();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
    adv_delayn(100);
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
	dsTolegateClock();dsTolegateClock();
    adv_delayn(100);

    for(c = 0; c < 16; c++)
	{
		ret >>= 1;

		dsTolegateClock();
		
		if (PICds_recieveBit() != 0)
			ret |= 0x8000;

		dsTolegateClock();
	}

    adv_delayn(100);

	return ret;
}

void PICds_BuldErase()
{
	// Step 1: Exit the Reset vector.
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// Step 2: Set the NVMCON to erase all program memory.
	PICds_WriteCommand(0x2404FA); // MOV #0x404F, W10
	PICds_WriteCommand(0x883B0A); // MOV W10, NVMCON

	// Step 3: Initiate the erase cycle.
	PICds_WriteCommand(0xA8E761); // BSET NVMCON, #WR
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP

    Sleep(330);

	// Step 4: Wait for Bulk Erase operation to complete and make sure WR bit is clear.
	for(;;)
	{
		PICds_WriteCommand(0x803B00); // MOV NVMCON, W0
		PICds_WriteCommand(0x883C20); // MOV W0, VISI
		PICds_WriteCommand(0x000000); // NOP
        uint16_t NVMCON = PICds_ReadCommand();
		PICds_WriteCommand(0x040200); // GOTO 0x200
		PICds_WriteCommand(0x000000); // NOP

		if ((NVMCON & 0x8000) == 0)
			break;
	}
}

void PICds_ReadC(uint16_t* buf, uint32_t addr, uint32_t size)
{
    uint32_t c;

	// Step 1: Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// Step2: Initialize TBLPAG and the read pointer (W6) for TBLRD instruction.
	PICds_WriteCommand(0x200000 | ((addr & 0x00FF0000) >> 12)); // MOV #<SourceAddress23:16>, W0
	PICds_WriteCommand(0x880190); // MOV W0, TBLPAG
	PICds_WriteCommand(0x200006 | ((addr & 0x0000FFFF) << 4)); // MOV #<SourceAddress15:0>, W6

    for(c = 0; c < size; c++)
	{
		// Step 3: Initialize the write pointer (W7) and store the next four locations of code memory to W0:W5.
		PICds_WriteCommand(0xEB0380); // CLR W7
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBA1B96); // TBLRDL [W6], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBADBB6); // TBLRDH.B [W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBADBD6); // TBLRDH.B [++W6], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBA1BB6); // TBLRDL [W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBA1B96); // TBLRDL [W6], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBADBB6); // TBLRDH.B [W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBADBD6); // TBLRDH.B [++W6], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBA0BB6); // TBLRDL [W6++], [W7]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP

		// Step 4: Output W0:W5 using the VISI register and REGOUT command.
		PICds_WriteCommand(0x883C20); // MOV W0, VISI
		PICds_WriteCommand(0x000000); // NOP
		*buf = PICds_ReadCommand();buf++;
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x883C21); // MOV W1, VISI
		PICds_WriteCommand(0x000000); // NOP
		*buf = PICds_ReadCommand();buf++;
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x883C22); // MOV W2, VISI
		PICds_WriteCommand(0x000000); // NOP
		*buf = PICds_ReadCommand();buf++;
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x883C23); // MOV W3, VISI
		PICds_WriteCommand(0x000000); // NOP
		*buf = PICds_ReadCommand();buf++;
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x883C24); // MOV W4, VISI
		PICds_WriteCommand(0x000000); // NOP
		*buf = PICds_ReadCommand();buf++;
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x883C25); // MOV W5, VISI
		PICds_WriteCommand(0x000000); // NOP
		*buf = PICds_ReadCommand();buf++;
		PICds_WriteCommand(0x000000); // NOP
	} // Step 5: Repeat step 4 until all desired code memory is read.

	// Step 6: Reset device internal PC.
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP
}

void PICds_WriteC(uint16_t *buf, uint32_t addr)
{
    int c;

	// Step 1: Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// Step 2: Set the NVMCON to program 64 instruction words.
	PICds_WriteCommand(0x24001A); // MOV #0x4001, W10
	PICds_WriteCommand(0x883B0A); // MOV W10, NVMCON

	// Step 3: Initialize the write pointer (W7) for TBLWT instruction.
	PICds_WriteCommand(0x200000 | ((addr & 0x00FF0000) >> 12)); // MOV #<DestinationAddress23:16>, W0
	PICds_WriteCommand(0x880190); // W0, TBLPAG
	PICds_WriteCommand(0x200007 | ((addr & 0x0000FFFF) << 4)); // MOV #<DestinationAddress15:0>, W7

    for(c = 0; c < 16; c++)
	{
		// Step 4: Initialize the read pointer (W6) and load W0:W5 with the next 4 instruction words to program.
		PICds_WriteCommand(0x200000 | (*buf << 4)); // MOV #<LSW0>, W0
		buf++;
		PICds_WriteCommand(0x200001 | (*buf << 4)); // MOV #<MSB1:MSB0>, W1
		buf++;
		PICds_WriteCommand(0x200002 | (*buf << 4)); // MOV #<LSW1>, W2
		buf++;
		PICds_WriteCommand(0x200003 | (*buf << 4)); // MOV #<LSW2>, W3
		buf++;
		PICds_WriteCommand(0x200004 | (*buf << 4)); // MOV #<MSB3:MSB2>, W4
		buf++;
		PICds_WriteCommand(0x200005 | (*buf << 4)); // MOV #<LSW3>, W5
		buf++;

		// Step 5: Set the read pointer (W6) and load the (next set of) write latches.
		PICds_WriteCommand(0xEB0300); // CLR W6
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBB0BB6); // TBLWTL[W6++], [W7]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBBDBB6); // TBLWTH.B[W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBBEBB6); // TBLWTH.B[W6++], [++W7]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBB1BB6); // TBLWTL[W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBB0BB6); // TBLWTL[W6++], [W7]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBBDBB6); // TBLWTH.B[W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBBEBB6); // TBLWTH.B[W6++], [++W7]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0xBB1BB6); // TBLWTL[W6++], [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
	} // Step 6: Repeat steps 4-5 sixteen times to load the write latches for 64 instructions.

	PICds_WriteCommand(0xA8E761); // BSET NVMCON, #WR
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP

    Sleep(2);

	for(;;) // Step 8: Wait for Row Program operation to complete and make sure WR bit is clear.
	{
		PICds_WriteCommand(0x803B00); // MOV NVMCON, W0
		PICds_WriteCommand(0x883C20); // MOV W0, VISI
		PICds_WriteCommand(0x000000); // NOP
        uint16_t NVMCON = PICds_ReadCommand();
		PICds_WriteCommand(0x040200); // GOTO 0x200
		PICds_WriteCommand(0x000000); // NOP

		if ((NVMCON & 0x8000) == 0)
			break;
	}
}

void PICds_ReadConfigMem(uint16_t *buf)
{
    int c;

	// Step 1: Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// Step 2: Initialize TBLPAG, the read pointer (W6) and the write pointer (W7) for TBLRD instruction.
	PICds_WriteCommand(0x200F80); // MOV #0xF8, W0
	PICds_WriteCommand(0x880190); // MOV W0, TBLPAG
	PICds_WriteCommand(0xEB0300); // CLR W6
	PICds_WriteCommand(0x207847); // MOV #VISI, W7
	PICds_WriteCommand(0x000000); // NOP

	// Step 3: Read the Configuration register and write it to the VISI register (located at 0x784) and clock out the VISI register using the REGOUT command.
    for(c = 0; c < 12; c++)
	{
		PICds_WriteCommand(0xBA0BB6); // TBLRDL [W6++], [W7]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP

		*buf = PICds_ReadCommand(); 

		buf++;

	}// Step 4: Repeat step 3 twelve times to read all the Configuration registers.

	// Step 5: Reset device internal PC.
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP
}

void PICds_WriteConfigMem(uint16_t *buf)
{
    int c;

	// Step 1: Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// Step 2: Initialize the write pointer (W7) for the TBLWT instruction.
	PICds_WriteCommand(0x200007); // MOV #0x0000, W7

	// Step 3: Set the NVMCON register to program one Configuration register.
	PICds_WriteCommand(0x24000A); // MOV #0x4000, W10
	PICds_WriteCommand(0x883B0A); // MOV W10, NVMCON

	// Step 4: Initialize the TBLPAG register.
	PICds_WriteCommand(0x200F80); // MOV #0xF8, W0
	PICds_WriteCommand(0x880190); // MOV W0, TBLPAG

    for(c = 0; c < 12; c++)
	{
		// Step 5: Load the Configuration register data to W6.
		PICds_WriteCommand(0x200000 | (*buf << 4)); // MOV #<CONFIG_VALUE>, W0

		// Step 6: Write the Configuration register data to the write latch and increment the write pointer.
		PICds_WriteCommand(0xBB1B80); // TBLWTL W0, [W7++]
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP

		// Step 7: Initiate the write cycle.
		PICds_WriteCommand(0xA8E761); // BSET NVMCON, #WR
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP
		PICds_WriteCommand(0x000000); // NOP

		// Step 8: Wait for the Configuration Register Write operation to complete and make sure WR bit is clear.
        Sleep(25);
		for(;;) // Step 8: Wait for Row Program operation to complete and make sure WR bit is clear.
		{
			PICds_WriteCommand(0x803B00); // MOV NVMCON, W0
			PICds_WriteCommand(0x883C20); // MOV W0, VISI
			PICds_WriteCommand(0x000000); // NOP
            uint16_t NVMCON = PICds_ReadCommand();
			PICds_WriteCommand(0x040200); // GOTO 0x200
			PICds_WriteCommand(0x000000); // NOP

			if ((NVMCON & 0x8000) == 0)
				break;
		}

		buf++;
	}
}

uint16_t PICds_ReadAppID()
{
	// Step 1: Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// Step 2: Initialize TBLPAG and the read pointer (W0) for TBLRD instruction.

	PICds_WriteCommand(0x200800); // MOV #0x80, W0
	PICds_WriteCommand(0x880190); // MOV W0, TBLPAG
	PICds_WriteCommand(0x205FE0); // MOV #0x5BE, W0
	PICds_WriteCommand(0x207841); // MOV #VISI, W1
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0xBA0890); // TBLRDL [W0], [W1]
	PICds_WriteCommand(0x000000); // NOP
	PICds_WriteCommand(0x000000); // NOP

	return PICds_ReadCommand();
}

void LightUpLED()
{
    Sleep(5000);

	// Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

	// void main()
	// TRISA = 0x0000;
	PICds_WriteCommand(0x200000); // MOV	#0, W0
	PICds_WriteCommand(0x881600); // MOV	W0, TRISA

	// LATA = 0xFFFF;
	PICds_WriteCommand(0x2FFFF0); // MOV	#65535, W0
	PICds_WriteCommand(0x881620); // MOV	W0, LATA

	// Exit Reset Vector
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x040200); // GOTO 0x200
	PICds_WriteCommand(0x000000); // NOP

    Sleep(1000);
}
