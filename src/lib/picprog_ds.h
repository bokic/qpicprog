#ifndef PICPROG_DS_H
#define PICPROG_DS_H

#include <stdint.h>


extern void PICds_Clr_All();
extern void PICds_Set_MCLR();

extern void PICds_EnterICSP();
extern void PICds_ExitICSP();

extern void PICds_write32(uint32_t);

extern uint32_t PICds_SanityCheck();

extern void PICds_ReadConfigMem(uint16_t *);
extern void PICds_WriteConfigMem(uint16_t *);
extern void PICds_BuldErase();
extern void PICds_ReadC(uint16_t *, uint32_t, uint32_t);
extern void PICds_WriteC(uint16_t*, uint32_t);

extern uint16_t PICds_ReadAppID();

extern void LightUpLED();

#endif // PICPROG_DS_H
