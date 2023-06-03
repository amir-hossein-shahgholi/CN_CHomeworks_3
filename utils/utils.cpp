#include "../include/utils.hpp"

vector<string> split(string text, char delimeter)
{
    vector<string> words;
    string temp = "";
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == delimeter)
        {
            words.push_back(temp);
            temp = "";
        }
        else
            temp += text[i];
    }
    words.push_back(temp);
    return words;
}

int digit_counter(int x)
{
    if (x == 0)
        return 1;
    int result = 0;
    while (x)
    {
        x /= 10;
        result++;
    }
    return result;
}