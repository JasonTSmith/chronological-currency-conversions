#include <fstream>
#include <vector>
#include "AVL.h"
using namespace std;

int main() {
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
    vector<AVLTree> countries;

    // Read header line
    getline(reader, line);

    // Temporary variable to control number of iterations; also used to find what %
    // Node insertions are successful
    int capacity = 2000;
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
        date.erase(remove(date.begin(), date.end(), '-'),date.end());
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
                countries.push_back(AVLTree(country));
                currentCountry = country;
            }
            // Insert new Node based on read data
            countries.back().insert(dateVal, country, rateVal);
        }
    }
    // TO DELETE - log cout should be just less than next int, which is height
    // (ensuring AVL is self-balancing correctly)
    cout << log(countries.back().getSize())/log(2) << " < ";
    cout << height(countries.back().getRoot()) << endl;
    // Total sum, esp notable out of previously defined capacity
    int sum = 0;
    for (int i = 0; i < countries.size(); i++) {
        sum += countries[i].getSize();
    }
    cout << float(sum)/capacity;
    // \TO DELETE
    return 0;
}
