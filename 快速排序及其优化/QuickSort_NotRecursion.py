"""
  Copyright(c) 2017 Gang Zhang
  All rights reserved.
  Author:Gang Zhang
  Date:2017.5.11
  
  Abstract:
    This program mainly implements quick_sort and its improvement.
    There are 5 methods for choose of the pivot element in total, and 
    in QuickSort_6 we implements the three-way quick_sorting.
    All the implementations are not recursive.
"""

import time
import random
import matplotlib.pyplot as plt

def InsertionSort(arr, low, high):
    """Insertion sorting"""
    size = high - low + 1
    if size <= 1: return
    for i in range(low, high + 1):
        tmp, j = arr[i], i - 1
        while j >= low:
            if arr[j] > tmp:
                arr[j + 1], j = arr[j], j - 1
            else:break
        arr[j + 1] = tmp

def median_of_three(arr, low, high, mid = -1):
    """return pos of the median of three"""
    if mid == -1: mid = (low + high) // 2
    if arr[mid] <= arr[low] <= arr[high] or arr[high] <= arr[low] <= arr[mid]:
        return low
    if arr[low] <= arr[mid] <= arr[high] or arr[high] <= arr[mid] <= arr[low]:
        return mid
    if arr[low] <= arr[high] <= arr[mid] or arr[mid] <= arr[high] <= arr[low]:
        return high

