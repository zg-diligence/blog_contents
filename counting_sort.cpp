/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.24
 */

#include <iostream>
#include <vector>
using namespace std;

/*计数排序

 *算法思想:
    假设n个输入元素都是在0到k区间内的一个整数,对每一个元素x,统计小于等于x的元素个数,
    包括其本身,利用这一信息可直接将元素输出到数组中的正确位置上.

 *算法流程:
    1、初始化辅助数组count[k],统计元素出现次数,如元素i用count[i]表示;
    2、对于每一个元素,统计小于等于其本身的元素个数,即count[i]+=count[i-1];
    3、反向扫描待排序数组,每扫描一项,将其存入临时数组dev中的第count[i]项,count[i]减一;
    4、拷贝回原数组,排序完成.

 *算法性能:
    时间O(k+n),空间O(k+n) 算法稳定
 */

void countingSort(vector<int> &arr)
{
    //找出最大值;
    if(arr.empty())return;
    int max = arr[0];
    for(int i=1; i != arr.size(); ++i)
        if(arr[i] > max)
            max = arr[i];

    //计数;
    vector<int> count(max+1,0);
    vector<int> dev(arr.size()+1);
    for(int i=0; i != arr.size(); ++i)
        count[arr[i]]++;
    for(int i=1; i <= max; ++i)
        count[i] += count[i-1];

    //将记录输出到正确位置,并复制回原数组;
    for(int i=arr.size()-1; i >= 0; --i)
        dev[count[arr[i]]--] = arr[i];
    for(int i=0; i != arr.size(); ++i)
        arr[i] = dev[i+1];
}