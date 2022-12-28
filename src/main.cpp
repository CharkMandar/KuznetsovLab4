

#include<iostream>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <string>
#include "Iterator.h"

using namespace std;

int main() {
    cout << ">> You are using a simple calculator " << endl << ">> There are some operations you can use: " << endl << ">> Add -> +" << endl << ">> Substract -> -" <<
        endl << ">> Divide -> /" << endl <<
        ">> Multiply -> *" << endl << ">> Raise to a degree - ^" << endl << ">> To stop the calculator write 'stop' ";
    string expr = "";
    while (expr != "stop") {
        elements texpr, pexpr;
        cout << endl << ">> Input your expression: " << endl << ">> ";
        cin >> expr;

        if (correct_bracket(expr) == 0 || correct_expression(expr) == 0 || multiple_points(expr) == 0) {
            cout << ">> You entered the wrong expression, please check out your expression";
            continue;
        }

        divide_expression_by_elements(expr, texpr);

        make_polish_notation(texpr, pexpr);

        cout << ">> " << calculate(pexpr);
    }
    return 0;
}