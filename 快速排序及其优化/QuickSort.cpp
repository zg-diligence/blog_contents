/*
 * Copyright(c) 2017 Gang Zhang
 * All rights reserved.
 * Author:Gang Zhang
 * Date:2017.5.14
 */
 
#include <iostream>
#include <vector>
using namespace std;

//左右指针赋值法.
int partition_1(vector<int> &arr, int low, int high)
{
    int p = arr[low];
    while(low < high)
    {
        while(arr[high] >= p && low < high) high--;
        arr[low] = arr[high];
        while(arr[low] < p && low < high) low++;
        arr[high] = arr[low];
    }
    arr[low] = p;
    return low;
}

void sort_1(vector<int> &arr, int low, int high)
{
    if(low >= high)
        return;
    int mid = partition_1(arr, low, high);
    sort_1(arr, low, mid-1);
    sort_1(arr, mid+1, high);
}

//左右指针交换法.
void swap(vector<int> &arr, int a, int b)
{
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int partition_2(vector<int> &arr, int low, int high)
{
    int pivot = arr[low];
    int p = low + 1, q = high;
    while(p <= q)
    {
        while(arr[p] < pivot && p <= q) p++;
        while(arr[q] >= pivot && p <= q) q--;
        if(p < q)
            swap(arr, p, q);
    }
    swap(arr, low, q);
    return q;
}

void sort_2(vector<int> &arr, int low, int high)
{
    if(low >= high)
        return;
    int mid = partition_2(arr, low, high);
    sort_2(arr, low, mid -1);
    sort_2(arr, mid + 1, high);
}

//前后指针法
int partition_3(vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int distionation = low;
    for(int i = low; i < high; i++)
    {
        if(arr[i] < pivot)
        {
            swap(arr, distionation, i);
            distionation++;
        }
    }
    swap(arr, distionation, high);
    return distionation;
}

void sort_3(vector<int> &arr, int low, int high)
{
    if(low >= high)
        return;
    int mid = partition_3(arr, low, high);
    sort_3(arr, low, mid-1);
    sort_3(arr, mid+1, high);
}

int main()
{
    cout<<"输入元素:"<<endl;
    vector<int> arr;
    int value;
    while(cin >> value && value != 0)
        arr.push_back(value);
    //sort_1(arr, 0, arr.size()-1);
    //sort_2(arr, 0, arr.size()-1);
    sort_3(arr, 0, arr.size()-1);
    for(auto n: arr)
        cout<<n<<" ";
    cout<<endl;
    system("pause");
    return 0;
}
