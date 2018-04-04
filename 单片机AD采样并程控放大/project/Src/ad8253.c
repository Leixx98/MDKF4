#include "ad8253.h"

//AD8253增益控制函数
void AD8253_Gain_1(void)
{
    A0(0);A1(0);
}

void AD8253_Gain_10(void)
{
    A0(1);A1(0);
}

void AD8253_Gain_100(void)
{
    A0(0);A1(1);
}

void AD8253_Gain_1000(void)
{
    A0(1);A1(1);
}




