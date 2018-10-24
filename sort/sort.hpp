#pragma once


/*
���������㷨
*/

template<typename T>
void swap(T& a, T& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}

//ð��
template<typename T>
void bubbleSort(T arr[],int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = n - 1; j > i; --j)
		{
			if (arr[i] < arr[j - 1])
			{
				::swap(arr[i], arr[j - 1]);
			}
		}
	}
}

//ѡ��
template<typename T>
void selectSort(T arr[], int n)
{
	//ѡ���ĵ�n-1=>ѡ�ڶ���ĵ�n-2 �Դ�����(����С)
	for (int size = n - 1; size > 1; size--)
	{
		int max = size;
		for (int j = 0; j < size; ++j)
		{
			if (arr[max] < arr[j])
			{
				max = j;
			}
		}
		if (max != size)
			::swap(arr[max], arr[size]);
	}
}

//����.
template<typename T>
void insertSort(T arr[], int n)
{
	//�Եڶ���Ԫ����˵ ��һ��Ԫ������ ���бȽ�����
	//�Ե�����Ԫ����˵ һ��������Ԫ������ ���бȽ�����
	//...
	for (int i = 1; i < n; ++i)
	{
		int tmp = arr[i];
		int index = i - 1;
		while (index >= 0 && arr[index] > tmp)
		{
			arr[index + 1] = arr[index];
			index--;
		}
		arr[index + 1] = tmp;
	}
}

/*����
	1. ��ѡһ������Ϊ����������С�ķ�����ߣ���֮�ұ�
	2. �ֱ�����������ظ�����ֱ������
*/
template<typename T>
void quickSort(T arr[], int left, int right)
{
	T ele = arr[right];
	while (left < right)
	{

	}
}