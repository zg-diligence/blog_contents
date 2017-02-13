/*
 *  Copyright(c) 2016 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.2.13
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

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
    //selectionSort(arr);
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