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

unordered_map<string, int> variables;

queue<string> Tokenized(string input, unordered_map<string, int>& variables) {
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
            auto contains = variables.find(funcName);
            if (contains != variables.end()) {
                curToken = to_string(contains->second);
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

int evaluateRPN(queue<string> rpnTokens) {
    stack<int> resultStack;

    while (!rpnTokens.empty()) {
        string token = rpnTokens.front();
        rpnTokens.pop();

        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            resultStack.push(stoi(token));
        }
        else {
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

queue<string> ReverseNotation(queue<string> tokens) {
    queue<string> outputQueue;
    stack<string> operatorStack;


    while (!tokens.empty()) {
        string token = tokens.front();
        tokens.pop();
        if (token == "var") {
            string var_name = tokens.front();
            tokens.pop();
            token = tokens.front();
            if (token == "=") {
                tokens.pop();
                queue<string> var_RPN = ReverseNotation(tokens);
                int value = evaluateRPN(var_RPN);
                variables[var_name] = value;
            }

        }
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


int main() {
    string input;
    while (input != "stop")
    {
        cout << "Enter expression: ";
        getline(cin, input);

        queue<string> tokens = Tokenized(input, va);
        queue<string> rpnTokens = ReverseNotation(tokens);

        int result = evaluateRPN(rpnTokens);
        cout << "Result: " << result << endl;
    }

    return 0;
}
