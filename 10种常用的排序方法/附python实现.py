"This article shows ten ways of sorting implemented by python."
# !/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint: disable = I0011, invalid-name, w0612, w0703, w0621, w0601, E0211

def insertSort(arr):
    "直接插入"
    cnt = len(arr)
    for i in range(1, cnt):
        tmp, j = arr[i], i-1
        while j >= 0:
            if arr[j] > tmp:
                arr[j+1] = arr[j]
                j -= 1
            else: break
        arr[j+1] = tmp

def insertSort_imp(arr):
    "折半插入"
    cnt = len(arr)
    for i in range(1, cnt):
        if arr[i] < arr[i-1]:
            low, high = 0, i-1
            while low <= high:
                mid = (high + low) / 2
                if arr[i] < arr[mid]:
                    high = mid - 1
                else:
                    low = mid + +1
            tmp = arr[i]
            for j in range(i, low, -1):
                arr[j] = arr[j-1]
            arr[low] = tmp

def bubbleSort(arr):
    "冒泡排序"
    cnt = len(arr)
    for i in range(0, cnt-1):
        for j in range(cnt-1, i, -1):
            if arr[j] < arr[j-1]:
                arr[j-1], arr[j] = arr[j], arr[j-1]

def bubbleSort_imp(arr):
    "冒泡排序,记录最后发生交换的位置"
    i, cnt = 0, len(arr)
    while i < cnt-1:
        nextValue_i = cnt - 1
        for j in range(cnt-1, i, -1):
            if arr[j] < arr[j-1]:
                arr[j-1], arr[j] = arr[j], arr[j-1]
                nextValue_i = j
        i = nextValue_i

def shellSort(arr):
    "希尔排序"
    d, cnt = len(arr) // 2, len(arr)
    while d >= 1:
        for i in range(d, cnt):
            j, tmp = i, arr[i]
            while j >= d and tmp < arr[j-d]:
                arr[j] = arr[j-d]
                j -= d
            arr[j] = tmp
        d //= 2

def partition(i, j, pivot, arr):
    "划分元素,小于放左边,大于等于放右边"
    left, right = i, j
    while left <= right:
        while arr[left] < pivot:
            left += 1
        while arr[right] >= pivot:
            right -= 1
        if left < right:
            arr[left], arr[right] = arr[right], arr[left]
    return left

def quickSort(arr):
    "快速排序"
    def findPivot_1(i, j, arr):
        "选取最先发现的两个不等元素中的较大者"
        for k in range(i+1, j):
            if arr[k] > arr[i]:
                return k
            elif arr[k] < arr[i]:
                return i
        return -1

    def quickSort(i, j, arr):
        "快速排序"
        pivotPos = findPivot_1(i, j, arr)
        if pivotPos != -1:
            pivot = arr[pivotPos]
            k = partition(i, j, pivot, arr)
            quickSort(i, k-1, arr)
            quickSort(k, j, arr)

    quickSort(0, len(arr)-1, arr)

def quickSort_imp(arr):
    "快速排序"
    def findPivot_2(i, j, arr):
        "三者排序取中"
        mid = (i + j) // 2
        if arr[i] > arr[j]:
            arr[i], arr[j] = arr[j], arr[i]
        if arr[mid] > arr[j]:
            arr[mid], arr[j] = arr[j], arr[mid]
        if arr[i] > arr[mid]:
            arr[i], arr[mid] = arr[mid], arr[i]
        return mid

    def quickSort_imp(i, j, arr):
        "快速排序"
        if j - i <= 5:  #序列规模小,采用直接插入排序;
            for h in range(i+1, j+1):
                tmp, k = arr[h], h-1
                while k >= i:
                    if arr[k] > tmp:
                        arr[k+1] = arr[k]
                        k -= 1
                    else: break
                arr[k+1] = tmp
        else:
            pivot = arr[findPivot_2(i, j, arr)] #选取基准元素;
            k = partition(i, j, pivot, arr)     #划分元素;
            quickSort_imp(i, k-1, arr)          #对左右序列递归求解问题;
            quickSort_imp(k, j, arr)

    quickSort_imp(0, len(arr)-1, arr)

def selectionSort(arr):
    "选择排序"
    def selectionSort(left, right, arr):
        "选择排序"
        for i in range(left, right):    #找出当前序列中的最小元素;
            minPos, minValue = i, arr[i]
            for j in range(i+1, right+1):
                if arr[j] < minValue:
                    minPos, minValue = j, arr[j]
            if minPos != i:             #将找到的最小元素放到已排序序列末端;
                arr[minPos], arr[i] = arr[i], minValue
    selectionSort(0, len(arr)-1, arr)

