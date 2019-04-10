#include "my_math.h"

/**
	* @brief      �ٽ�ֵ����
	* @param[in]  range: ������(������)�ܷ�Χ ����β����֮��ľ���ֵ ��0~360��-180~180 range��Ϊ360
	* @param[in]  mid: ��Χ��ֵ��� ��0~360��midΪ180 ��-180~180��midΪ0
	* @note       �����ǸĽ����ٽ�ֵ������ ������һ����ڲ���
	*/
void critical_value_treatment(float get, float *set,float mid,float range)//�ٽ�ֵ����
{
	if((*set < mid) && (get > *set + range / 2))
		*set = *set + range;
	else if((*set > mid) && (get < *set - range / 2))
		*set = *set - range;
	else 
		*set = *set;
}

void moving_average_filter(float data_in, u8 num, float *data_out)//����ƽ��ֵ�˲���(num:ȡ������)
{
	u8 i;
	float buf[num+1];
	float sum = 0;

	buf[num] = data_in;
	for(i = 0; i < num; i++)
	{
		buf[i] = buf[i+1];
		sum += buf[i];
	}
	*data_out = sum / num;
}

//�޷�
float constrain(float amt, float low, float high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}
 
int16_t constrain_int16(int16_t amt, int16_t low, int16_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int32_t constrain_int32(int32_t amt, int32_t low, int32_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int constrain_int(int amt,int low,int high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}


