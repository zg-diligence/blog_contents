/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.24
 */

#include <iostream>
#include <vector>
using namespace std;

/*直接插入排序

 *算法思想:
    依次选取一个待排序的元素,在已排序序列中从后向前扫描,找到相应位置并插入.

 *算法流程：
    1、从第一个元素开始,该元素可认为已经排好序;
    2、取出待排序序列第一个元素,即已排序序列后一个元素;
    3、在已排序序列中从后往前扫,如果被扫描元素大于新元素,则将该元素后移一位;
    4、重复步骤3,直到找到一个小于等于目标值的元素,将元素插入在其后面;
    5、重复步骤2-4,直到待排序序列只有一个元素,则排序完成.

 *算法性能:
    时间O(n^2),空间O(1) 算法稳定

 *算法优化:
    直接插入排序每次寻找插入位置时,是按顺序依次往前找,可采用二分查找改进,即折半插入.
    折半插入的时间复杂度降为O(nlogn);
 */

//直接插入;
void insertSort(vector<int> &arr)
{
    for(int i=1; i!= arr.size(); ++i)
    {
        int j = i;
        int tmp =arr[i];
        for(; j !=0 && tmp < arr[j-1]; --j)
            arr[j] = arr[j-1];
        arr[j] = tmp;
    }
}

//折半插入;
void insertSort_imp(vector<int> &arr)
{
    for(int i=1; i != arr.size(); ++i)
        if(arr[i] < arr[i-1])
        {
            int tmp = arr[i];
            int low = 0, high = i-1, mid;
            while(low <= high)
            {
                mid = (high + low) / 2;
                if(tmp < arr[mid])
                    high = mid - 1;
                else
                    low = mid + 1;
            }
            for(int j = i; j != low; --j)
                arr[j] = arr[j-1];
            arr[low] = tmp;
        }
}