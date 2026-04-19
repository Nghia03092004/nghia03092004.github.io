// IOI 1997 - Sentiment (Expression Evaluation)
// Recursive descent parser for boolean expressions with &, |, !
// Precedence: ! > & > |
// Time: O(n), Space: O(n) recursion stack
#include <bits/stdc++.h>
using namespace std;

char expr_str[10005];
int pos, len;
int vars[26]; // variable values a-z

int parseExpr();
int parseAnd();
int parseNot();
int parseAtom();

// expr -> and_expr ('|' and_expr)*
int parseExpr() {
    int val = parseAnd();
    while (pos < len && expr_str[pos] == '|') {
        pos++;
        val = val | parseAnd();
    }
    return val;
}

// and_expr -> not_expr ('&' not_expr)*
int parseAnd() {
    int val = parseNot();
    while (pos < len && expr_str[pos] == '&') {
        pos++;
        val = val & parseNot();
    }
    return val;
}

// not_expr -> '!' not_expr | atom
int parseNot() {
    if (pos < len && expr_str[pos] == '!') {
        pos++;
        return !parseNot();
    }
    return parseAtom();
}

// atom -> variable | '(' expr ')'
int parseAtom() {
    if (pos < len && expr_str[pos] == '(') {
        pos++; // skip '('
        int val = parseExpr();
        pos++; // skip ')'
        return val;
    }
    // variable: single lowercase letter
    int idx = expr_str[pos] - 'a';
    pos++;
    return vars[idx];
}

void stripSpaces(char* s) {
    int j = 0;
    for (int i = 0; s[i]; i++)
        if (s[i] != ' ')
            s[j++] = s[i];
    s[j] = '\0';
}

int main() {
    // Read expression
    fgets(expr_str, sizeof(expr_str), stdin);
    int slen = strlen(expr_str);
    if (slen > 0 && expr_str[slen - 1] == '\n') expr_str[slen - 1] = '\0';
    stripSpaces(expr_str);
    len = strlen(expr_str);

    // Read variable assignments
    int numVars;
    scanf("%d", &numVars);
    for (int i = 0; i < numVars; i++) {
        char c;
        int v;
        scanf(" %c %d", &c, &v);
        vars[c - 'a'] = v;
    }

    pos = 0;
    printf("%d\n", parseExpr());
    return 0;
}
