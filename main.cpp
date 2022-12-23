#include <algorithm>
#include "functions.h"

// This project aims to develop a sentiment analysis system for product rating. The main goal of this system is to
// understand the hidden sentiments of customers in feedback and comments and analyze their product rating patterns.
// When customers use this app to view products, product features, and comment on different products, the sentiment
// analysis system will analyze the comments of various users and ranks products accordingly. The system leverages
// a database of sentiment-based keywords (positively or negatively weighed). When a user comments on a particular
// product, the sentiment analysis system analyzes the keywords in the comment to find the match with the keywords
// stored in the database. After analyzing the matches against the positive and negative keywords and sentiments,
// the system ranks a product by adding or removing from the score, respectively.

// Data Source
// Word sentiments: https://www.kaggle.com/datasets/mukulkirti/positive-and-negative-word-listrar
// Product Reviews: https://www.kaggle.com/datasets/datafiniti/consumer-reviews-of-amazon-products
//                  https://www.kaggle.com/datasets/yasserh/amazon-product-reviews-dataset
//                  https://www.kaggle.com/code/nehaprabhavalkar/analysis-of-indian-product-reviews-on-amazon/data


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

    // File parsing and reviews sentiment scoring
    sent_analysis("Product Review Dataset.csv", allProducts, positives, negatives);
    sent_analysis("Product Review Dataset 2.csv", allProducts, positives, negatives);
    sent_analysis("Product Review Dataset 3.csv", allProducts, positives, negatives);
    sort(allProducts.begin(), allProducts.end(), cmp);

    // Top 10 products
    cout << "========= TOP 10 =========" << endl;
    for (int i = 0; i < 10; i++) {
        cout << i+1 << ". " << allProducts[i].first << endl;
    }

    return 0;
}
