#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

// Decode base-n encoded string into integer
long long decodeBaseN(const string &s, int base) {
    long long val = 0;
    for (char c : s) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'z') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') digit = c - 'A' + 10;
        else digit = 0; // fallback
        val = val * base + digit;
    }
    return val;
}

// Function to solve secret (c) from JSON file
long double solveFromJson(const string &filename) {
    ifstream f(filename);
    if (!f.is_open()) {
        cerr << "Error: Cannot open " << filename << endl;
        return -1;
    }

    json data;
    f >> data;

    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    vector<long double> X, Y;

    // Collect first k points from JSON
    for (auto &el : data.items()) {
        if (el.key() == "keys") continue;
        if ((int)X.size() == k) break;

        int x = stoi(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string ystr = el.value()["value"];
        long long y = decodeBaseN(ystr, base);

        X.push_back(x);
        Y.push_back(y);
    }

    // Lagrange interpolation at x=0 to find constant term (secret)
    long double c = 0.0;
    for (int i = 0; i < k; i++) {
        long double term = Y[i];
        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            term *= (0 - X[j]) / (X[i] - X[j]);
        }
        c += term;
    }

    return c;
}

int main() {
    cout << fixed << setprecision(0);

    long double c1 = solveFromJson("testcase1.json");
    if (c1 != -1) cout << "Secret from testcase1.json = " << c1 << endl;

    long double c2 = solveFromJson("testcase2.json");
    if (c2 != -1) cout << "Secret from testcase2.json = " << c2 << endl;

    return 0;
}

