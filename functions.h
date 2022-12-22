#ifndef SENTIMENT_ANALYSIS_FUNCTIONS_H
#define SENTIMENT_ANALYSIS_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_set>
using namespace std;

bool alphaInStr (const string &str);
string cleanToken(string &s);
unordered_set<string> gatherTokens(const string &text);
bool stopWord (const string &token);
bool cmp(pair<string, int>& a, pair<string, int>& b);
void sent_analysis(string filename, vector<pair<string, int>>& allProducts,
                   unordered_set<string>& positives, unordered_set<string>& negatives);

#endif //SENTIMENT_ANALYSIS_FUNCTIONS_H