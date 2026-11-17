#include <iostream>
#include <string>
using namespace std;

struct Operation {
    char type;
    int  value;
    string name;
};

struct OpNode {
    string opName;
    OpNode* next;
};

void pushOp(OpNode*& top, string name) {
    OpNode* n = new OpNode;
    n->opName = name;
    n->next = top;
    top = n;
}

void popOp(OpNode*& top) {
    if (top == NULL) return;
    OpNode* temp = top;
    top = top->next;
    delete temp;
}

void printStack(OpNode* top) {
    if (top == NULL) { cout << "(empty)"; return; }
    string buf[50];
    int cnt = 0;
    OpNode* p = top;
    while (p != NULL && cnt < 50) {
        buf[cnt++] = p->opName;
        p = p->next;
    }
    for (int i = cnt - 1; i >= 0; i--) {
        cout << buf[i];
        if (i > 0) cout << " -> ";
    }
}

void findMacros(int cur, int target, int stepsLeft, int Mmax,
                Operation ops[], int nOps,
                OpNode*& stackTop, int& solutionNo, bool& found) {

    if (cur == target) {
        solutionNo++;
        cout << "  Solution " << solutionNo << ": ";
        printStack(stackTop);
        cout << "\n";
        found = true;
        return;
    }

    if (stepsLeft == 0) return;

    for (int i = 0; i < nOps; i++) {
        int next = cur;
        bool valid = true;

        switch (ops[i].type) {
            case '+': next = cur + ops[i].value; break;
            case '-': next = cur - ops[i].value; break;
            case '*': next = cur * ops[i].value; break;
            case '/':
                if (ops[i].value == 0) { valid = false; break; }
                next = cur / ops[i].value;
                break;
        }

        if (!valid) continue;
        if (next < 0) continue;
        if (next > Mmax) continue;

        pushOp(stackTop, ops[i].name);
        findMacros(next, target, stepsLeft - 1, Mmax,
                   ops, nOps, stackTop, solutionNo, found);
        popOp(stackTop);
    }
}

int main() {
    cout << "===== TASK 2: IDE Undo/Redo Macro Synthesizer =====\n";

    int Sstart = 5;
    int Starget = 20;
    int K = 4;
    int Mmax = 100;

    Operation ops[4] = {
        {'+', 3, "+3"},
        {'*', 2, "*2"},
        {'-', 5, "-5"},
        {'/', 2, "/2"}
    };
    int nOps = 4;

    cout << "Start = " << Sstart << ", Target = " << Starget
         << ", Max steps = " << K << ", Overflow bound = " << Mmax << "\n";
    cout << "Allowed ops: +3, *2, -5, /2\n\n";

    OpNode* stackTop = NULL;
    int solutionNo = 0;
    bool found = false;

    findMacros(Sstart, Starget, K, Mmax, ops, nOps, stackTop, solutionNo, found);

    if (!found) {
        cout << "No valid macro sequence found.\n";
    } else {
        cout << "\nTotal solutions found: " << solutionNo << "\n";
    }

    return 0;
}