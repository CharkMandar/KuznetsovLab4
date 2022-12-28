#pragma once
#include<iostream>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <string>
using namespace std;


enum class States {
    WAIT_NUMBER,
    WAIT_NUMBER_OR_POINT,
    WAIT_NUMBER_OR_POINT_OR_OPERATOR,
    WAIT_POINT,
    WAIT_OPERATOR,
    WAIT_NUMBER_OR_MINUS,
    ERROR
};


bool multiple_points(string exp)
{
    int count = 0;
    for (int i = 1; i < exp.size(); i++) {
        if (exp[i] == '.' && isdigit(exp[i-1]))
            count++;
        else if (count > 1)
            return 0;
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '/' || exp[i] == '*' || exp[i] == '^' || exp[i] == '(')
            count = 0;

    }
    return count < 2;
}

bool correct_bracket(string exp) {
    int count = 0;
    for (int i = 0; i < exp.size(); i++) {
        if (exp[i] == '(') {
            count += 1;
            continue;
        }
        if (exp[i] == ')' && count != 0) {
            count -= 1;
            continue;
        }
        if (exp[i] == ')' && count == 0) {
            return 0;
        }
    }
    return count == 0;
}

bool correct_expression(string exp)
{
    //Проверка постановки скобок реализована в отдельной функции, так что здесь не будет вестись их подсчет. 
    //Указание отрицательного числа может быть только в скобках (не считая самого первого числа). Уравнение в форме 5*-1 не будет принято, только вида 5*(-1)

    States state;
    state = States::WAIT_NUMBER_OR_MINUS;
    size_t count = 0;
    for (int i = 0; i < exp.size(); i++) {
        char elem = exp[i];
        switch (state)
        {
        case States::WAIT_NUMBER_OR_MINUS: // начальная позиция набора числа 
            if (std::isdigit(elem))
                state = States::WAIT_NUMBER_OR_POINT_OR_OPERATOR;
            else if (elem == '-')
                state = States::WAIT_NUMBER;
            else if (elem == '(')
                state = States::WAIT_NUMBER_OR_MINUS; //зацикливаем, пока не дойдем до цифры или минуса (учитывается возможность ввода вида "((что то")
            else if (elem == ')')
                state = States::WAIT_OPERATOR;
            else
                state = States::ERROR;
            break;
        case States::WAIT_NUMBER: // заходим если увидели минус в состоянии ожидания числа или минуса
            if (std::isdigit(elem))
                state = States::WAIT_NUMBER_OR_POINT_OR_OPERATOR;
            else if (elem == '(')
                state = States::WAIT_NUMBER_OR_MINUS;
            else if (elem == ')' && exp[i - 1] != '.')
                state = States::WAIT_OPERATOR;
            else if (elem == ')' && exp[i - 1] == '.')
                state = States::ERROR;
            else
                state = States::ERROR;
            break;
        case States::WAIT_OPERATOR: /* переходим сюда только в случае, когда видим закрывающуюся скобку, учитывается вохзможность вида "))".
            Так как после нее может быть только оператор или еще одна скобка, все остальное вызовет ошибку*/
            if (elem == '+' || elem == '-' || elem == '*' || elem == '/' || elem == '^')
                state = States::WAIT_NUMBER;
            if (elem == ')')
                state = States::WAIT_OPERATOR;
            else
                state = States::ERROR;
        case States::WAIT_NUMBER_OR_POINT_OR_OPERATOR: //продолжаем собирать число, либо оператор
            if (elem == '.')
                state = States::WAIT_NUMBER;
            else if (elem == '+' || elem == '-' || elem == '*' || elem == '/' || elem == '^')
                state = States::WAIT_NUMBER;
            else if (std::isdigit(elem))
                state = States::WAIT_NUMBER_OR_POINT_OR_OPERATOR;
            else if (elem == '(')
                state = States::WAIT_NUMBER_OR_MINUS;
            else if (elem == ')')
                state = States::WAIT_OPERATOR;
            else
                state = States::ERROR;
            break;
        case States::ERROR: // если что то введено не правильно, выражение отправляется сюда 
            //std::cout << "this expression is incorrect";
            return 0;
            break;
        default:
            break;
        }
    }
    return 1;
}



enum elemtype {
    num, op_br, cl_br, op
};

struct element {
    string name;
    elemtype type;

    element(string str, elemtype typ) {
        name = str;
        type = typ;
    }
    element() {}
};

typedef vector<element> elements;

