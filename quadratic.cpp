#include <bits/stdc++.h>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// Convert string in given base to decimal
long long decodeValue(const string &value, int base) {
    return stoll(value, nullptr, base);
}

// Lagrange interpolation at x=0
double lagrangeInterpolation(const vector<pair<int, long long>> &points, int k) {
    double y = 0.0;

    for (int i = 0; i < k; i++) {
        double xi = points[i].first;
        double yi = points[i].second;
        double numerator = 1.0, denominator = 1.0;

        for (int j = 0; j < k; j++) {
            if (j != i) {
                double xj = points[j].first;
                numerator *= (0.0 - xj);
                denominator *= (xi - xj);
            }
        }
        y += (numerator / denominator) * yi;
    }
    return y;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <json-file>" << endl;
        return 1;
    }

    try {
        // Read JSON file (first argument)
        ifstream inFile(argv[1]);
        if (!inFile) {
            cerr << "Error: Could not open " << argv[1] << endl;
            return 1;
        }

        json obj;
        inFile >> obj;

        // Extract n and k
        int n = obj["keys"]["n"];
        int k = obj["keys"]["k"];

        // Extract points
        vector<pair<int, long long>> points;
        for (auto& [key, val] : obj.items()) {
            if (key == "keys") continue;
            int x = stoi(key);
            int base = stoi(val["base"].get<string>());
            string valueStr = val["value"].get<string>();
            long long y = decodeValue(valueStr, base);
            points.push_back({x, y});
        }

        // Sort points by x
        sort(points.begin(), points.end());

        // Use first k points for Lagrange interpolation at x=0
        double secret = lagrangeInterpolation(points, k);

        cout << llround(secret) << endl;  // print only secret value

    } catch (exception &e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
