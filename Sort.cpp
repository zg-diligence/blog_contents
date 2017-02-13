/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2016.12.31
 */

#include <iostream>
#include <vector>
#include <queue>
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
    因此通过记录最后发生数据交换的位置就可以确定下次循环的范围,此位置初始化为底层元素位置.
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
        int next = n - 1;  //标记最后一次数据交换的位置,初始化为底层元素位置;
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



//堆排序(降序)
void pushDown(int first, int last, vector<int> &arr)
{
    int cur = first;
    int value = arr[first];
    while(cur <= last/2)
    {
        int next = cur * 2;
        if(next < last && arr[next+1] < arr[next])
            next++;
        if(value <= arr[next])break;
        arr[cur] = arr[next];
        cur = next;
    }
    arr[cur] = value;
}

void heapSort(vector<int> &arr)
{
    vector<int> dev(arr.size()+1);
    for(int i=1; i!=arr.size()+1; ++i)
        dev[i]=arr[i-1];

    for(int i=arr.size()/2; i>=1; --i)
        pushDown(i,arr.size(),dev);

    for(int i=arr.size(); i>=2; --i)
    {
        int tmp = dev[1];
        dev[1] = dev[i];
        dev[i] = tmp;
        pushDown(1,i-1,dev);
    }

    for(int i=1; i!=arr.size()+1; ++i)
        arr[i-1]=dev[i];
}


//直接插入排序;
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

//希尔排序;
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

//归并排序 -- 由底层向上的递归算法;
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

void mergePass(int h, vector<int> &arrA, vector<int> &arrB)
{
    int i = 0;
    for(; i+2*h-1 < arrA.size(); i += 2*h)
        merge(i,i+h-1,i+2*h-1,arrA,arrB);
    if(i+h-1 < arrA.size()-1)
        merge(i,i+h-1,arrA.size()-1,arrA,arrB);
    else
        for(; i!=arrA.size(); ++i)
            arrB[i] = arrA[i];
}

void mergeSort_1(vector<int> &arr)
{
    int h = 1;
    vector<int> dev(arr.size());
    while(h < arr.size())
    {
        mergePass(h,arr,dev);
        h *= 2;
        mergePass(h,dev,arr);
        h *= 2;
    }
}

//归并排序 -- 分治算法
void mergeSort_2(int low, int high, vector<int> &arrA, vector<int> &arrB)
{
    if(low < high)
    {
        int mid = low + (high - low) / 2;
        mergeSort_2(low,mid,arrA,arrB);
        mergeSort_2(mid+1,high,arrA,arrB);
        merge(low,mid,high,arrA,arrB);
        for(int i = low; i <= high; ++i)
            arrA[i] = arrB[i];
    }
}

void mergeSort_2(vector<int> &arr)
{
    vector<int> dev(arr.size());
    mergeSort_2(0,arr.size()-1,arr,dev);
}


//基数排序;
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

int main()
{
    vector<int> arr;int num;

    cout<<"请输入数据,以0结束:"<<endl;
    while(cin >> num && num != 0)
        arr.push_back(num);
    
    //quickSort(arr);
    //bubbleSort(arr);
    //bubbleSort_imp(arr);
    selectionSort(arr);
    //heapSort(arr);
    //insertSort(arr);
    //shellSort(arr);
    //mergeSort_1(arr);
    //mergeSort_2(arr);
    //radixSort(arr,5);

    cout<<"排序后:";
    for(auto n:arr)
        cout<<n<<" ";
    cout<<endl;
    system("pause");
    return 0;
}