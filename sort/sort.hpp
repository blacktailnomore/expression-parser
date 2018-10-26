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

//插入.直接
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
			//arr[index + 1] = arr[index];
			::swap(arr[index + 1], arr[index]);
			index--;
		}
		//arr[index + 1] = tmp;
	}
}

/*快速
	1. 任选一个数作为基数，比他小的放他左边，反之右边
	2. 分别对两个部分重复操作直到有序
*/
template<typename T>
void quickSort(T arr[], int left, int right)
{
	if (left >= right) return;

	int index = left;

	int leftTmp = left + 1;
	int rightTmp = right;
	while (leftTmp != rightTmp)
	{	
		while (leftTmp != rightTmp)
		{
			if (rightTmp > leftTmp
				&& arr[rightTmp] < arr[index])
			{
				::swap(arr[rightTmp], arr[index]);
				index = rightTmp;
				break;
			}
			rightTmp--;
		}
		while (leftTmp != rightTmp)
		{
			if (leftTmp < rightTmp
				&& arr[leftTmp] > arr[index])
			{
				::swap(arr[leftTmp], arr[index]);
				index = leftTmp;
				break;
			}
			leftTmp++;
		}
	}
	quickSort(arr, left, rightTmp - 1);
	quickSort(arr, rightTmp + 1, right);
}

/*
希尔
1.从下标0开始按一定增量分组，对这些组分别插入排序
->比如说：43，3，32，12，7，8，3
		  增量=7/2,分为[43,12],[3,7],[32,8],[3]
		  并不是真的分组，只是把这两个下标(0,0+7/2)看成是一组
*/



/*
堆排
*/


/*
归并
1.拆分成根号n组，分别排序
2.两两合并，如果剩下的组数没有达到1就继续合并到1
*/




