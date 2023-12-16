#include "CodeLength.h"
#include <cmath>
#include <algorithm>
#include <iomanip>

void CodeLength::init()
{
    for (int i = 0; i < SYM_NUM; i++)
        if (codeLength[i] > 0)
            symList.push_back(i);
    symNum = symList.size();
    lenSet = vector<vector<int>>(CL_MAX + 1, vector<int>());
    for (auto i : symList)
        lenSet[codeLength[i]].push_back(i);
    prevLenSet = vector<vector<int>>(CL_MAX + 1, vector<int>());
    for (int i = 0; i <= CL_MAX; i++)
        prevLenSet[i] = lenSet[i];
}

CodeLength::CodeLength()
{
    codeLength = vector<int>(SYM_NUM, 8);
    prevCodeLength = vector<int>(SYM_NUM, 8);
    init();
}

CodeLength::CodeLength(vector<int> cl)
{
    codeLength = cl;
    prevCodeLength = cl;
    init();
}

CodeLength::CodeLength(const CodeLength &cl)
{
    codeLength = cl.codeLength;
    prevCodeLength = cl.prevCodeLength;
    init();
}

ostream &operator<<(ostream &os, const CodeLength &cl)
{
    for (int i = 0; i < SYM_NUM; i++)
    {
        os << cl.codeLength[i] << " ";
        if (i % 64 == 63)
            os << endl;
    }
    return os;
}

bool CodeLength::checkKraft()
{
    double sum = 0;
    int offset = 16;
    vector<int> lenCount(CL_MAX + 1, 0);
    for (int i = 0; i < SYM_NUM; i++)
    {
        if (codeLength[i] > 0)
            sum += pow(2, -codeLength[i] + offset);
        lenCount[codeLength[i]]++;
    }
    // cout << endl;
    // cout << "Check Kraft: " << endl;
    // cout << "Length Count: " << endl;
    // for (int i = 1; i <= CL_MAX; i++)
    // {
    //     if (lenCount[i] == 0)
    //         continue;
    //     cout << i << ": " << lenCount[i] << endl;
    // }
    if (sum != double(pow(2, offset)))
    {
        cout << "Kraft inequality is not satisfied!" << endl;
        cout << resetiosflags(ios::fixed) << setprecision(10);
        cout << "Sum: " << sum << endl;
        return false;
    }
    return sum == double(pow(2, offset));
}

void erase_victim(vector<int> &vec, int val)
{
    vec.erase(find(vec.begin(), vec.end(), val));
}

