/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.13
 */

#include <iostream>
#include <vector>
using namespace std;

/*直接选择排序

 *算法思想:
    每趟排序在当前待排序序列中选取关键字最小或最大的记录,添加到有序序列中.

 *算法流程:
    1、在待排序序列中找到最小（大）元素,存放到排序序列的末尾;
    2、重复步骤1,直到待排序序列只有一个元素,排序结束.

 *算法性能:
    时间O(n^2),空间O(1) 排序不稳定
 */
 
void selectionSort(int left, int right, vector<int> &arr)
{
    for(int i=left; i != right; ++i)
    {
        int minPos=i;
        int minValue=arr[i];
        for(int j = i + 1; j != right + 1; ++j)   //找出当前序列中的最小元素;
            if(arr[j] < minValue)
            {
                minPos = j;
                minValue = arr[j];
            }
        if(minPos!=i)    //将找到的最小元素放到已排序序列末端;
        {
            arr[minPos] = arr[i];
            arr[i] = minValue;
        }
    }
}

void selectionSort(vector<int> &arr)
{
    selectionSort(0, arr.size()-1, arr);
}
