#include <iostream>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string>

using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ',';
}
unordered_map<string, int> mathOperators = {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
    {"(", 0},
    {")", 0},
};

queue<string> Tokenized(string input) {
    queue<string> tokens;
    string curToken = "";

    for (char ch : input) {
        if (isdigit(ch)) {
            curToken += ch;
        }
        else if (ch == ' ') {
           
            continue;
        }
        else if ((ch == '-' && tokens.size() == 0 && curToken.empty())
            || (ch == '-' && isOperator(tokens.back()[0]) && curToken.empty())) {
            curToken += ch;
        }
        else {
            if (!curToken.empty()) {
                tokens.push(curToken);
            }
            tokens.push(string(1, ch));
            curToken = "";
        }
    }

    if (!curToken.empty()) {
        tokens.push(curToken);
    }

    return tokens;
}

int main() {
    string input = "12 + 34 * ( 56 - 78 )";
    queue<string> tokens = Tokenized(input);
    while (!tokens.empty()) {
        cout << tokens.front() << " ";
        tokens.pop();
    }
    cout << endl;

    return 0;
}

