#pragma once


/*
常用排序算法
*/

template<typename T>
void swap(T& a, T& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}

//冒泡
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

//选择
template<typename T>
void selectSort(T arr[], int n)
{
	//选最大的到n-1=>选第二大的到n-2 以此类推(或最小)
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

//插入.
template<typename T>
void insertSort(T arr[], int n)
{
	//对第二个元素来说 第一个元素有序 进行比较排序
	//对第三个元素来说 一、二两个元素有序 进行比较排序
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

/*快速
	1. 任选一个数作为基数，比他小的放他左边，反之右边
	2. 分别对两个部分重复操作直到有序
*/
template<typename T>
void quickSort(T arr[], int left, int right)
{
	T ele = arr[right];
	while (left < right)
	{

	}
}