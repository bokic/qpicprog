#include <picprog.h>

#include <assert.h>

#define PICPROG_MCLR_SET               0x01
#define PICPROG_DATA_SET               0x02
#define PICPROG_CLOCK_SET              0x04

#define PICPROG_MCLR_CLEAR             0x00
#define PICPROG_DATA_CLEAR             0x00
#define PICPROG_CLOCK_CLEAR            0x00

#define PICPROG_MCLR_OUTPUT_DIRECTION  0x10
#define PICPROG_DATA_OUTPUT_DIRECTION  0x20
#define PICPROG_CLOCK_OUTPUT_DIRECTION 0x40

#define PICPROG_DATA_GET               PICPROG_DATA_SET

#define PICPROG_SET_MCLR(val)     (((val) | PICPROG_MCLR_OUTPUT_DIRECTION) | PICPROG_MCLR_SET)
#define PICPROG_SET_DATA(val)     (((val) | PICPROG_DATA_OUTPUT_DIRECTION) | PICPROG_DATA_SET)
#define PICPROG_SET_CLOCK(val)    (((val) | PICPROG_CLOCK_OUTPUT_DIRECTION) | PICPROG_CLOCK_CLEAR)
#define PICPROG_CLEAR_MCLR(val)   (((val) | PICPROG_MCLR_OUTPUT_DIRECTION) & (~PICPROG_MCLR_SET))
#define PICPROG_CLEAR_DATA(val)   (((val) | PICPROG_DATA_OUTPUT_DIRECTION) & (~PICPROG_DATA_SET))
#define PICPROG_CLEAR_CLOCK(val)  (((val) | PICPROG_CLOCK_OUTPUT_DIRECTION) & (~PICPROG_CLOCK_SET))
#define PICPROG_TOGGLE_CLOCK(val) ((val) ^ PICPROG_CLOCK_SET)

#define PICPROG_SET_DATA_AS_INPUT(val) ((val) & (~PICPROG_DATA_OUTPUT_DIRECTION))

#define FT_CAST(val) ((struct FT_PicProgHandle *)val)

#include <ftd2xx.h>

struct FT_PicProgHandle
{
    uint8_t start;
    bool is_first_command;
    FT_HANDLE ft_handle;
    uint8_t ft_port_status;
    uint8_t end;
};

MY_EXPORT PICPROG_HANDLE picprog_open()
{
    PICPROG_HANDLE ret = NULL;
    FT_HANDLE ft_handle;
    if (FT_OpenEx("PicProg", FT_OPEN_BY_DESCRIPTION, &ft_handle) == FT_OK)
    {
        ret = malloc(sizeof(struct FT_PicProgHandle));

        FT_CAST(ret)->start = '+';
        FT_CAST(ret)->is_first_command = true;
        FT_CAST(ret)->ft_handle = ft_handle;
        FT_CAST(ret)->ft_port_status = 0;
        FT_CAST(ret)->end = '-';
    }

    return ret;
}

MY_EXPORT void picprog_close(PICPROG_HANDLE handle)
{
    if (handle != NULL)
    {
        assert(FT_CAST(handle)->start == '+');
        assert(FT_CAST(handle)->end == '-');

        FT_Close(FT_CAST(handle)->ft_handle);
        free(handle);
    }
}

MY_EXPORT void picprog_set_all_as_input(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = 0x00;
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_clear_all(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_CLEAR_MCLR(FT_CAST(handle)->ft_port_status);
    FT_CAST(handle)->ft_port_status = PICPROG_CLEAR_DATA(FT_CAST(handle)->ft_port_status);
    FT_CAST(handle)->ft_port_status = PICPROG_CLEAR_CLOCK(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_set_mclr(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_SET_MCLR(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_clear_mclr(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_CLEAR_MCLR(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_set_data(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_SET_DATA(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_clear_data(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_CLEAR_DATA(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

extern MY_EXPORT uint8_t picprog_read_data_bit(PICPROG_HANDLE handle)
{
    UCHAR ch;
    FT_GetBitMode(FT_CAST(handle)->ft_handle, &ch);

    if (ch & PICPROG_DATA_GET)
    {
        return 1;
    }

    return 0;
}

MY_EXPORT void picprog_set_data_pin_as_input(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_SET_DATA_AS_INPUT(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_set_data_pin_as_output(PICPROG_HANDLE handle)
{
    picprog_set_data(handle);
}

MY_EXPORT void picprog_toggle_clock(PICPROG_HANDLE handle)
{
    FT_CAST(handle)->ft_port_status = PICPROG_TOGGLE_CLOCK(FT_CAST(handle)->ft_port_status);
    FT_SetBitMode(FT_CAST(handle)->ft_handle, FT_CAST(handle)->ft_port_status, FT_BITMODE_CBUS_BITBANG);
}

MY_EXPORT void picprog_set_first_commmand(PICPROG_HANDLE handle, bool value)
{
    FT_CAST(handle)->is_first_command = value;
}

MY_EXPORT bool picprog_get_first_commmand(PICPROG_HANDLE handle)
{
    return FT_CAST(handle)->is_first_command;
}
