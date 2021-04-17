#include <fstream>
#include <vector>
#include "AVL.h"
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
    cout << "2 for " << endl;
    cin >> option;
    // Use AVL Trees
    if (option == 1) {
        // Temporary variable to control number of iterations; also used to find what %
        // Node insertions are successful
        int capacity = 10000;
        vector<AVLTree> *countries = createAVLTrees(capacity);
        while (option != 6) {
            cout << "Enter: " << endl;
            cout << "1 to convert from USD to foreign currency" << endl;
            cout << "2 to convert from foreign currency to USD" << endl;
            cout << "6 to quit" << endl;
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

    }
    cout << "Successfully quit" << endl;
    return 0;
}
