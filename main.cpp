#include <fstream>
#include <vector>
#include "AVL.h"
#include "HashTable.h"
using namespace std;

string formatIntDate(int date) {
    string res, temp = to_string(date);
    for (int i = 0; i < temp.size(); i++) {
        // Inserts dashes
        if (res.size() == 4 || res.size() == 7)
            res += '-';
        res += temp[i];
    }
    return res;
}

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
    if (!inFile.is_open())
        inFile.open("daily.csv");
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
    // Use Hash Table for storage
    else if (option == 2) {
        int capacity;
        float maxLoadFactor;

        cout << "Enter capacity of each hash table at start (ex. 1000):" << endl;
        cin >> capacity;
        cout << "Enter max load factor of each hash table (ex. 3.0):" << endl;
        cin >> maxLoadFactor;

        cout << "Reading in data...";
        vector<pair<string, HashTable>> tableByCountry = createHashTables(capacity, maxLoadFactor);
        cout << "done!" << endl;

        while (option != 8) {
            cout << endl << "Enter:" << endl;
            cout << "1 to convert from USD to foreign currency." << endl;
            cout << "2 to convert from foreign currency to USD." << endl;
            cout << "3 to find date when a foreign currency was MOST valuable (compared to USD)." << endl;
            cout << "4 to find date when a foreign currency was LEAST valuable (compared to USD)." << endl;
            cout << "5 to add a conversion rate." << endl;
            cout << "6 to remove a conversion rate." << endl;
            cout << "7 to print hash table statistics for a country." << endl;
            cout << "8 to quit." << endl;
            cin >> option;

            // Clears space from cin after choosing option
            string temp;
            getline(cin, temp);
            if (option == 1) { // USD to foreign
                bool countryExists = false;
                string country;
                string date;
                float amountUSD, amountForeign;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        countryExists = true;
                        cout << "Enter date in format YYYY-MM-DD:" << endl;
                        cin >> date;

                        int accessDate = stoi(cleanUpDate(date));
                        double rate = tableByCountry[i].second[accessDate];
                        if (rate > 0) {
                            cout << "Enter amount of USD:" << endl;
                            cin >> amountUSD;
                            amountForeign = amountUSD * rate;

                            cout << "Conversion of USD$";
                            printf("%.2f", amountUSD);
                            cout << " to the currency of " << country << " is ";
                            printf("%.2f", amountForeign);
                            cout << "." << endl;
                            break;
                        } else {
                            cout << "No data exists for " << date << " in " << country << "." << endl;
                        }
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
            else if (option == 2) { // foreign to USD
                bool countryExists = false;
                string country;
                string date;
                float amountUSD, amountForeign;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        countryExists = true;
                        cout << "Enter date in format YYYY-MM-DD:" << endl;
                        cin >> date;

                        int accessDate = stoi(cleanUpDate(date));
                        double rate = tableByCountry[i].second[accessDate];
                        if (rate > 0) {
                            cout << "Enter amount of foreign currency:" << endl;
                            cin >> amountForeign;
                            amountUSD = amountForeign / rate;

                            cout << "Conversion of ";
                            printf("%.2f", amountForeign);
                            cout << " in the currency of " << country << " is USD$";
                            printf("%.2f", amountUSD);
                            cout << "." << endl;
                            break;
                        } else {
                            cout << "No data exists for " << date << " in " << country << "." << endl;
                        }
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
            else if (option == 3) { // Find most valuable date for currency
                bool countryExists = false;
                string country;
                int date;
                double mostValuableRate = INFINITY;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        HashTable table = tableByCountry[i].second;
                        countryExists = true;

                        // Most valuable rate is when less in foreign currency gets same/more of USD
                        int earliest = 19700101, latest = 20181231;
                        for (int j = earliest; j < latest; j++) {
                            if (table[j] > 0 && table[j] < mostValuableRate) {
                                mostValuableRate = table[j];
                                date = j;
                            }
                        }
                        cout << "The most valuable date for the currency of " << country << " is " << formatIntDate(date) << "." << endl;

                        float amountForeign = 1000.0f * table[date];
                        cout << "USD$1000 converted was ";
                        printf("%.2f", amountForeign);
                        cout << " in that currency." << endl;
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
            else if (option == 4) { // Find least valuable date for currency
                bool countryExists = false;
                string country;
                int date;
                double leastValuableRate = 0;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        HashTable table = tableByCountry[i].second;
                        countryExists = true;

                        // Least valuable rate is when more in foreign currency gets same/less of USD
                        int earliest = 19700101, latest = 20181231;
                        for (int j = earliest; j < latest; j++) {
                            if (table[j] > 0 && table[j] > leastValuableRate) {
                                leastValuableRate = table[j];
                                date = j;
                            }
                        }
                        cout << "The least valuable date for the currency of " << country << " is " << formatIntDate(date) << "." << endl;

                        float amountForeign = 1000.0f * table[date];
                        cout << "USD$1000 converted was ";
                        printf("%.2f", amountForeign);
                        cout << " in that currency." << endl;
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
            else if (option == 5) { // Adds a new conversion rate to table
                bool countryExists = false;
                string country, date;
                double conversionRate;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        HashTable& table = tableByCountry[i].second;
                        countryExists = true;
                        cout << "Enter date in format YYYY-MM-DD:" << endl;
                        cin >> date;
                        cout << "Enter conversion rate (currency/USD):" << endl;
                        cin >> conversionRate;

                        table.insert(stoi(cleanUpDate(date)), conversionRate);
                    }
                }
                if (!countryExists) {
                    int index = tableByCountry.size();
                    tableByCountry.push_back(make_pair(country, HashTable(capacity, maxLoadFactor)));
                    HashTable& table = tableByCountry[index].second;

                    cout << "Enter date in format YYYY-MM-DD:" << endl;
                    cin >> date;
                    cout << "Enter conversion rate (currency/USD):" << endl;
                    cin >> conversionRate;

                    table.insert(stoi(cleanUpDate(date)), conversionRate);
                }
            }
            else if (option == 6) { // Removes conversion rate from table by date
                bool countryExists = false;
                string country, date;
                cout << "Enter country:" << endl;
                getline(cin, country);
                for (int i = 0; i < tableByCountry.size(); i++) {
                    if (tableByCountry[i].first == country) {
                        HashTable& table = tableByCountry[i].second;
                        countryExists = true;

                        cout << "Enter date in format YYYY-MM-DD:" << endl;
                        cin >> date;

                        table.remove(stoi(cleanUpDate(date)));
                        cout << "Success!" << endl;
                    }
                }
                if (!countryExists) {
                    cout << "No data exists for " << country << "." << endl;
                }
            }
            else if (option == 7) { // Prints stats for specified country or all countries
                bool countryExists = false;
                string country;
                cout << "Enter country (or \"all\"):" << endl;
                getline(cin, country);
                if (country != "all") {
                    for (int i = 0; i < tableByCountry.size(); i++) {
                        if (tableByCountry[i].first == country) {
                            HashTable table = tableByCountry[i].second;
                            countryExists = true;

                            cout << "Current items: " << table.getCurrItems() << endl;
                            cout << "Current buckets: " << table.getNumBuckets() << endl;
                            cout << "Current load factor: " << table.getCurrLoadFactor() << endl;
                            cout << "Maximum load factor: " << table.getMaxLoadFactor() << endl;
                        }
                    }
                    if (!countryExists) {
                        cout << "No data exists for " << country << "." << endl;
                    }
                }
                else { // Prints stats for each country
                    for (int i = 0; i < tableByCountry.size(); i++) {
                        HashTable table = tableByCountry[i].second;
                        cout << "Stats for " << tableByCountry[i].first << "'s table:" << endl;
                        cout << "\tCurrent items: " << table.getCurrItems() << endl;
                        cout << "\tCurrent buckets: " << table.getNumBuckets() << endl;
                        cout << "\tCurrent load factor: " << table.getCurrLoadFactor() << endl;
                        cout << "\tMaximum load factor: " << table.getMaxLoadFactor() << endl;
                    }
                }
            }
        }
    }
    cout << "Successfully quit" << endl;
    return 0;
}
