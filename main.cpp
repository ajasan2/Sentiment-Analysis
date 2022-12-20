#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;

// Data Source
// Word sentiments: https://www.kaggle.com/datasets/mukulkirti/positive-and-negative-word-listrar
// Product Reviews: https://www.kaggle.com/datasets/datafiniti/consumer-reviews-of-amazon-products
//                  https://www.kaggle.com/datasets/yasserh/amazon-product-reviews-dataset
//                  https://www.kaggle.com/code/nehaprabhavalkar/analysis-of-indian-product-reviews-on-amazon/data



// Checks if a string contains any letters
// Iterates through the string to check each character
// Parameter: any string (single word or words seperated by spaces)
bool alphaInStr (const string &str) {
    for (const char &c : str) {
        if (isalpha(c)) {
            return true;    // Returns true when first letter is encountered
        }
    }
    return false;   // Otherwise returns false
}

// Cleans a string: removes any punctuation from the beginning and end of a string, makes the string lowercase
// If string contains a letter, it is tokenized and returned. Else an empty string is returned.
// Parameter: any string (single word or words seperated by spaces)
// Return letter-containing cleaned string, else return an empty string
string cleanToken(string &s) {
    if (alphaInStr(s)) {
        // Get the index after all leading punctuation
        size_t index = 0;
        while (ispunct(s[index])) {
            index++;
        }
        // Overwrite string starting form the first letter
        s = s.substr(index);

        // Remove all punctuation from the back
        size_t reverseIndex = s.length() - 1;
        while (ispunct(s[reverseIndex])) {
            s.pop_back();
            reverseIndex--;
        }

        // Convert to lowercase
        for (char &c: s) {
            c = tolower(c);
        }
        return s;
    }
    else {
        return "";  // When string does not contain letters
    }
}

// Gathers individual tokens from a string and return them in a set
// Loop through each word. If it is a valid token, insert it in the set
// Parameter: any string (single word or words seperated by spaces)
unordered_set<string> gatherTokens(const string &text) {
    unordered_set<string> allTokens;
    stringstream bodyText(text);
    string token;

    while(bodyText >> token) {
        token = cleanToken(token);
        if (!token.empty()) {
            allTokens.insert(token);
        }
    }
    return allTokens;
}

// Check if token is one of the stop words
// Store stop words in a set and see if token matches any of those before being inserted into the index
// Parameters: token to be inserted in the index
// Return true if the token is one of the stop word else return false
bool stopWord (const string &token) {
    unordered_set<string> stopWords = {"the", "a", "an", "another", "for", "nor", "but", "and",
                             "yet", "so", "in", "under", "toward", "before", "if"};
    return stopWords.count(token);
}

// Comparator for sorting
bool cmp(pair<string, int>& a, pair<string, int>& b) {
    return a.second < b.second;
}


int main() {
    string word;
    unordered_set<string> positives;
    unordered_set<string> negatives;
    vector<pair<string, int>> allProducts;

    ifstream inSentiments("Positive and Negative Word List.csv");

    // File parsing and sentiment storage
    getline(inSentiments, word);
    getline(inSentiments, word);
    while (!inSentiments.eof()) {
        getline(inSentiments, word, ',');
        getline(inSentiments, word, ',');
        negatives.insert(word);
        getline(inSentiments, word);
        positives.insert(word);
    }
    inSentiments.close();

    // File parsing and reviews storage
    ifstream inReviews("Product Review Dataset.csv");
    getline(inReviews, word);      // skip first title line
    getline(inReviews, word, '"'); // skip first newline char
    string name, review;
    int index = -1;

    while(!inReviews.eof()) {
        getline(inReviews, name, '"');
        getline(inReviews, word, ','); // skip comma
        getline(inReviews, word, ','); // skip rating
        getline(inReviews, review, '"');

        // If this is the first or a different product, create a new entry
        if (allProducts.empty() || allProducts[index].first != name) {
            pair<string, int> newPair = {name, 0};
            allProducts.push_back(newPair);
            index++;
        }

        // Tokenize and score
        unordered_set<string> tokenSet = gatherTokens(review);
        for (auto& s : tokenSet) {
            if (positives.count(s)) {
                allProducts[index].second += 5;
            }
            else if (negatives.count(s)) {
                allProducts[index].second -= 5;
            }
        }
    }

    inReviews.close();
    sort(allProducts.begin(), allProducts.end(), cmp);

//    for (auto& p : allProducts) {
//        cout << p.first << " " << p.second << endl;
//    }
    return 0;
}