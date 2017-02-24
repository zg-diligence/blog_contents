/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.24
 */

#include <iostream>
#include <vector>
using namespace std;

/*希尔排序

 *思考改进:
    希尔排序是对直接插入排序的改进,若排序序列基本有序时,直接插入排序的效率可以大大提高,
    同时由于直接插入排序算法简单,当待排序序列数量较小时效率也很高.

 *算法思想:
    将整个待排序记录分割成若干个子序列,在子序列中分别进行直接插入排序,当整个序列中的记录
    基本有序时,对全体记录进行直接插入排序.如何分割子序列是希尔排序的关键,不同的分割策略,
    算法的效率亦不同.

 *算法流程:
    1、选取合适的gap(<n),将全部元素分割成gap个子序列,所有相距为gap个元素放入同一个子序列;
    2、对每一个子序列进行直接插入排序;
    3、缩小间隔gap,例如 gap=gap/2;
    4、重复步骤1-3,直到gap=1,将所有元素放入同一个序列中进行直接插入排序,排序结束.

 *算法性能:
    时间O(nlogn)~O(n^2),空间O(1) 算法不稳定
*/

void shellSort(vector<int> &arr)
{
    for(int d=arr.size()/2; d >= 1; d /= 2)
        for(int i=d; i != arr.size(); ++i)
        {
            int j = i;
            int tmp = arr[i];
            for(; j >= d && tmp < arr[j-d]; j -= d)
                arr[j] = arr[j-d];
            arr[j] = tmp;
        }
}