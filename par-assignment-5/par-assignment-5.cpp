#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string>
#include <cmath>
#include <cctype>

using namespace std;



bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ',';
}

int getPriority(const std::string& oper) {
    if (oper == "+" || oper == "-") {
        return 1;
    }
    else if (oper == "*" || oper == "/") {
        return 2;
    }
    else if (oper == "pow" || oper == "abs" || oper == "min"||  oper == "max") {
        return 3;
    }
    return 0;
}

queue<string> Tokenized(string input) {
    queue<string> tokens;
    string curToken = "";

    for (size_t i = 0; i < input.size(); ++i) {
        if (isdigit(input[i])) {
            curToken += input[i];
        }
        else if (isalpha(input[i])) {
            string funcName = "";
            while (isalpha(input[i])) {
                funcName += input[i++];
            }
            --i;
            if (input[i] == '-'  || input[i] == '+' || input[i] == '*' || input[i] == '/'|| input[i] == '(' || input[i] == ')'  || input[i] == ',') {
                tokens.push(std::string(1, input[i]));
            }
            else {
                tokens.push(funcName);
            }
        }
        else if (input[i] == ' ') {
            continue;
        }
        else if ((input[i] == '-' && tokens.size() == 0 && curToken.empty())
                 || (input[i] == '-' && isOperator(tokens.back()[0]) && curToken.empty())) {
            curToken += input[i];
        }
        else {
            if (!curToken.empty()) {
                tokens.push(curToken);
            }
            tokens.push(string(1, input[i]));
            curToken = "";
        }
    }

    if (!curToken.empty()) {
        tokens.push(curToken);
    }

    return tokens;
}

queue<string> ReverseNotation(queue<string> tokens) {
    queue<string> outputQueue;
    stack<string> operatorStack;
    stack<int> argCountStack;

    while (!tokens.empty()) {
        string token = tokens.front();
        tokens.pop();

        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            outputQueue.push(token);
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/"
                 || token == "pow" || token == "abs" || token == "min" || token == "max") {
            while (!operatorStack.empty() && getPriority(operatorStack.top()) >= getPriority(token)) {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);

            if (token == "min" || token == "max") {
                argCountStack.push(2);
            }
            else if (token == "abs") {
                argCountStack.push(1);
            }
            else if (token == "pow") {
                argCountStack.push(2);
            }
        }
        else if (token == "(") {
            operatorStack.push(token);
        }
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.pop();

            if (!operatorStack.empty() && (operatorStack.top() == "pow" || operatorStack.top() == "abs"
                                           || operatorStack.top() == "min" || operatorStack.top() == "max")) {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            //argCountStack.pop();
        }
        else if (token == ",") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
        }
        else {
            operatorStack.push(token);
        }
    }

    while (!operatorStack.empty()) {
        outputQueue.push(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}


int evaluateRPN(queue<string> rpnTokens) {
    stack<int> resultStack;

    while (!rpnTokens.empty()) {
        string token = rpnTokens.front();
        rpnTokens.pop();

        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            resultStack.push(stoi(token));
        }
        else{
            if (token == "+") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(operand1 + operand2);
            }
            else if (token == "-") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(operand1 - operand2);
            }
            else if (token == "*") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(operand1 * operand2);
            }
            else if (token == "/") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(operand1 / operand2);
            }
            else if (token == "max") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(max(operand1, operand2));
            }
            else if (token == "min") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(min(operand1, operand2));
            }
            else if (token == "pow") {
                int operand2 = resultStack.top();
                resultStack.pop();
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(pow(operand1, operand2));
            }
            else if (token == "abs") {
                int operand1 = resultStack.top();
                resultStack.pop();
                resultStack.push(abs(operand1));
            }
        }
    }
    return resultStack.top();
}

int main() {
    string input;
    cout << "Enter expression: ";
    getline(cin, input);

    queue<string> tokens = Tokenized(input);
    queue<string> rpnTokens = ReverseNotation(tokens);

    int result = evaluateRPN(rpnTokens);
    cout << "Result: " << result << endl;

    return 0;
}
