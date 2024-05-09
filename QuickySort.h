#include <chrono>
#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
#include <map>
using namespace std;
size_t currentMemory = 0;
size_t peakMemory = 0;
//masurarea memoriei consumate de fiecare functie de soratare in parte
void* operator new(size_t size) {
    currentMemory += size;
    peakMemory = max(peakMemory, currentMemory);
    return malloc(size);
}

// Overload the delete operator
void operator delete(void* memory, size_t size) noexcept {
    currentMemory -= size;
    free(memory);
}
class arr {
private:
    vector<int> a;
    int n;

    static bool cmp(const int& a, const int& b) {
        return a < b;
    }
    //soratare in O(n * sqrt(n))
    void SquareSort() {
        if(a.empty()) return;
        bool ok = false;
        int distance = 1;
        while(!ok){
            ok = true;
            int maxi = ceil(sqrt(a.size()));
            for(distance = 1; distance <= maxi; distance++){
                bool local = true;
                for(int i = 0; i < n-distance; i++){
                    if(a[i]>a[i+1]){
                        swap(a[i],a[i+1]);
                        local = false;
                    }
                }
                ok &= local;
            }
        }
    }
    //sortare in O(n log n)
    void QuickySort() {
        sort(a.begin(), a.end(), cmp);
    }
    //sortare in O(n^2)
    void SelectionSort() {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (a[i] > a[j]) {
                    swap(a[i], a[j]);
                }
            }
        }
    }
    //radix sort, O(n) ca timp de executie dar slab din punct de vedere al memoriei consumate
    int getMaxDigits(vector<int>& a) {
        int maxDigits = 0;
        for (int num : a) {
            if (num == 0) maxDigits = max(maxDigits, 1);
            else maxDigits = max(maxDigits, static_cast<int>(log10(num) + 1));
        }
        return maxDigits;
    }
    int getDigit(int number, int place) {
        return (number / static_cast<int>(pow(10, place))) % 10;
    }
    void radixSort() {
        int maxDigits = getMaxDigits(a);

        // Create a bucket for each digit (0-9)
        vector<vector<int>> buckets(10);

        // Sort from least significant digit to most significant digit
        for (int digit = 0; digit < maxDigits; digit++) {
            // Distribute elements into buckets
            for (int num : a) {
                int bucketIndex = getDigit(num, digit);
                buckets[bucketIndex].push_back(num);
            }

            // Reassemble aay from buckets
            int index = 0;
            for (int i = 0; i < 10; i++) {
                for (int num : buckets[i]) {
                    a[index++] = num;
                }
                buckets[i].clear();
            }
        }
    }
    //bit sort, O(n) dar totusi mai rapid decat radix pentru ca opereaza pe biti si toata treaba de sortare se intampla
    //pe baza MSB (most semnificant bit)-ului
    void sortByBits() {
        if (n < 2) return;

        int maxVal = *max_element(a.begin(), a.end());
        int numBits = 0;
        while (maxVal > 0) {
            maxVal >>= 1;
            numBits++;
        }

        for (int bit = 0; bit < numBits; bit++) {
            int mask = 1 << bit;
            int zeroIndex = 0, oneIndex = n - 1;

            // Partition in-place
            while (zeroIndex < oneIndex) {
                while (zeroIndex < n && !(a[zeroIndex] & mask)) zeroIndex++;
                while (oneIndex >= 0 && (a[oneIndex] & mask)) oneIndex--;

                if (zeroIndex < oneIndex) {
                    std::swap(a[zeroIndex], a[oneIndex]);
                }
            }
        }
    }


public:
    ~arr() {}

    void Smart() {
        QuickySort();
    }
    void Medium() {
        SquareSort();
    }
    void aBitDum() {
        SelectionSort();
    }
    void radix(){
        radixSort();
    }
    void binary(){
        sortByBits();
    }
    //nu o sa mai folosim un csv ca sa testam algoritmul doar sa o sa ii dam sa sorteze valori de la 0 la 99
    arr(int size) : n(size), a(size) {}

    void fill() {
        a.resize(n); // Ensure the vector is resized to original size
        for (auto &i: a)
            i = rand() % 100; // Use rand() and seed it in main() if necessary
    }

    void clear() {
        a.clear();
        a.resize(n);  // Reallocate memory
        currentMemory = 0;  // Reset memory usage measurement
        peakMemory = 0;
    }
    //cronometrul cu care calculam timpul de sortare in secunde si memoria pe care o ocupa fiecare algoritm
    template<typename Func>
    void testSortingAlgorithms(Func function, const string& functionName) {
        auto start = chrono::high_resolution_clock::now();
        function();
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "Function: " << functionName << " - Time taken: " << duration.count() << " microseconds" << "  |  ";
        cout << "Peak memory used: " << peakMemory << " bytes" << endl;
    }
};