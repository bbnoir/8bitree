#include "CodeLength.h"
#include <cmath>
#include <algorithm>

void CodeLength::init()
{
    for (int i = 0; i < SYM_NUM; i++)
        if (codeLength[i] > 0)
            symList.push_back(i);
    symNum = symList.size();
    lenSet = vector<vector<int>>(CL_MAX + 1, vector<int>());
    for (auto i : symList)
        lenSet[codeLength[i]].push_back(i);
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
    for (int i = 0; i < SYM_NUM; i++)
    {
        sum += pow(2, -codeLength[i]);
    }
    if (sum > 1)
    {
        cout << "Kraft inequality is not satisfied!" << endl;
        return false;
    }
    return sum <= 1;
}

void erase_victim(vector<int> &vec, int val)
{
    vec.erase(find(vec.begin(), vec.end(), val));
}

void CodeLength::modify(int times)
{
    srand(time(NULL));
    // prevCodeLength = codeLength;
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
}

vector<int> CodeLength::getCodeLength()
{
    return codeLength;
}

vector<int> &CodeLength::getCodeLengthRef()
{
    return codeLength;
}