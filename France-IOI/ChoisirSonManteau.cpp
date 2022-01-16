#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

vector<int> start_;
vector<int> end_;


int main() {

    int nbManteaux;
    cin >> nbManteaux;

    // get the tableau
    for (int i(0); i < nbManteaux; i++) {
        int a, b;
        std::cin >> a;
        std::cin >> b;
        start_.push_back(a);
        end_.push_back(b);
    }

    int size = start_.size();

    int maxStart = *max_element(start_.begin(), start_.end());
    int maxEnd = *max_element(end_.begin(), end_.end());

    int maxAll = max(maxStart, maxEnd);

    int countingInterval[maxAll + 2];
    memset(countingInterval, 0, sizeof(countingInterval));

    for (int i(0); i < size; i++) {
        // Ajoute 1 à l'endroit ou démarre un intervalle
        countingInterval[start_[i]]++;
        // On soustrait 1 là ou un intervalle se termine
        countingInterval[end_[i]]--;
    }

    for (int i(0); i < maxAll + 2; i++) {
        cout << countingInterval[i] << " ";
    }
    cout << endl;


    int maxIntervalles = 1;
    int cur = 0;

    for (int i(0); i < maxAll + 2; i++) {
        cur += countingInterval[i];
        if (maxIntervalles < cur) {
            maxIntervalles = cur;
        }
    }

    cout << maxIntervalles << endl;


}