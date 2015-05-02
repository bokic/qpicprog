#include "qpicprog.h"

const unsigned char MCLR			=	0x40;// MCLR/VPP/THV  portokal
const unsigned char PGM				=	0x20;// RB3/PGM       crveno
const unsigned char PGM_MCLR                    =	0x60;
const unsigned char DATA			=	0x10;// RB7/PGD       plavo
// Za Data pazi da smenis vo LowLevel.cpp 136
const unsigned char CLOCK			=	0x04;// RB6/PGC       portokal

QPicProg::QPicProg()
{
}

QPicProg::QPicProg(quint16 p_Port)
{
    m_Port = p_Port;
}

bool QPicProg::Initport()
{
    bool ret;

    asm(
        "   mov 0x0378, %dx\n" // TODO: hardcoded address.
        "   add 0x0002, %dx\n"
        "   in %dx, %ax\n"
        "   or 0x20, %al\n"
        "   out %al, %dx\n"

        "   mov 0x00, %al\n"
        "   mov 0x0378, %dx\n" // TODO: hardcoded address.
        "   out %al, %dx\n"
        "   in %dx, %ax\n"
        "   cmp 0x00, %al\n"
        "   je try2\n"
    );

    ret = false;

    asm(
       "    jmp ok\n"
       "try2:\n"
       "    mov 0xFF, %al\n"
       "    out %al, %dx\n"
       "    in %dx, %al\n"
       "    cmp 0xFF, %al\n"
       "    je err\n"
    );

    ret = false;

    asm(
        "err:\n"
    );

    ret = true;

    asm(
        "ok:\n"
         "   mov 0x0378, %dx\n" // TODO: hardcoded address.
         "   add 0x0002, %dx\n"
         "   in %dx, %ax\n"
         "   and 0xDF, %al\n"
         "   out %al, %dx\n"
         "   mov 0x00, %al\n"
         "   mov 0x0378, %dx\n" // TODO: hardcoded address.
         "   out %al, %dx\n"
    );

    return ret;
}

void QPicProg::Data_Clr_All()
{
    asm(
         "   mov 0x00, %al\n"
         "   mov 0x0378, %dx\n" // TODO: hardcoded address.
         "   out %al, %dx\n"
    );
}

void QPicProg::Data_Set_PGM()
{
    asm(
         "   mov 0x00, %al\n"
         "   mov 0x0378, %dx\n" // TODO: hardcoded address.
         "   out %al, %dx\n"
    );
}

void QPicProg::Data_Clr_PGM()
{
}

void QPicProg::ToggleClock()
{
    asm(
         "   mov 0x00, %al\n"
         "   mov 0x0378, %dx\n" // TODO: hardcoded address.
         "   in %dx, %al\n"
         "   xor 0x04, %al\n" // TODO: hardcoded pin.
         "   out %al, %dx\n"
    );
}

void QPicProg::Data_SetData(quint8 p_Data)
{
}

quint8 QPicProg::Data_GetData()
{
    return 0;
}
