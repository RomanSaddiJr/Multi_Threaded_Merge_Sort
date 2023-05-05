# Multi_Threaded_Merge_Sort
This C++ program implements the Merge Sort algorithm using multithreading

## How It's Made:
**Tools used:** C++

This program performs a fairly simple merge_sort algorithm on an input array derived from input.txt. This program utilizes the mutex and **<pthread.h>** libraries to ensure optimize the merge sort and avoid race conditions.

## Contents:
This repository contains the files:

**'main.cpp':** This C++ file reads the contents of **'input.txt'** as an array of integers and performs a merge sort on the array. The merge sort is accomplished through a combination of pthread creations and mutex locks and unlocks. The sorted array is then printed into the console.

**'input.txt':** Write any list of integers separated by spaces to alter input.

## How to run:
Simply call the program, ensuring input.txt is visible and contains a list of integers.
