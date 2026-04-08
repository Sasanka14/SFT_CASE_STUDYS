#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <iomanip>

using namespace std;


// DATA STRUCTURES


typedef pair<string, int> Query;
vector<Query> searchData;
const int TOP_K = 5;

void printDivider();


// UTILITIES


string toLowerCase(string str) {
    for (char& c : str) c = tolower(c);
    return str;
}

string trim(string str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}


// EDIT DISTANCE (FUZZY SEARCH)


int editDistance(string a, string b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({
                    dp[i - 1][j],
                    dp[i][j - 1],
                    dp[i - 1][j - 1]
                });
        }
    }

    return dp[n][m];
}


// DATA LOADING


void loadData() {
    ifstream file("data.txt");

    if (!file.is_open()) {
        cerr << "Error: Unable to open data.txt\n";
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        size_t commaPos = line.find(',');

        if (commaPos == string::npos || commaPos == 0) continue;

        string query = trim(line.substr(0, commaPos));
        string freqStr = trim(line.substr(commaPos + 1));

        try {
            int frequency = stoi(freqStr);
            query = toLowerCase(query);
            searchData.push_back({query, frequency});
        } catch (...) {
            continue;
        }
    }

    file.close();

    if (searchData.empty()) {
        cerr << "Error: No data loaded\n";
        exit(1);
    }

    sort(searchData.begin(), searchData.end(),
         [](const Query& a, const Query& b) {
             return a.first < b.first;
         });

    cout << "✓ Loaded " << searchData.size() << " queries\n";
}


// BINARY SEARCH (PREFIX)


int binarySearchPrefix(const string& prefix) {
    int left = 0, right = searchData.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = (left + right) / 2;

        int cmp = searchData[mid].first.compare(0, prefix.size(), prefix);

        if (cmp == 0) {
            result = mid;
            right = mid - 1;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

// HIGHLIGHT MATCH

string highlightMatch(string text, string prefix) {
    size_t pos = text.find(prefix);

    if (pos == string::npos) return text;

    return text.substr(0, pos) +
           "[" + text.substr(pos, prefix.length()) + "]" +
           text.substr(pos + prefix.length());
}


// AUTOCOMPLETE ENGINE (HYBRID + FUZZY)


vector<Query> autocomplete(string prefix) {
    vector<Query> results;

    if (prefix.empty()) return results;

    prefix = toLowerCase(prefix);

    // STEP 1: Prefix match (Binary Search)
    int startIdx = binarySearchPrefix(prefix);

    if (startIdx != -1) {
        for (int i = startIdx; i < searchData.size(); i++) {
            if (searchData[i].first.compare(0, prefix.size(), prefix) == 0) {
                results.push_back(searchData[i]);
            } else break;
        }

    }

    // STEP 2: Substring fallback
    if (results.empty()) {
        for (const auto& item : searchData) {
            if (item.first.find(prefix) != string::npos) {
                results.push_back(item);
            }
        }

    }

    // STEP 3: Fuzzy matching (typo tolerance)
    if (results.empty()) {
        for (const auto& item : searchData) {
            string part = item.first.substr(0, prefix.size());

            if (editDistance(part, prefix) <= 1) {
                results.push_back(item);
            }
        }
    }

    // STEP 4: Ranking
    sort(results.begin(), results.end(),
         [](const Query& a, const Query& b) {
             if (a.second != b.second)
                 return a.second > b.second;
             return a.first < b.first;
         });

    if (results.size() > TOP_K)
        results.resize(TOP_K);

    return results;
}


// DISPLAY (PREMIUM MINIMAL)


void printDivider() {
    cout << "────────────────────────────────────────────\n";
}

string trimDisplay(string text, int width = 38) {
    if (text.length() > width)
        return text.substr(0, width - 3) + "...";
    return text;
}

void displaySuggestions(const vector<Query>& results, const string& searchTerm) {
    printDivider();
    cout << "🔍 Query: " << toLowerCase(searchTerm) << "\n\n";

    if (results.empty()) {
        cout << "No suggestions found.\n";
        printDivider();
        return;
    }

    cout << "Top Suggestions\n";
    cout << "───────────────\n";

    string lowerQuery = toLowerCase(searchTerm);

    for (const auto& r : results) {
        string highlighted = highlightMatch(r.first, lowerQuery);

        cout << "• "
             << left << setw(40)
             << trimDisplay(highlighted)
             << r.second << "\n";
    }

    printDivider();
}


// MAIN


int main() {
    cout << "\nSearchCraft — Autocomplete Engine\n";
    cout << "Type 'exit' to quit\n";
    printDivider();

    cout << "Loading data...\n";
    loadData();

    string userInput;

    while (true) {
        cout << "\nEnter search: ";
        getline(cin, userInput);

        userInput = trim(userInput);

        if (userInput == "exit" || userInput == "quit" || userInput == "q") {
            cout << "\nGoodbye.\n";
            break;
        }

        if (userInput.empty()) {
            cout << "Please enter a valid query.\n";
            continue;
        }

        vector<Query> suggestions = autocomplete(userInput);

        displaySuggestions(suggestions, userInput);
    }

    return 0;
}