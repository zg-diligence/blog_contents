/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.24
 */

#include <iostream>
#include <vector>
using namespace std;

/*桶排序
 
 *算法思想:
    假设输入数据服从均匀分布,基于某种映射规则,将待排序数据分别放入各个桶中,对各个桶中的数据
    进行排序,然后按照次序把各个桶中的元素放回原数组即可.

 *算法流程:
    1、建立映射规则;
    2、将待排序数据分配到各个桶中;
    3、对各个桶中的数据进行排序;
    4、按照次序把各个桶中的元素放回原数组,排序完成.

 *算法性能:
    时间O(n),空间O(n) 算法稳定
  注:如果数据分布不均匀,当所有桶大小的平方和与总的元素个数成线性关系时,桶排序仍然可在线性时间内完成.
 */

//例如:假设输入数据在0-1000之间,101个桶,对每一个桶采用直接插入排序
void bucketSort(vector<int> &arr)
{
    //根据映射关系把元素分配到各个桶中;
    vector<vector<int>> buckets(101);
    for(auto num:arr)
        buckets[num/10].push_back(num);

    //对各个桶中数据进行直接插入排序;
    for(auto &tmp:buckets)
        insertSort(tmp);

    //按照次序将各个桶中的数据写回原数组;
    arr.clear();
    for(auto &tmp:buckets)
        for(auto num:tmp)
            arr.push_back(num);
}