def heapSort(arr):
    "堆排序"
    def pushdown(first, last, arr):
        "向下整理堆元素"
        cur, value = first, arr[first]
        while cur <= last // 2:             #将左右儿子中的较小者上移;
            nextValue = cur * 2
            if nextValue < last and arr[nextValue+1] < arr[nextValue]:
                nextValue += 1              #有右儿子且右儿子小于左儿子;
            if value <= arr[nextValue]:
                break
            arr[cur], cur = arr[nextValue], nextValue
        arr[cur] = value

    dev = [0] + arr     #个人习惯从下标为1开始建堆,这不是必须的操作,故不计此开销;
    for i in range(len(arr) // 2, 0, -1):   #建立最小堆;
        pushdown(i, len(arr), dev)

    for i in range(len(arr), 1, -1):        #交换元素,将剩余元素整理成堆;
        dev[1], dev[i] = dev[i], dev[1]
        pushdown(1, i-1, dev)

    for i in range(1, len(arr)+1):          #复制回初始数组;
        arr[i-1] = dev[i]

def merge(left, right):
    "二路合并"
    i, j = 0, 0
    result = []
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result += left[i:]
    result += right[j:]
    return result

def mergeSort_1(arr):
    "自底向上的非递归算法"
    def mergePass(h, arr):
        "merge pass"
        i, result = 0, []
        while i+2*h-1 < len(arr):
            result += merge(arr[i:i+h], arr[i+h:i+2*h])
            i += 2 * h
        if i+h-1 < len(arr)-1:  #剩下元素个数大于一段,不足两段;
            result += merge(arr[i:i+h], arr[i+h:])
        else:                   #剩下元素个数不足一段,直接拷贝;
            result += arr[i:]
        return result

    h = 1
    while h < len(arr):
        result = mergePass(h, arr)
        arr = result
        h *= 2
    return arr

def mergeSort_2(arr):
    "分治算法"
    if len(arr) <= 1:   #递归终止条件
        return arr
    num = len(arr) // 2
    left = mergeSort_2(arr[:num])
    right = mergeSort_2(arr[num:])
    return merge(left, right)   #二路归并

def radixSort_1(arr, figure):
    "采用桶的分配与收集作为稳定排序方法"
    for i in range(0, figure):
        q = [[] for i in range(10)]
        while len(arr) > 0: #分配
            data = arr.pop(0)
            r = data // int(pow(10, i)) % 10
            q[r].append(data)
        for lst in q:       #收集
            arr += lst

def radixSort_2(arr, figure):
    "采用计数排序作为稳定排序方法"
    for i in range(0, figure):
        comp = []                               #分离关键字分量,找出最大值
        for elem in arr:
            comp.append(elem // int(pow(10, i)) % 10)
        maxValue = max(comp)

        count = [0 for i in range(maxValue+1)]  #计数
        dev = [0 for i in range(len(arr)+1)]
        for elem in comp:
            count[elem] += 1
        for j in range(1, maxValue+1):
            count[j] += count[j-1]

        for j in range(len(arr)-1, -1, -1):     #将记录输出到正确位置上
            dev[count[comp[j]]] = arr[j]
            count[comp[j]] -= 1
        for j in range(0, len(arr)):            #拷贝回原list
            arr[j] = dev[j+1]

def countingSort(arr):
    "计数排序"
    if len(arr) == 0:
        return
    maxValue = max(arr)                     #统计最大值

    count = [0 for i in range(maxValue+1)]  #计数
    dev = [0 for i in range(len(arr)+1)]
    for elem in arr:
        count[elem] += 1
    for i in range(1, maxValue+1):
        count[i] += count[i-1]

    for i in range(len(arr)-1, -1, -1):     #将记录输出到正确位置
        dev[count[arr[i]]] = arr[i]
        count[arr[i]] -= 1
    for i in range(0, len(arr)):            #拷贝回原list
        arr[i] = dev[i+1]

def bucketSort(arr):
    "桶排序"
    buckets = [[] for i in range(101)]
    for elem in arr:            #分配
        buckets[elem//10].append(elem)
    for lst in buckets:         #分别排序
        insertSort(lst)
    arr.clear()
    for lst in buckets:         #收集
        arr += lst

arr = [int(n) for n in input("Please input some integers:").split()]
radixSort_2(arr, 4)
print("排序后:", arr)
