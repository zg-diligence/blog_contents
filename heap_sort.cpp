/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.13
 */

#include <iostream>
#include <vector>
using namespace std;

/*堆排序

 *思考改进:
    堆排序是对直接选择排序的改进,直接选择排序每一次排序是找出待排序序列中的关键字值最小的记录,
    如果在此同时也找出关键字值较小的记录,则可以减少后面选择中所用的比较次数,从而提高效率.

 *算法思想:
    将待排序的记录序列用完全二叉树表示,然后构造最小堆或者最大堆,移除堆顶元素,
    再将剩余元素整理成堆,重复步骤直至堆中只有一个元素,则排序完成.

 *算法流程:
    1、将待排序的记录序列用完全二叉树表示;
    2、初始化建堆,依次对n/2,...,2,1,0元素执行pushDown操作;
    3、交换堆顶元素与下标最大的叶结点,将剩余n-1个元素整理成堆;
    4、重复执行步骤3,直至堆中只有一个元素,则排序完成.

 *算法性能:
    时间O(nlogn),空间O(1) 算法不稳定
 */

void pushDown(int first, int last, vector<int> &arr)
{
    int cur = first;
    int value = arr[first];
    while(cur <= last/2)
    {
        //将左右儿子中的较小者上移;
        int next = cur * 2;
        if(next < last && arr[next+1] < arr[next])  //有右儿子且右儿子小于左儿子;
            next++;
        if(value <= arr[next])break;
        arr[cur] = arr[next];
        cur = next;
    }
    arr[cur] = value;
}

void heapSort(vector<int> &arr)
{
    vector<int> dev(arr.size()+1);      //个人习惯从下标为1开始建堆,这不是必须的操作,故不计此开销;
    for(int i=1; i!=arr.size()+1; ++i)  //建立完全二叉树;
        dev[i]=arr[i-1];

    for(int i=arr.size()/2; i>=1; --i)  //建立最小堆;
        pushDown(i,arr.size(),dev);

    for(int i=arr.size(); i>=2; --i)    //交换元素,将剩余元素整理成堆;
    {
        int tmp = dev[1];
        dev[1] = dev[i];
        dev[i] = tmp;
        pushDown(1,i-1,dev);
    }

    for(int i=1; i!=arr.size()+1; ++i)  //复制回初始数组;
        arr[i-1]=dev[i];
}