set<char> OperSet;
const string operators = " ()+/*-^";

bool isOperator(char el) {
    return OperSet.count(el) > 0;
}

void divide_expression_by_elements(string& expr, elements& texpr) {
    string ex = expr + " ";
    string name;
    for (int i = 0; i < operators.size(); i++)
        OperSet.insert(operators[i]);

    int i = 0;
    while (i < ex.size() - 1) {
        name = "";

        if (isOperator(ex[i])) {
            if (ex[i] == ' ') {
                i++;
                continue;
            }
            name = ex[i];
            i++;
        }

        else {
            while (!isOperator(ex[i])) {
                name += ex[i];
                i++;
            }
        }

        texpr.push_back(element(name, num));
    }

    for (int j = 0; j < texpr.size(); j++) {
        if (texpr[j].name[0] == '(') {
            texpr[j].type = op_br;
            continue;
        }
        if (texpr[j].name[0] == ')') {
            texpr[j].type = cl_br;
            continue;
        }
        if (isdigit(texpr[j].name[0])) {
            texpr[j].type = num;
            continue;
        }

        texpr[j].type = op;

    }

    for (int j = 0; j < texpr.size(); j++) {
        if (texpr[j].name == "-" && (j == 0 || texpr[j - 1].type == op_br))
            texpr[j].name = "~";
    }

    return;
}


map <string, int> prio;

void add_prio() {
    prio["+"] = 10;
    prio["-"] = 10;
    prio["*"] = 20;
    prio["/"] = 20;
    prio["^"] = 30;
    prio["~"] = 10; // унарный минус

}


void make_polish_notation(elements& texpr, elements& pexpr) {

    add_prio();
    stack <element> TStack;

    for (int i = 0; i < texpr.size(); i++) {
        switch (texpr[i].type) {
        case num:
            pexpr.push_back(texpr[i]);
            break;

        case op_br:
            TStack.push(texpr[i]);
            break;

        case cl_br:
            while (TStack.top().type != op_br) {
                pexpr.push_back(TStack.top());
                TStack.pop();
            }
            TStack.pop();
            break;

        case op:
            if (TStack.size()) {
                while (TStack.size() && ((TStack.top().type == op && prio[texpr[i].name] <= prio[TStack.top().name]))) {
                    pexpr.push_back(TStack.top());
                    TStack.pop();
                }
            }
            TStack.push(texpr[i]);
            break;

        }
    }

    while (TStack.size()) {
        pexpr.push_back(TStack.top());
        TStack.pop();
    }

    return;
}

typedef double(*func_type)(stack<double>&);

typedef map<string, func_type> Ops;
Ops ops;


double op_plus(stack <double>& s) {
    double a, b;
    a = s.top();
    s.pop();
    b = s.top();
    s.pop();
    return a + b;
}
double op_minus(stack <double>& s) {
    double a, b;
    a = s.top();
    s.pop();
    b = s.top();
    s.pop();
    return b - a;
}
double op_mul(stack <double>& s) {
    double a, b;
    a = s.top();
    s.pop();
    b = s.top();
    s.pop();
    return a * b;
}
double op_div(stack <double>& s) {
    double a, b;
    a = s.top();
    s.pop();
    b = s.top();
    s.pop();
    if (a == 0) {
        cout << "you cant divide by zero";
        return -1;
    }
    return b / a;
}
double op_deg(stack <double>& s) {
    double a, b;
    a = s.top();
    s.pop();
    b = s.top();
    s.pop();
    //b^a!!
    return pow(b, a);
}
double op_opposite(stack <double>& s) {
    double a;
    a = s.top();
    s.pop();
    return -a;
}

void createOperations() {
    ops["+"] = op_plus;
    ops["-"] = op_minus;
    ops["*"] = op_mul;
    ops["/"] = op_div;
    ops["^"] = op_deg;
    ops["~"] = op_opposite;



    return;
}

double calculate(elements& pexpr) {
    createOperations();
    stack <double> s;

    for (int i = 0; i < pexpr.size(); i++)
        switch (pexpr[i].type) {
        case num: {
            s.push(atof(pexpr[i].name.c_str()));
            break;


        case op:
            Ops::iterator Oit;
            for (Oit = ops.begin(); Oit != ops.end(); Oit++) {
                if (Oit->first == pexpr[i].name) {
                    s.push(Oit->second(s));
                    break;
                }
            }
            break;
        }
        }

    return s.top();
}



