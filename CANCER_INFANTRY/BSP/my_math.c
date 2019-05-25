#include "my_math.h"

/*����һ��ʧ�ܵ��ٽ�ֵ������ ˵���׻�����ѧ��ѧ�� �˽�ѵ����������*/
//void Critical_Value_Treatment(float get, float *set,float mid)//�ٽ�ֵ����
//{
//	if((*set < mid) && (get > *set + mid))
//		*set = *set + mid * 2;
//	else if((*set > mid) && (get < *set - mid))
//		*set = *set - mid * 2;
//	else 
//		*set = *set;
//}

/**
	* @brief      �ٽ�ֵ����
	* @param[in]  range: ������(������)�ܷ�Χ ����β����֮��ľ���ֵ ��0~360��-180~180 range��Ϊ360
	* @param[in]  mid: ��Χ��ֵ��� ��0~360��midΪ180 ��-180~180��midΪ0
	* @note       �����ǸĽ����ٽ�ֵ������ ������һ����ڲ���
	*/
void Critical_Value_Treatment(float get, float *set,float mid,float range)//�ٽ�ֵ����
{
	if((*set < mid) && (get > *set + range / 2))
		*set = *set + range;
	else if((*set > mid) && (get < *set - range / 2))
		*set = *set - range;
	else 
		*set = *set;
}


void Moving_Average_Filter(float data_in, u8 num, float *data_out)//����ƽ��ֵ�˲���(num:ȡ������)
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