def median_of_nine(arr, low, high):
    """return pos of the median of nine"""
    size = high - low + 1
    median_1 = median_of_three(arr, low, low + size // 3)
    median_2 = median_of_three(arr, low + size // 3 + 1, high - size // 3)
    median_3 = median_of_three(arr, high - size // 3 + 1, high)
    return median_of_three(arr, median_1, median_2, median_3)

def qsort(arr, stack):
    """called by main sorting function"""
    high, low = stack.pop(-1), stack.pop(-1)
    pivot, less_equal, greater = arr[high], [], []
    for x in arr[low:high]:
        if x <= pivot:less_equal.append(x)
        else:greater.append(x)
    arr[low: high + 1] = less_equal + [pivot] + greater
    if len(less_equal) > 1:
        stack.append(low);stack.append(low + len(less_equal) - 1)
    if len(greater) > 1:
        stack.append(high - len(greater) + 1);stack.append(high)

def QuickSort_1(arr):
    """choose arr[n] as the pivot"""
    if len(arr) <= 1: return arr
    stack = [0, len(arr)-1]
    while stack:
        qsort(arr, stack)
    return arr

def QuickSort_2(arr):
    """choose an element randomly as the pivot"""
    if len(arr) <= 1: return arr
    stack = [0, len(arr)-1]
    while stack:
        low, high = stack[-2], stack[-1]
        pos = random.randint(low, high)
        arr[pos], arr[high] = arr[high], arr[pos]
        qsort(arr, stack)
    return arr

def QuickSort_3(arr):
    """choose the bigger one between the first two diffrent elements as the pivot"""
    if len(arr) <= 1:return arr
    stack = [0, len(arr)-1]
    while stack:
        low, high = stack[-2], stack[-1]
        pos, k = low, low + 1
        while k <= high:
            if arr[k] > arr[pos]:
                pos = k;break
            elif arr[k] < arr[pos]:
                break
            k += 1
        if k == high + 1:
            stack.pop(-1); stack.pop(-1); continue
        arr[pos], arr[high] = arr[high], arr[pos]
        qsort(arr, stack)
    return arr

def QuickSort_4(arr):
    """choose median of three as the pivot"""
    if len(arr) <= 1: return arr
    stack = [0, len(arr) - 1]
    while stack:
        low, high = stack[-2], stack[-1]
        pos = median_of_three(arr, low, high)
        arr[pos], arr[high] = arr[high], arr[pos]
        qsort(arr, stack)
    return arr

def QuickSort_5(arr):
    """sort in different ways at lenth of the arr"""
    """
    when n < 7, insertion_sort
    when n = 7, choose the median one as the pivot
    when 7 < n < 40, choose the median of three as the pivot
    when n >= 40, choose the median of ni
    ne as the pivot
    """
    if len(arr) <= 1: return arr
    stack = [0, len(arr) - 1]
    while stack:
        size, low, high = stack[-1] - stack[-2] + 1, stack[-2], stack[-1]
        if size < 7:
            InsertionSort(arr, low, high)
            stack.pop(-1); stack.pop(-1); continue
        elif size == 7:
            arr[low + 3], arr[high] = arr[high], arr[low + 3]
        elif 7 < size < 40:
            pos = median_of_three(arr, low, high)
            arr[pos], arr[high] = arr[high], arr[pos]
        else:
            pos = median_of_nine(arr, low, high)
            arr[pos], arr[high] = arr[high], arr[pos]
        qsort(arr, stack)
    return arr

def QuickSort_6(arr):
    """quick_sort in three ways"""
    if len(arr) <= 1: return arr
    stack = [0, len(arr)-1]
    while stack:
        high, low = stack.pop(-1), stack.pop(-1)
        pivot, less, equal, greater =arr[random.randint(low, high)], [], [], []
        for x in arr[low: high + 1]:
            if x < pivot: less.append(x)
            elif x == pivot: equal.append(x)
            else:greater.append(x)
        arr[low: high + 1] = less + equal + greater
        if len(less) > 1:
            stack.append(low); stack.append(low + len(less) - 1)
        if len(greater) > 1:
            stack.append(high - len(greater) + 1);stack.append(high)
    return arr

def test_func(SortWays, standard = 1000):
    """test for each function with different scale data"""
    arrays = [[random.randint(1, 10000000) for k in range(standard * m)] for m in range(1, 91, 3)]
    times, names = [], [str(len(arr)) for arr in arrays]
    for sort in SortWays: # test for the custom sorting function.
        func_time = []
        for arr in arrays:
            t1 = time.time()
            sort(arr[:])
            t2 = time.time()
            func_time.append(round(t2 - t1, 6))
        times.append(func_time)
    func_time = []
    for arr in arrays: # test for the library sorting function.
        t1 = time.time()
        sorted(arr[:])
        t2 = time.time()
        func_time.append(round(t2 - t1, 6))
    times.append(func_time)
    return times, names

def test_arr(SortWays, RecordsNum = 10000):
    """test different function with the same data."""
    arrays = [[25] * RecordsNum,
              list(range(RecordsNum)),
              list(range(RecordsNum, 0, -1)),
              [random.randint(1, 10000000) for k in range(RecordsNum)],
              [random.randint(1, 100) for k in range(RecordsNum)]]
    names = ['same records',
             'positive sequence',
             'negative sequence',
             'random sequence',
             'repetited records']
    times = []
    for arr in arrays:
        arr_time = []
        for sort in SortWays: # test for the custom sorting function.
            t1 = time.time()
            sort(arr[:])
            t2 = time.time()
            arr_time.append(round(t2 - t1, 6))
        t1 = time.time()
        sorted(arr[:]) # test for library function.
        t2 = time.time()
        arr_time.append(round(t2 - t1, 6))
        times.append(arr_time)
    return times, names

if __name__ == '__main__':
    SortWays = [QuickSort_1, QuickSort_2, QuickSort_3, QuickSort_4, QuickSort_5, QuickSort_6]
    func_names = ['right_most', 'random', 'bigger_one', 'mid_three', 'mid_nine', '3ways', 'sorted']

    # test for data users input.
    while True:
        arr = [int(elem) for elem in input('请输入数据:\n').split(' ')]
        for k in range(len(SortWays)):
            print(func_names[k], ":\n", SortWays[k](arr[:]), sep='', end='\n\n')
        if input('Do you want to continue(Y/N):') != 'Y':break

    # test for single data.
    times, names = test_arr(SortWays)
    for k in range(len(times)):
        plt.subplot(3, 2, k+1)
        plt.bar([x for x in range(1, 8)], times[k], color='y', width=0.5)
        plt.xticks(list(range(1, 8)), func_names)
        plt.title(names[k])
    plt.tight_layout(w_pad=0.5, h_pad=0.5)
    plt.show()

    # test for single function, show together.
    times, names = test_func(SortWays)
    color = list('bgrcmyk')
    for k in range(len(times)):
        plt.plot(names, times[k], color[k] + 'o-', linewidth=1, label=func_names[k])
    plt.legend(loc='upper left')
    plt.title('comparision')
    plt.show()

    # test for single function, show separately.
    for k in range(len(times)):
        plt.subplot(3, 3, k+1)
        plt.plot(names, times[k], color[k] + 'o-', linewidth=1)
        plt.title(func_names[k])
    plt.tight_layout(w_pad=0.5, h_pad=0.5)
    plt.show()

"""
4 8 45 12 47 12 45 48 123 45 78 455 2 1 2 3 5 45 78 4 54 1 24 5 42 456 2 456 7 51 564 5 15 1 2 45 42 4 5 1 5421 565 2 1 54 12 45 1 2 4 51 5 4 15 54 878 4 54 78 45 12 154 612 45 4 5 2124 215 4 2 45 12 45 1 24 51 54 2 45 6122 45 4 1 2 5 2 8 4 4 5 4 4 1 2 8 4
"""