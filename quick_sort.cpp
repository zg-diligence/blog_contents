/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.13
 */

#include <iostream>
#include <vector>
using namespace std;

/*快速排序

 *思考改进:
    快速排序算法是对冒泡排序的改进,在冒泡排序中,元素的比较和移动都是在相邻单元中进行的,
    因此可通过减少总的比较次数和移动次数,增大记录的比较和移动距离,提高算法性能.

 *算法思想:
    分治策略,以减少排序过程中的比较次数.
    分解:将原问题分解为若干个与原问题形似的子问题;
    求解:递归地求解子问题,若子问题规模足够小,则直接求解子问题;
    组合:将每一个子问题的解组合成原问题的解.

 *算法流程:
    1、选取基准元素;
    2、划分:比基准元素小的放在左边,大于等于放在右边;
    3、对左右区间递归执行步骤1、2,直至各区间只有一个元素.
 
 *基准元素选取:
    方法1:取序列最左端元素;
    方法2:从序列arr[i],...,arr[j]中,将arr[i],arr[(i+j)/2],arr[j]排序,返回中值;
    方法3:从序列arr[i],...,arr[j]中,选择最先找到的两个不同关键字中的较大者.若无两个关键字不同,则已经有序.
 
 *算法性能:
    时间O(nlogn),空间O(logn) 算法不稳定--平均性能

 *优化:
    1、当初始序列是非递减序列时,快排性能下降到最坏情况O(n^2),基准元素策略采用方法2;
    2、在规模较小的情况下,采用直接插入排序.
 */

void mySwap(int i, int j, vector<int> &arr)
{
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

//基准元素选取,方法3;
int findPivot_1(int i, int j, const vector<int> &arr)
{
    for(int k=i+1; k <= j; ++k)
        if(arr[k] > arr[i])
            return k;
        else if(arr[k] < arr[i])
            return i;
    return -1; //排序已完成;
}

//基准元素选取,方法2;
int findPivot_2(int i, int j, vector<int> &arr)
{
    int mid = (i + j) / 2;
    if(arr[i] > arr[j])
        mySwap(i, j, arr);
    if(arr[mid] > arr[j])
        mySwap(mid, j, arr);
    if(arr[i] > arr[mid])
        mySwap(i, mid, arr);
    return mid;
}

//划分元素,小于放在左边,大于等于放在右边;
int partition(int i, int j, int pivot, vector<int> &arr)
{
    int left=i, right=j;
    while(left <= right)
    {
        while(arr[left] < pivot)left++;
        while(arr[right] >= pivot)right--;
        if(left < right)
        {
            int tmp = arr[right];
            arr[right] = arr[left];
            arr[left] = tmp;
        }
    }
}

void quickSort(int i, int j, vector<int> &arr)
{
    int pivotPos = findPivot_1(i, j, arr);
    if(pivotPos != -1)
    {
        int pivot = arr[pivotPos];
        int k = partition(i, j, pivot, arr);
        quickSort(i, k-1, arr);
        quickSort(k, j, arr);
    }
}

//改进后的快速排序算法;
void quickSort_imp(int i, int j, vector<int> &arr)
{
    if(j - i <= 5)
        selectionSort(i, j, arr);       //序列规模小,采用直接插入排序;
    else
    {
        int pivot = arr[findPivot_2(i, j, arr)];    //选取基准元素;
        int k = partition(i, j, pivot, arr);   //划分元素;
        quickSort_imp(i, k-1, arr);   //对左右序列递归求解问题;
        quickSort_imp(k, j, arr);
    }
}

void quickSort(vector<int> &arr)
{
    //quickSort(0,arr.size()-1,arr);
    quickSort_imp(0,arr.size()-1,arr);
}