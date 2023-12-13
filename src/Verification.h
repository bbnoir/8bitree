#pragma once
#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "DataLoader.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

bool verify(DataLoader *dl, string decodedFileName);

bool verify_quiet(DataLoader *dl, string decodedFileName);

bool verify_bin(DataLoader *dl, string decodedFileName);

bool verify_bin_quiet(DataLoader *dl, string decodedFileName);