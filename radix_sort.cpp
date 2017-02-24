/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.24
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

/*基数排序

 *算法思想:
    通过比较关键字每个分量的值,对记录进行排序.

 *算法流程:(设每个关键字有figure个分量,基数为10)
    1、将待排序记录装入一个队列A,设置10个初始空队列vector<queue<int>> q;
    2、从队列A中取出每个数据data,第pass遍处理时,考察data.key右起第pass位数字,
       设其为r,把data插入队列q[r]中;
    3、从q[0]开始依次取出队列中的全部数据,并按照取出顺序插入到队列A中;
    4、重复步骤1-3,对于关键字中有figure位数字的数据进行figure遍处理,排序完成.

 *算法性能:
    设n为关键字个数,d为关键字分量个数,r为基数,若队列采用链表表示,则时间O(d*(n+r)),空间O(n+r) 算法稳定

 *算法优化:
    每一个桶中存放多少个关键字分量相同的记录是未知的,因此队列的长度难以确定,故队列采用链表表示,
    分别设置队首、队尾指针,方便于收集数据.
 */

//返回关键字对应位数据;
int radix(int value, int p)
{
    int power = 1;
    for(int i=0; i != p-1; ++i)
        power *= 10;
    return value/power % 10;
}

void radixSort(vector<int> &arr, int figure)
{
    //用队列保存;
    queue<int> A;
    for(auto num:arr)
        A.push(num);

    //基数排序主代码;
    for(int pass=1; pass <= figure; ++pass)
    {
        vector<queue<int>> q(10);
        while(!A.empty())
        {
            int data = A.front();A.pop();
            int r = radix(data,pass);
            q[r].push(data);
        }
        for(int i=0; i!=10; ++i)
            while(!q[i].empty())
            {
                A.push(q[i].front());
                q[i].pop();
            }
    }

    //写回原数组;
    for(int i=0; i != arr.size(); ++i)
    {
        arr[i] = A.front();
        A.pop();
    }
}