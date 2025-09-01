#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Multiply polynomials with (x - root)
vector<long long> multiply(const vector<long long>& poly, long long root) {
    int n = poly.size();
    vector<long long> result(n + 1, 0);
    for (int i = 0; i < n; i++) {
        result[i] += poly[i];
        result[i + 1] += poly[i];
        result[i] -= root * poly[i];
    }
    return result;
}

// Evaluate polynomial at x
long long evaluate(const vector<long long>& poly, long long x) {
    long long result = 0;
    long long power = 1;
    for (auto coeff : poly) {
        result += coeff * power;
        power *= x;
    }
    return result;
}

// Solve one testcase and return polynomial + roots
pair<vector<long long>, vector<long long>> solveTest(const string &filename) {
    ifstream inFile(filename);

    // If not found in build folder, try project root
    if (!inFile) {
        inFile.open("../" + filename);
    }

    if (!inFile) {
        cerr << "Error: Could not find " << filename << endl;
        return {{}, {}}; // return empty
    }

    json data;
    inFile >> data;

    int k = data["keys"]["k"];
    vector<long long> roots;
    for (auto& [key, val] : data.items()) {
        if (key == "keys") continue;
        int base = stoi(val["base"].get<string>());
        string value = val["value"].get<string>();

        try {
            long long root = stoll(value, nullptr, base);
            roots.push_back(root);
        } catch (const std::out_of_range& e) {
            cerr << "Warning: Value " << value << " too large for long long. Setting root = 0." << endl;
            roots.push_back(0); // fallback
        } catch (const std::invalid_argument& e) {
            cerr << "Warning: Invalid number format: " << value << ". Setting root = 0." << endl;
            roots.push_back(0); // fallback
        }
    }

    vector<long long> poly = {1};
    for (int i = 0; i < k && i < (int)roots.size(); i++) {
        poly = multiply(poly, roots[i]);
    }

    return {poly, roots};
}

// Count wrong dataset points (roots that don’t satisfy the polynomial)
int countWrongPoints(const vector<long long>& poly, const vector<long long>& roots) {
    int wrong = 0;
    for (auto r : roots) {
        if (evaluate(poly, r) != 0) {
            wrong++;
        }
    }
    return wrong;
}

int main() {
    // Test Case 1
    cout << "Output for TestCase - 1" << endl;
    cout << "Enter your answer" << endl;
    auto [poly1, roots1] = solveTest("testcase1.json");
    if (!poly1.empty()) cout << poly1[0] << endl;

    // Test Case 2
    cout << "Output for TestCase - 2" << endl;
    cout << "Enter your answer" << endl;
    auto [poly2, roots2] = solveTest("testcase2.json");
    if (!poly2.empty()) cout << poly2[0] << endl;

    // Wrong Data Set Points
    cout << "Wrong Data Set Points for Test Case-1 :" << endl;
    cout << "Enter your answer" << endl;
    if (!poly1.empty()) cout << countWrongPoints(poly1, roots1) << endl;

    cout << "Wrong Data Set Points for Test Case-2 :" << endl;
    cout << "Enter your answer" << endl;
    if (!poly2.empty()) cout << countWrongPoints(poly2, roots2) << endl;

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.