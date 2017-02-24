/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.24
 */

#include <iostream>
#include <vector>
using namespace std;

/*归并排序
 *算法思想:
    将若干个有序序列逐步归并,最终得到一个有序序列.
 
 *算法流程:
    1、自底向上 非递归算法 将一个具有n个记录的待排序序列看成n个长度为1的有序序列,进行两两归并,
       得到[n/2]个长度为2的有序序列,再进行两两归并,得到[n/4]个长度为4的有序序列,重复
       步骤,直到得到一个长度为n的有序序列.
    2、自顶向下 分治算法(递归) 将待排序序列一分为二,分裂点mid=(low+high)/2,递归地对序列arr[low],...,arr[mid]
       和arr[mid+1],...,arr[high]进行归并排序,然后合并两个子序列.递归终止条件,子序列长度为1.

 *算法性能:
    时间O(nlogn),空间O(n) 算法稳定
 */

//二路归并
void merge(int start, int mid, int end, vector<int> &arrA, vector<int> &arrB)
{
    //注:此函数将合并段结果储存在arrB数组中;
    int i = start, j = mid + 1, k = start;
    while(i <= mid && j <= end)
        arrB[k++] = arrA[i] <= arrA[j] ? arrA[i++] : arrA[j++];
    while(i <= mid)
        arrB[k++] = arrA[i++];
    while(j <= end)
        arrB[k++] = arrA[j++];
}

//自底向上的非递归算法
void mergePass(int h, vector<int> &arrA, vector<int> &arrB)
{
    int i = 0;
    for(; i+2*h-1 < arrA.size(); i += 2*h)
        merge(i, i+h-1, i+2*h-1, arrA, arrB);
    if(i+h-1 < arrA.size()-1)  //剩下元素个数大于一段,不足两段;
        merge(i, i+h-1, arrA.size()-1, arrA, arrB);
    else                       //剩下元素个数不足一段,直接拷贝;
        for(; i!=arrA.size(); ++i)
            arrB[i] = arrA[i];
}

void mergeSort_1(vector<int> &arr)
{
    int h = 1;
    vector<int> dev(arr.size());
    while(h < arr.size())
    {
        mergePass(h,arr,dev); //从数组arr归并到数组dev;
        h *= 2;
        mergePass(h,dev,arr); //从数组dev归并到数组arr;
        h *= 2;
    }
}

//自顶向下的分治算法
void mergeSort_2(int low, int high, vector<int> &arrA, vector<int> &arrB)
{
    if(low < high)
    {
        int mid = low + (high - low) / 2;
        mergeSort_2(low, mid, arrA, arrB);
        mergeSort_2(mid+1, high, arrA, arrB);
        merge(low, mid, high, arrA, arrB);  //二路归并;
        for(int i = low; i <= high; ++i)    //拷贝回原数组;
            arrA[i] = arrB[i];
    }
}

void mergeSort_2(vector<int> &arr)
{
    vector<int> dev(arr.size());
    mergeSort_2(0, arr.size()-1, arr, dev);
}