#include <advanced_delay.h>
#include <picprog_ds.h>

#include <stdio.h>


void test_pins(PICPROG_HANDLE handle)
{
    picprog_clear_all(handle);

    picprog_set_mclr(handle);
    picprog_clear_mclr(handle);

    picprog_set_data(handle);
    picprog_clear_data(handle);

    picprog_toggle_clock(handle);
    picprog_toggle_clock(handle);
}

void LightUpLED(PICPROG_HANDLE handle)
{
    adv_delaym(5000);

    picprog_ds_enter_icsp(handle);

    uint16_t app_id = picprog_ds_read_app_id(handle);

    adv_delayn(100);

    uint16_t buffer[6];
    picprog_ds_read_program(handle, buffer, 0x0200, 1);

    // Exit Reset Vector
    picprog_ds_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds_write_command(handle, 0x000000); // NOP

    // void main()
    // TRISA = 0x0000;
    picprog_ds_write_command(handle, 0x200000); // MOV	#0, W0
    picprog_ds_write_command(handle, 0x881600); // MOV	W0, TRISA

    // LATA = 0xFFFF;
    picprog_ds_write_command(handle, 0x2FFFF0); // MOV	#65535, W0
    picprog_ds_write_command(handle, 0x881620); // MOV	W0, LATA

    // Exit Reset Vector
    picprog_ds_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds_write_command(handle, 0x040200); // GOTO 0x200
    picprog_ds_write_command(handle, 0x000000); // NOP

    picprog_ds_exit_icsp(handle);

    adv_delaym(1000);
}


int main(int argc, char *argv[])
{
    adv_delay_setup();

    PICPROG_HANDLE handle = picprog_open();

    if (handle == NULL)
    {
        printf("Couldn\'t open picprog programmer.\n");
        return -1;
    }

    //test_pins(handle);

    LightUpLED(handle);

    picprog_close(handle);

    return 0;
}
