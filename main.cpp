#include <fstream>
#include <vector>
#include "AVL.h"
#include "HashTable.h"
using namespace std;

string cleanUpDate(string date) {
    string res = "";
    for (int i = 0; i < date.length(); i++) {
        if (date[i] != '-') {
            res += date[i];
        }
    }
    return res;
}

vector<pair<string, HashTable>> createHashTables(int capacity, float maxLoadFactor) {
    vector<pair<string, HashTable>> res;
    ifstream inFile("../daily.csv");
    if (inFile.is_open()) {
        string line, date, country, rate;
        bool inserted;
        int dateVal, resIndex;
        float rateVal;

        // Header line
        getline(inFile, line);
        while (getline(inFile, line)) {
            inserted = false;
            // Format date into integer - YYYYMMDD
            date = line.substr(0, 10);
            date = cleanUpDate(date);
            dateVal = stoi(date);

            // Find second comma, separating country and rate
            // If rate is missing, do not insert
            int secondIndex = line.find(',', 11);
            if (secondIndex != string::npos and secondIndex != line.size()-1) {
                country = line.substr(11, secondIndex - 11);
                rate = line.substr(secondIndex + 1, line.size() - secondIndex);
                rateVal = stof(rate);
                // Normalize rates to be currency/USD
                if (country == "Australia" or country == "Euro" or country == "Ireland"
                    or country == "New Zealand" or country == "United Kingdom") {
                    rateVal = (1 / rateVal);
                }
                // Check if current country has a hash table
                // Insert a new pair if needed
                for (int i = 0; i < res.size(); i++) {
                    if (res[i].first == country) {
                        res[i].second.insert(dateVal, rateVal);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    resIndex = res.size();
                    res.push_back(make_pair(country, HashTable(capacity, maxLoadFactor)));
                    res[resIndex].second.insert(dateVal, rateVal);
                }
            }
        }
    }
    return res;
}

vector<AVLTree>* createAVLTrees(int capacity) {
    // Declare variables, most of which are initialized per loop iteration
    ifstream reader;
    reader.open("daily.csv");
    string line;
    string date;
    string country;
    string rate;

    int dateVal;
    float rateVal;

    string currentCountry = "nReal";
    vector<AVLTree>* countries = new vector<AVLTree>;

    // Read header line
    getline(reader, line);

    for (int i = 0; i < capacity; i++) {
        // Read line
        getline(reader, line);
        // All dates should be in YYYY-MM-DD format, if they are not
        // 10 digits long, cout error
        if (line.at(10) != ',') {
            cout << "Error! date is nonstandard" << endl;
        }
        // Format date into integer - YYYYMMDD
        date = line.substr(0, 10);
        date = cleanUpDate(date);
        dateVal = stoi(date);
        // Find second comma, separating country and rate
        // Some entries are missing this, so don't bother continuing with insertion process
        // if second comma (and rate) are missing
        int secondIndex = line.find(',', 11);
        if (secondIndex != string::npos and secondIndex != line.size()-1) {
            country = line.substr(11, secondIndex - 11);
            rate = line.substr(secondIndex + 1, line.size() - secondIndex);
            rateVal = stof(rate);
            // Annoyingly, 5 countries are stored in the
            // (USD/local) format while the rest are in the (local/USD)
            // So, normalize here.
            if (country == "Australia" or country == "Euro" or country == "Ireland"
                or country == "New Zealand" or country == "United Kingdom") {
                rateVal = (1 / rateVal);
            }
            // Initialize new country tree if a new country is read
            if (country != currentCountry) {
                cout << "Initialize new tree for: " << country << endl;
                countries->push_back(AVLTree(country));
                currentCountry = country;
            }
            // Insert new Node based on read data
            countries->back().insert(dateVal, country, rateVal);
        }
    }
    return countries;
}

int main() {
    int option;
    cout << "Enter: " << endl;
    cout << "1 for AVL Trees" << endl;
    cout << "2 for a hash map" << endl;
    cin >> option;
    // Use AVL Trees
    if (option == 1) {
        // Temporary variable to control number of iterations; also used to find what %
        // Node insertions are successful
        int capacity = 10000;
        vector<AVLTree> *countries = createAVLTrees(capacity);
        while (option != 3) {
            cout << "Enter: " << endl;
            cout << "1 to convert from USD to foreign currency" << endl;
            cout << "2 to convert from foreign currency to USD" << endl;
            cout << "3 to quit" << endl;
            cin >> option;
            if (option == 1) {
                bool countryExists = false;
                string country;
                int date;
                float amount;
                cout << "Enter country: ";
                cin >> country;
                for (int i = 0; i < countries->size(); i++) {
                    if (countries->at(i).getCountry() == country) {
                        cout << "Enter date in format YYYYMMDD: ";
                        cin >> date;
                        cout << "Enter amount of USD: ";
                        cin >> amount;
                        float conversion = countries->at(i).convertFromUSD(date, amount);
                        cout << "Conversion of " << amount << " USD to currency in " << country << " is ";
                        printf("%.2f",conversion);
                        cout << endl;
                        countryExists = true;
                        break;
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
            else if (option == 2) {
                bool countryExists = false;
                string country;
                int date;
                float amount;
                cout << "Enter country: ";
                cin >> country;
                for (int i = 0; i < countries->size(); i++) {
                    if (countries->at(i).getCountry() == country) {
                        cout << "Enter date in format YYYYMMDD: ";
                        cin >> date;
                        cout << "Enter amount of foreign currency: ";
                        cin >> amount;
                        float conversion = countries->at(i).convertToUSD(date, amount);
                        cout << "Conversion of " << amount << " currency in " << country << " to USD is ";
                        printf("%.2f",conversion);
                        cout << endl;
                        countryExists = true;
                        break;
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
        }
    }
    // Use second data structure, undecided
    else if (option == 2) {
        int capacity;
        float maxLoadFactor;

        cout << "Enter capacity of each hash table at start (ex. 5000):" << endl;
        cin >> capacity;
        cout << "Enter max load factor of each hash table (ex. 4.0):" << endl;
        cin >> maxLoadFactor;

        cout << "Reading in data...";
        vector<pair<string, HashTable>> tableByCountry = createHashTables(capacity, maxLoadFactor);
        cout << "done!" << endl << endl;

        while (option != 6) {
            string temp;
            cout << "Enter: " << endl;
            cout << "1 to convert from USD to foreign currency" << endl;
            cout << "2 to convert from foreign currency to USD" << endl;
            cout << "6 to quit" << endl;
            cin >> option;

            // Clears space from cin
            getline(cin, temp);
            if (option == 1) {
                bool countryExists = false;
                string country;
                string date;
                float amountUSD, amountForeign;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        cout << "Enter date in format YYYY-MM-DD:" << endl;
                        cin >> date;
                        cout << "Enter amount of USD:" << endl;
                        cin >> amountUSD;

                        int accessDate = stoi(cleanUpDate(date));
                        amountForeign =  amountUSD * tableByCountry[i].second[accessDate];
                        if (amountForeign > 0) {
                            cout << "Conversion of USD$";
                            printf("%.2f", amountUSD);
                            cout << " to the currency of " << country << " is ";
                            printf("%.2f", amountForeign);
                            cout << endl;
                            countryExists = true;
                            break;
                        } else {
                            cout << "No data exists for " << date << "." << endl;
                        }
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            } else if (option == 2) {
                bool countryExists = false;
                string country;
                string date;
                float amountUSD, amountForeign;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        cout << "Enter date in format YYYY-MM-DD:" << endl;
                        cin >> date;
                        cout << "Enter amount of foreign currency:" << endl;
                        cin >> amountForeign;

                        int accessDate = stoi(cleanUpDate(date));
                        amountUSD = amountForeign / tableByCountry[i].second[accessDate];
                        if (amountUSD > 0) {
                            cout << "Conversion of ";
                            printf("%.2f", amountForeign);
                            cout << " in the currency of " << country << " is USD$";
                            printf("%.2f", amountUSD);
                            cout << endl;
                            countryExists = true;
                            break;
                        } else {
                            cout << "No data exists for " << date << "." << endl;
                        }
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
        }
    }
    cout << "Successfully quit" << endl;
    return 0;
}
