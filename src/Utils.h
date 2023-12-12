#pragma once
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

void progressBar(int progress, int total);
void progressBar(string title, int progress, int total);
vector<pair<int, int>> sort_cl(vector<int> M);
bool checkTreeValid(vector<int> codeLength);
