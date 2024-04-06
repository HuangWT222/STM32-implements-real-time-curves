#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "TIMER.h"

uint16_t ADValue;			//定义AD值变量
float Voltage;				//定义电压变量
float Vol[128];
int i;
int main(void)
{
	/*模块初始化*/
	OLED_Init();			//OLED初始化
	AD_Init();				//AD初始化
	
	
	for(i=0;i<128;i++)
	{
		ADValue = AD_GetValue();			//获取值
		Vol[i]= (float)ADValue / 4095 * 3.3;
		OLED_DrawPoint(i, 64-Vol[i]*19.09);  //
	}
	OLED_Update();
	Timer_Init();
	while (1)
	{

		
	}
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		OLED_Clear();
		for(i=0;i<127;i++) //将数据往前移并且显示出来
		{
			Vol[i]= Vol[i+1];
			if((Vol[i]-Vol[i+2])>0.5||(Vol[i]-Vol[i+2])<0.5)  //如果两个值相差太大，值绘制直线
			{
				OLED_DrawLine(i,64-Vol[i]*19.09,i,63-Vol[i+2]*19.09);
			}
			OLED_DrawPoint(i, 64-Vol[i]*19.09);
		}
		ADValue = AD_GetValue();
		Vol[127]=(float)ADValue / 4095 * 3.3;
		OLED_DrawPoint(127, Vol[127]);
		OLED_Update();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