void CodeLength::modify(int times)
{
    srand(time(NULL));
    prevCodeLength = codeLength;
    for (int i = 0; i <= CL_MAX; i++)
        prevLenSet[i] = lenSet[i];
    for (int t = 0; t < times; t++)
    {
        bool success = false;
        do
        {
            int sym = symList[rand() % symNum];
            int curLength = codeLength[sym];
            int mode = rand() % 4;
            int victim;
            switch (mode)
            {
            case 0: // [+1, -3, +2]
                if (curLength > 1 && curLength < CL_MAX && lenSet[curLength].size() >= 3)
                {
                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] -= 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] += 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] += 1;
                    lenSet[codeLength[victim]].push_back(victim);
                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                break;
            case 1: // [-1, +3, -2]
                if (rand() % 2 && curLength > 0 && curLength + 2 <= CL_MAX && lenSet[curLength].size() >= 1 && lenSet[curLength + 2].size() >= 2)
                {
                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] += 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    victim = lenSet[curLength + 2][rand() % lenSet[curLength + 2].size()];
                    erase_victim(lenSet[curLength + 2], victim);
                    codeLength[victim] -= 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    victim = lenSet[curLength + 2][rand() % lenSet[curLength + 2].size()];
                    erase_victim(lenSet[curLength + 2], victim);
                    codeLength[victim] -= 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                else if (curLength >= 3 && curLength <= CL_MAX && lenSet[curLength].size() >= 2 && lenSet[curLength - 2].size() >= 1)
                {
                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] -= 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] -= 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    victim = lenSet[curLength - 2][rand() % lenSet[curLength - 2].size()];
                    erase_victim(lenSet[curLength - 2], victim);
                    codeLength[victim] += 1;
                    lenSet[codeLength[victim]].push_back(victim);

                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                break;
            case 2: // [+1, -2, -1, +2]
                if (rand() % 2 && curLength >= 2 && curLength + 2 <= CL_MAX && lenSet[curLength].size() >= 2 && lenSet[curLength + 1].size() >= 1)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                        erase_victim(lenSet[curLength], victim);
                        codeLength[victim] += 2;
                        lenSet[codeLength[victim]].push_back(victim);
                    }

                    victim = lenSet[curLength + 1][rand() % lenSet[curLength + 1].size()];
                    erase_victim(lenSet[curLength + 1], victim);
                    codeLength[victim] -= 2;
                    lenSet[codeLength[victim]].push_back(victim);

                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                else if (curLength >= 3 && curLength + 1 <= CL_MAX && lenSet[curLength].size() >= 1 && lenSet[curLength - 1].size() >= 2)
                {
                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] -= 2;
                    lenSet[codeLength[victim]].push_back(victim);

                    for (int i = 0; i < 2; i++)
                    {
                        victim = lenSet[curLength - 1][rand() % lenSet[curLength - 1].size()];
                        erase_victim(lenSet[curLength - 1], victim);
                        codeLength[victim] += 2;
                        lenSet[codeLength[victim]].push_back(victim);
                    }

                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                break;
            case 3: //  [-1, +2, +1, -2]
                if (rand() % 2 && curLength >= 1 && curLength + 3 < CL_MAX && lenSet[curLength].size() >= 1 && lenSet[curLength + 3].size() >= 2)
                {
                    victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                    erase_victim(lenSet[curLength], victim);
                    codeLength[victim] += 2;
                    lenSet[codeLength[victim]].push_back(victim);

                    for (int i = 0; i < 2; i++)
                    {
                        victim = lenSet[curLength + 3][rand() % lenSet[curLength + 3].size()];
                        erase_victim(lenSet[curLength + 3], victim);
                        codeLength[victim] -= 2;
                        lenSet[codeLength[victim]].push_back(victim);
                    }
                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                else if (curLength >= 4 && curLength <= CL_MAX && lenSet[curLength].size() >= 2 && lenSet[curLength - 3].size() >= 1)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        victim = lenSet[curLength][rand() % lenSet[curLength].size()];
                        erase_victim(lenSet[curLength], victim);
                        codeLength[victim] -= 2;
                        lenSet[codeLength[victim]].push_back(victim);
                    }

                    victim = lenSet[curLength - 3][rand() % lenSet[curLength - 3].size()];
                    erase_victim(lenSet[curLength - 3], victim);
                    codeLength[victim] += 2;
                    lenSet[codeLength[victim]].push_back(victim);

                    // cout << endl;
                    // cout << "After modify:" << endl;
                    // cout << "Mode: " << mode << endl;
                    // this->showInfo();
                    if (!checkKraft())
                    {
                        throw std::invalid_argument("Kraft inequality is not satisfied!");
                        exit(1);
                    }

                    success = true;
                }
                break;
            default:
                cout << "Error: Invalid mode" << endl;
                exit(1);
            }
        } while (!success);
    }
}

void CodeLength::recover()
{
    codeLength = prevCodeLength;
    for (int i = 0; i <= CL_MAX; i++)
        lenSet[i] = prevLenSet[i];
}

vector<int> CodeLength::getCodeLength()
{
    return codeLength;
}

vector<int> &CodeLength::getCodeLengthRef()
{
    return codeLength;
}

void CodeLength::showInfo()
{
    cout << "Symbol number: " << symNum << endl;
    cout << "Length set: " << endl;
    double sum = 0;
    int offset = 16;
    cout << setiosflags(ios::fixed) << setprecision(10);
    for (int i = 0; i <= CL_MAX; i++)
    {
        if (lenSet[i].size() == 0)
            continue;
        cout << i << ": " << lenSet[i].size() << " - " << pow(2, -i) << " - " << lenSet[i].size() * pow(2, -i) << endl;
        sum += lenSet[i].size() * pow(2, -i + offset);
    }
    cout << "Sum: " << sum << endl;
}
