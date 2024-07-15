#include <iostream> 
#include <iostream> 
#include <queue> 
#include <stack> 
#include <unordered_map> 
#include <string> 

using namespace std;

bool isOperator(char c) {
    return c == '+'  c == '-'  c == '*'  c == '/'  c == '('  c == ')'  c == ',';
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

queue<string> ReverseNotation(queue<string> tokens) {
    queue<string> numberQueue;
    stack<string> operationStack;
    int length = tokens.size();

    while (!tokens.empty()) {
        string currentElement = tokens.front();
        tokens.pop();

        if ((currentElement[0] == '-' && currentElement.length() > 1) || isdigit(currentElement[0])) {
            numberQueue.push(currentElement);
        }
        else if (currentElement == "(") {
            operationStack.push(currentElement);
        }
        else if (currentElement == ")") {
            while (!operationStack.empty() && operationStack.top() != "(") {
                numberQueue.push(operationStack.top());
                operationStack.pop();
            }
            operationStack.pop();
        }
        else {
            while (!operationStack.empty() && mathOperators[currentElement] <= mathOperators[operationStack.top()]) {
                numberQueue.push(operationStack.top());
                operationStack.pop();
            }
            operationStack.push(currentElement);
        }
    }

    while (!operationStack.empty()) {
        numberQueue.push(operationStack.top());
        operationStack.pop();
    }

    return numberQueue;
}

int resultNumber(queue<string> expression) {
    stack<int> resultStack;

    while (!expression.empty()) {
        string currElement = expression.front();
        expression.pop();

        if ((currElement[0] == '-' && currElement.length() > 1) || isdigit(currElement[0])) {
            int number = stoi(currElement);
            resultStack.push(number);
        }
        else if (isOperator(currElement[0])) {
            int operand2 = resultStack.top();
            resultStack.pop();
            int operand1 = resultStack.top();
            resultStack.pop();

            if (currElement == "+") {
                resultStack.push(operand1 + operand2);
            }
            else if (currElement == "-") {
                resultStack.push(operand1 - operand2);
            }
            else if (currElement == "*") {
                resultStack.push(operand1 * operand2);
            }
            else if (currElement == "/") {
                resultStack.push(operand1 / operand2);
            }
        }
    }

    return resultStack.top();
}

int main() {
    string input;
    cout << " Please? enter expression: " << endl;
    getline(cin, input);
    queue<string> tokens = Tokenized(input);
    queue<string> rpnTokens = ReverseNotation(tokens);


    int result = resultNumber(rpnTokens);
    cout << "Result: " << result << endl;

    return 0;
}