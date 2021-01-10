/**
*
* Solution to course project # 4
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2020/2021
*
* @author Dochko Madzharov
* @idnumber 62523
* @compiler GCC
*
* <solution file>
*
*/

#include<iostream>
#include<fstream>
#include<string>
#include<stack>
#include<cmath>

using namespace std;

// RPN - Reversed Polish Notation

bool aroper(char c);    // checks if it is an arithmetic operator
int prior(char c);      // gives each operation a certain priority number

string createRPN(string t);     // creates the RPN
double calcRPN(string t);       // calculates the RPN

int main() {
    string math;
    ifstream problems("test.txt");
    if (!problems.is_open()) {
        return 1;
    }
    getline(problems,math);
    cout << math << endl;
    string rpn = createRPN(math);                            // first we create the RPN
    if (rpn != "Error") {
        double rez = calcRPN(rpn);                      // then we calculate its value
        if (rez != -999999999) {
            cout << rez << endl;
        }
    }
    return 0;
}

// function, which creates the RPN
// it uses a char type stack to store the operation signs "+, -, *, /, ^"
// in this way it rearranges everything according to the priority of the operations
string createRPN(string t) {
    stack <char> st;
    string rpn;
    for (int i = 0; i < (int)t.size(); i++) {
        if (t[i] == '(') {                                              // if it is an opening bracket'('
            st.push(t[i]);
        }
        else if (t[i]==')') {                                       // if it is an closing bracket ')'
            while (!st.empty() && st.top() != '(') {                  // until it finds a bracket '('
                rpn = rpn + st.top() + " ";
                st.pop();
            }
            if (st.empty()) {                                       // if there is no opening bracket before a closing one then it is not correct
                cout << "NaN" << endl;
                return "Error";
            }
            st.pop();
        }
        else if (aroper(t[i])) {                                     // it it is an arithmetic operator
            if (t[0] == '+' || t[0] == '-' || t[0] == '*' || t[0] == '^' || t[0] == '/') {
                cout << "Nan" << endl;
                return "Error";
            }

            while (!st.empty() && prior(st.top()) >= prior(t[i])) { // we extract from the stack the operations with higher or equal priority to the priority  of the current operation
                rpn = rpn + st.top() + " ";
                st.pop();
            }
            st.push(t[i]);
        }
        else if (isdigit(t[i])) {                                    // if it is a number(digit)
            while (i < (int)t.size() && isdigit(t[i])) {
                rpn += t[i];
                i++;
            }
            i--;
            rpn += " ";
        }
    }
    // until now the input string is checked every '(' has a corresponding ')' ; if there is '(' after the cycle, then there was not a ')' for it, thus the eqation is invalid
    while (!(st.empty())) {
        if (st.top()=='(') {                                      // if it finds '(' then there is not ')'
            cout << "NaN" << endl;
            return "Error";
        }
        rpn = rpn + st.top() + " ";
        st.pop();
    }
    return rpn;
}

// this function calculates the RPN
// uses a double type stack, which stores the numbers
// when the element is a number, it stores it and when it is an operation sign, it does it with the 2 numbers on the top of the stack
double calcRPN(string t) {
    stack <double> st;
    for (int i = 0; i < (int)t.size(); i++) {
        if (isdigit(t[i])) {             // if the element is a digit
            double num = 0;
            while (i < (int)t.size() && isdigit(t[i])) { // while there are consecutive elements which form a number it calculates the number
                num = num*10 + (t[i]-'0');
                i++;
            }
            i--;
            st.push(num);                              // the digits are finished and the number is added to the stack
        }
        else if (aroper(t[i])) {         // if the element is an operator
            if (st.size() < 2) {                          // if the numbers are less than two the RPN is incorrect
                cout << "NaN" << endl;
                return -999999999;
            }
            double a = st.top();                       // if the RPN is correct then it does the calculation with the top 2 numbers of the stack
            st.pop();                                  // it pops the top number in order to gain access to the number below it, and the result is stored on the top of the stack
            if (t[i] == '+') {
                st.top() += a;
            }
            else if (t[i] == '-') {
                    st.top() -= a;
            }
            else if (t[i] == '*') {
                    st.top() *= a;
            }
            else if (t[i] == '/') {
                if (st.top() == 0) {
                    cout << "Error" << endl;
                }
                st.top() /= a;
            }
            else if (t[i] == '^') {
                st.top() = pow(st.top(),a);
            }
        }
    }
    // after the calculations the stack must have only 1 value stored in it
    if (st.size() > 1) {                             // if the values in the stack are 2 or more, than the RPN is incorrect
        cout << "NaN" << endl;
        return -999999999;
    }
    return st.top();
}

bool aroper(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int prior (char c) {
    if (c == '+' || c == '-') {
        return 1;
    }
    if (c == '*' || c == '/') {
        return 2;
    }
    if (c == '^') {
        return 3;
    }
    return 0;
}
