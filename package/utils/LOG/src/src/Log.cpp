//============================================================================
// Name        : Log.cpp
// Author      : rd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

int doSegment(const char *sentence)
{
    int count = 0;
    string str_cat = sentence;
    for (int i = 0; i < str_cat.length(); i++)
    {
        if (str_cat[i] == '\n')
        {
            count++;
        }
    }
    std::stringstream ss(sentence);
    std::string to;
    string log_rm = "";
    int i = 0;
    if (count > 3)
    {
        while (std::getline(ss, to, '\n') && i < (count - 3))
        {
            log_rm = "rm " + to;
            cout << log_rm << endl;
            const char * cmd = log_rm.c_str();
            system(cmd);
            ++i;
        }
    }
    return 0;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main()
{
    string output = exec((const char *)("find /root/log_RD*.txt"));
    doSegment(output.c_str());
    string outputBle = exec((const char *)("find /root/logfile*.log"));
    doSegment(outputBle.c_str());
    return 0;
}
