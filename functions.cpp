#include "functions.h"

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
    return a.second > b.second;
}

// Processes a dataset file and scores the products based on sentiments in the reviews
// Parameters: data file name, vector of pairs that stores product name and its score,
//             set of positive words, and set of negative words
void sent_analysis(string filename, vector<pair<string, int>>& allProducts,
         unordered_set<string>& positives, unordered_set<string>& negatives) {
    int index = -1;
    string tempStr, name, review;
    ifstream inReviews(filename);
    getline(inReviews, tempStr);      // skip first title line
    getline(inReviews, tempStr, '"'); // skip first newline char

    while(!inReviews.eof()) {
        getline(inReviews, name, '"');
        if (name.empty()) { // name has embedded comma
            getline(inReviews, name, '"');
        }
        getline(inReviews, tempStr, ',');
        getline(inReviews, tempStr, ',');
        getline(inReviews, review, '"');
        if (review.empty()) { // review has embedded comma
            getline(inReviews, review, '"');
            getline(inReviews, tempStr);
        }

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
                allProducts[index].second += 3;
            }
            else if (negatives.count(s)) {
                allProducts[index].second -= 3;
            }
        }
    } // end file reading
    inReviews.close();
}
