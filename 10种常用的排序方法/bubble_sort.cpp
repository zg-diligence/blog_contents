/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.13
 */

#include <iostream>
#include <vector>
using namespace std;

/*冒泡排序

 *算法思想:
    将待排序的记录看做是竖着排列的气泡,关键字较小的记录比较轻,从而要往上浮.

 *算法流程:
    1、比较相邻的元素,如果下层元素小于上层元素,则交换;
    2、从底层向上,对每一对相邻元素重复步骤一,直到当前序列顶端;
    3、每一轮交换之后,序列元素除去顶端元素,对剩余序列元素重复以上操作;
    4、若当前序列只有一个元素,排序结束.

 *算法性能:
    时间O(N^2),空间O(1) 算法稳定

 *优化:
    记录某次遍历时最后发生数据交换的位置,这个位置之后的数据显然已经有序,不用再排序了.
    因此通过记录最后发生数据交换的位置就可以确定下次循环的范围,此位置初始化为待排序序列顶层元素位置.
 */

void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for(int i=0; i != n-1; ++i)
        for(int j=n-1; j != i; --j)
            if(arr[j] < arr[j-1])
            {
                int tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
            }
}

//改进后的冒泡排序;
void bubbleSort_imp(vector<int> &arr)
{
    int n = arr.size();
    for(int i=0; i != n-1;)
    {
        int next = n - 1;  //标记最后一次数据交换的位置,初始化为顶层元素位置;
        for(int j=n-1; j != i; --j)
            if(arr[j] < arr[j-1])
            {
                int tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
                next = j;
            }
        i = next;  //更新下一轮排序的序列范围;
    }
}
