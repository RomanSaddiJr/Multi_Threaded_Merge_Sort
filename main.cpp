#include <iostream>
#include <fstream>
#include <pthread.h>
#include <mutex>
#include <stdint.h>

using namespace std;

// Define the mutex to prevent race conditions
mutex mtx;

// Define merge function
void merge(int arr[], int l, int m, int r) {
  // l = starting position of left subarray (Left)
  // m = ending position of left and starting position of right (Middle)
  // r = ending position of the right subarray (Right)
  
    int n1 = m - l + 1; // left array lenth
    int n2 = r - m;     // right array length
    int L[n1], R[n2];   // Create temp arrays to store the two arrays
  
  // Create a Left and Right Array and fill them with elements
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

  // Empty the iterators for the next loop
    int i = 0, j = 0, k = l;

  // Merge the left and right arrays into the original array in correct order
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

  // copy back any remaining left array elements into original array
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 // copy back any remaining right array elements into original array
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Define merge sort function
void* mergeSort(void* arg) {
  
    // Arguments are array of integers
    int* args = (int*)arg;
    // Integer array is cast to a pointer pointing to an array to sort
    int* arr = (int*)(uintptr_t)args[1];
    // Define the start and end of each array
    int l = 0;
    int r = args[0] - 1;

    // iterate through array
    if (l < r) {
        // Find the middle index of array
        int m = l + (r - l) / 2;

        // Create threads for left and right subarrays
        pthread_t threadLeft, threadRight;
        int argsLeft[2] = { m - l + 1, (uintptr_t)(arr + l) };
        int argsRight[2] = { r - m, (uintptr_t)(arr + m + 1) };
        pthread_create(&threadLeft, NULL, mergeSort, argsLeft);
        pthread_create(&threadRight, NULL, mergeSort, argsRight);

        // Wait for threads to complete
        pthread_join(threadLeft, NULL);
        pthread_join(threadRight, NULL);

        // Merge the two subarrays
        mtx.lock();
        merge(arr, l, m, r);
        mtx.unlock();
    }
    pthread_exit(NULL);
}

int main() {
    // open input file
    ifstream input("input.txt");
    // return error if file cannot be opened
    if (!input.is_open()) {
        cout << "Unable to open input file." << endl;
        return 1;
    }
    // count the number of integers in the input file
    int n = 0;
    int temp;
    while (input >> temp) {
        n++;
    }

    // return back to start of file
    input.clear();
    input.seekg(0, ios::beg);

    // allocate space for an array of integers on the heap that was counted earlier
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }
    // close the file
    input.close();

    // create an array of two integers,
    // arg[1] = size of array
    // arg[2] = address of the array
    int args[2] = { n, (uintptr_t)arr };

    // create a thread to sort the array with mergeSort()
    pthread_t thread;
    pthread_create(&thread, NULL, mergeSort, args);

    // wait for the thread to complete
    pthread_join(thread, NULL);

    // Print the merge sorted array
    cout << "Sorted array: " << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Deallocate the memory used within the array
    delete[] arr;

    return 0;
}
