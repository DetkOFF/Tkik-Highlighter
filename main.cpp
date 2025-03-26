#include <iostream>
#include <istream>
#include <fstream>
#include <map>
#include <string>
#include <list>

using namespace std;

enum ColorGroup {
    black = 0,
    red,
    green,
    blue,
    yellow,
};

struct TokenBase {
    string kod;
    string opis;
    ColorGroup color_group;
    TokenBase() : kod(""), opis(""), color_group(red) {}
    TokenBase(string _kod, string _opis, ColorGroup clr_grp) : kod(_kod), opis(_opis), color_group(clr_grp) {}
};

enum Stage {
    start = 0,
    varName, // _varName
    intValue, // 46548
    floatValue,  //* to be designed *//
    stringValue, // 'stringValue'
    assignmentOperator,
    ifKeyword, // if
    elseKeyword, // else
    whileKeyword, // while
    functionKeyword, // func
    returnKeyword, // return
};

Stage stan = start;
pair<TokenBase, string> currentToken = {TokenBase(),""};
int currentInd = 0;

int skaner(list <pair<TokenBase, string>> &tokeny, ifstream &fin) { //0 - end_of_line
    char currentChar = fin.get();
    // if (currentChar == ' ' && stan != stringValue) {currentInd++; return 2;} //symbol bialy

    string currentString = currentToken.second;
    switch (stan) {
        case start:
            if (isdigit(currentChar)) {
                stan = intValue;
                currentToken = {TokenBase("intVal", "integer value", green),
                    string(1, currentChar)};
            }
            else if (currentChar==' ' || currentChar=='\n') {
                return 2; //symbol bialy
            }
            else if (currentChar=='+') {
                currentToken = {TokenBase("+", "plus", black), "+"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='-') {
                currentToken = {TokenBase("-", "minus", black), "-"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='*') {
                currentToken = {TokenBase("*", "multiply", black), "*"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='/') {
                currentToken = {TokenBase("/", "divide", black), "/"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='=') {
                currentToken = {TokenBase("=", "equal to", black), "="};
                tokeny.push_back(currentToken);
            }
            else if (currentChar==':') {
                stan = assignmentOperator;
                currentToken = {TokenBase(":=", "assignment", black), ":"};
            }
            else if (currentChar=='(') {
                currentToken = {TokenBase("(", "left round bracket", yellow), "("};
                tokeny.push_back(currentToken);
            }
            else if (currentChar==')') {
                currentToken = {TokenBase(")", "right round bracket", yellow), ")"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='{') {
                currentToken = {TokenBase("{", "left curved bracket", blue), "{"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='}') {
                currentToken = {TokenBase("}", "right curved bracket", blue), "}"};
                tokeny.push_back(currentToken);
            }
            else if (currentChar=='_') {
                stan = varName;
                currentToken = {TokenBase("varName", "variable name", red), "_"};
            }
            else if (currentChar=='\'') {
                stan = stringValue;
                currentToken = {TokenBase("stringVal", "string value", green), "\'"};
            }
            else if (currentChar==EOF){// || currentChar=='\n') { //end_of_line
                return 0;
            }
            else if (currentChar=='i') {
                stan = ifKeyword;
                currentToken = {TokenBase("if", "if keyword", blue), "i"};
            }
            else if (currentChar=='e') {
                stan = elseKeyword;
                currentToken = {TokenBase("else", "else keyword", blue), "e"};
            }
            else if (currentChar=='w') {
                stan = whileKeyword;
                currentToken = {TokenBase("while", "while keyword", blue), "w"};
            }
            else if (currentChar=='f') {
                stan = functionKeyword;
                currentToken = {TokenBase("func", "function keyword", blue), "f"};
            }
            else if (currentChar=='r') {
                stan = returnKeyword;
                currentToken = {TokenBase("return", "return keyword", blue), "r"};
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case intValue:
            if (isdigit(currentChar)) {
                currentToken.second += string(1, currentChar);
            }
            else {
                tokeny.push_back(currentToken);
                fin.unget();
                stan = start;
                currentInd--;
            }
            break;
        case varName:
            if (isalnum(currentChar) || currentChar=='_') {
                currentToken.second += string(1, currentChar);
            }
            else {
                tokeny.push_back(currentToken);
                fin.unget();
                stan = start;
                currentInd--;
            }
            // else if (currentChar == '\"') {
            //     currentToken.second += string(1, currentChar);
            //     tokeny.push_back(currentToken);
            //     stan = start;
            //     currentInd--;
            // }
            // else {
            //     throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
            //     return -1;
            // }
            break;
        case stringValue:
            if (isalnum(currentChar) || currentChar == ' ') {
                currentToken.second += string(1, currentChar);
            }
            else if (currentChar == '\'') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
                currentInd--;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case ifKeyword:
            if (currentChar == 'f') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case elseKeyword:
            if ((currentString == "e" && currentChar == 'l') ||
                    (currentString == "el" && currentChar == 's')){
                currentToken.second += string(1, currentChar);
            }
            else if (currentString == "els" && currentChar == 'e') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case whileKeyword:
            if ((currentString == "w" && currentChar == 'h') ||
                    (currentString == "wh" && currentChar == 'i') ||
                    (currentString == "whi" && currentChar == 'l')){
                currentToken.second += string(1, currentChar);
            }
            else if (currentString == "whil" && currentChar == 'e') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case functionKeyword:
            if ((currentString == "f" && currentChar == 'u') ||
                        (currentString == "fu" && currentChar == 'n')){
                currentToken.second += string(1, currentChar);
            }
            else if (currentString == "fun" && currentChar == 'c') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case returnKeyword:
            if ((currentString == "r" && currentChar == 'e') ||
                    (currentString == "re" && currentChar == 't') ||
                    (currentString == "ret" && currentChar == 'u') ||
                    (currentString == "retu" && currentChar == 'r')){
                currentToken.second += string(1, currentChar);
            }
            else if (currentString == "retur" && currentChar == 'n') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
            break;
        case assignmentOperator:
            if (currentChar == '=') {
                currentToken.second += string(1, currentChar);
                tokeny.push_back(currentToken);
                stan = start;
            }
            else {
                throw invalid_argument("unexpected character [" + string(1, currentChar) +  "] at " + to_string(currentInd));
                return -1;
            }
    }
    currentInd++;
    return 1;
}
void colorize(list <pair<TokenBase, string>> &tokeny, ofstream &fout) {
    fout << "<!DOCTYPE html>" << endl;
    fout << "<style>\n .red{color: red;}\n .blue{color: blue;}\n .green{color: green;}\n .yellow{color: purple;}\n .black{color: black;}\n";
    fout << "body{\n display: flex;\n flex-direction: column;\nbackground-color: white;\n}\n </style>" << endl;
    fout << "<block>" << endl;
    for (auto tok: tokeny) {
        switch (tok.first.color_group) {
            case blue:
                fout << "</block>"<< endl << "<block>" << endl;
                fout << "<span class=\"blue\">";
                break;
            case green:
                fout << "<span class=\"green\">";
                break;
            case yellow:
                fout << "<span class=\"yellow\">";
                break;
            case red:
                fout << "<span class=\"red\">";
                break;
            case black:
                fout << "<span class=\"black\">";
                break;
        }
        fout << tok.second << " ";
        fout << "</span>" << endl;
    }
    fout << endl << "</block>" << endl;
    fout.close();
}

int main() {

    ifstream fin("input.txt");
    list<pair<TokenBase, string>> tokeny;
    while (skaner(tokeny, fin)) {
    }
    for (auto tok: tokeny) {
        cout << tok.first.kod << ": " << tok.second  << " (" << tok.first.opis << ")" << endl;
    }
    cout << "----------------------------" << endl;
    for (auto tok: tokeny) {
        cout << tok.second  << endl;
    }
    ofstream fout("output.html");
    colorize(tokeny, fout);
    return 0;
}