#include <iostream>
using namespace std;

struct Service {
    int id;
    int cost;
    Service* left;
    Service* right;
};

Service* insertBST(Service* root, int id, int cost) {
    if (root == NULL) {
        Service* n = new Service;
        n->id = id;
        n->cost = cost;
        n->left = n->right = NULL;
        return n;
    }
    if (cost < root->cost)
        root->left = insertBST(root->left, id, cost);
    else
        root->right = insertBST(root->right, id, cost);
    return root;
}

void findDownwardPaths(Service* node, int B,
                       int pathIds[], int pathCosts[], int pathLen,
                       int& pathCount) {
    if (node == NULL) return;

    pathIds[pathLen]   = node->id;
    pathCosts[pathLen] = node->cost;
    pathLen++;

    int sum = 0;
    for (int i = pathLen - 1; i >= 0; i--) {
        sum += pathCosts[i];
        if (sum == B) {
            pathCount++;
            cout << "  Path " << pathCount << ": ";
            for (int j = i; j < pathLen; j++) {
                cout << pathIds[j];
                if (j < pathLen - 1) cout << " -> ";
            }
            cout << "  (sum = " << B << ")\n";
        }
        if (sum > B) break;
    }

    findDownwardPaths(node->left,  B, pathIds, pathCosts, pathLen, pathCount);
    findDownwardPaths(node->right, B, pathIds, pathCosts, pathLen, pathCount);

    pathLen--;
}

int nodeIds[100];
int nodeCosts[100];
int nodeCount = 0;

void collectInOrder(Service* node) {
    if (node == NULL) return;
    collectInOrder(node->left);
    nodeIds[nodeCount] = node->id;
    nodeCosts[nodeCount] = node->cost;
    nodeCount++;
    collectInOrder(node->right);
}

void subsetSumBST(int index, int curSum, int B, bool chosen[], int& subCount) {
    if (curSum == B) {
        subCount++;
        cout << "  Subset " << subCount << ": { ";
        for (int i = 0; i < index; i++) {
            if (chosen[i]) cout << nodeIds[i] << " ";
        }
        cout << "}\n";
        return;
    }

    if (index >= nodeCount) return;
    if (curSum > B) return;

    if (curSum + nodeCosts[index] > B) {
        return;
    }

    chosen[index] = true;
    subsetSumBST(index + 1, curSum + nodeCosts[index], B, chosen, subCount);

    chosen[index] = false;
    subsetSumBST(index + 1, curSum, B, chosen, subCount);
}

int main() {
    cout << "===== TASK 3: Cloud Budget & Audit Path Finder =====\n";

    Service* root = NULL;
    root = insertBST(root, 1, 10);
    root = insertBST(root, 2,  5);
    root = insertBST(root, 3, 15);
    root = insertBST(root, 4,  3);
    root = insertBST(root, 5,  7);
    root = insertBST(root, 6, 12);
    root = insertBST(root, 7, 18);
    root = insertBST(root, 8,  2);

    int B = 15;

    cout << "\n-- Part A: Downward paths summing to " << B << " --\n";
    int pathIds[100];
    int pathCosts[100];
    int pathCount = 0;
    findDownwardPaths(root, B, pathIds, pathCosts, 0, pathCount);
    if (pathCount == 0) cout << "  No downward paths found.\n";

    cout << "\n-- Part B: BST subsets summing to " << B << " --\n";
    nodeCount = 0;
    collectInOrder(root);
    cout << "  In-order node costs: ";
    for (int i = 0; i < nodeCount; i++) cout << nodeCosts[i] << " ";
    cout << "\n";

    bool chosen[100];
    for (int i = 0; i < 100; i++) chosen[i] = false;
    int subCount = 0;
    subsetSumBST(0, 0, B, chosen, subCount);
    if (subCount == 0) cout << "  No subsets found.\n";

    return 0;
}