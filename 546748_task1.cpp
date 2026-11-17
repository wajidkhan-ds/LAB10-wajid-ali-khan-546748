#include <iostream>
using namespace std;

struct Container {
    int id;
    int weight;
    Container* next;
};

struct Truck {
    int capacity;
    Truck* next;
};

Container* buildContainerList(int ids[], int weights[], int n) {
    Container* head = NULL;
    Container* tail = NULL;
    for (int i = 0; i < n; i++) {
        Container* node = new Container;
        node->id = ids[i];
        node->weight = weights[i];
        node->next = NULL;
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

Truck* buildTruckQueue(int caps[], int n) {
    Truck* front = NULL;
    Truck* rear = NULL;
    for (int i = 0; i < n; i++) {
        Truck* t = new Truck;
        t->capacity = caps[i];
        t->next = NULL;
        if (front == NULL) {
            front = rear = t;
        } else {
            rear->next = t;
            rear = t;
        }
    }
    return front;
}

int remainingListWeight(Container* node) {
    int sum = 0;
    while (node != NULL) {
        sum += node->weight;
        node = node->next;
    }
    return sum;
}

int remainingTruckCapacity(Truck* t) {
    int sum = 0;
    while (t != NULL) {
        sum += t->capacity;
        t = t->next;
    }
    return sum;
}

int assignment[100];
int assignmentCount = 0;

bool loadContainers(Container* curContainer, Truck* truckList,
                    int usedTrucks, int truckLoads[], int truckCaps[]) {
    if (curContainer == NULL) {
        return true;
    }

    int remWeight = remainingListWeight(curContainer);
    int remCap = remainingTruckCapacity(truckList);
    if (remWeight > remCap) {
        return false;
    }

    for (int i = 0; i < usedTrucks; i++) {
        if (truckLoads[i] + curContainer->weight <= truckCaps[i]) {
            truckLoads[i] += curContainer->weight;
            assignment[assignmentCount++] = i;

            if (loadContainers(curContainer->next, truckList,
                               usedTrucks, truckLoads, truckCaps))
                return true;

            assignmentCount--;
            truckLoads[i] -= curContainer->weight;
        }
    }

    if (truckList != NULL) {
        if (curContainer->weight <= truckList->capacity) {
            int newIndex = usedTrucks;
            truckLoads[newIndex] = curContainer->weight;
            truckCaps[newIndex]  = truckList->capacity;
            assignment[assignmentCount++] = newIndex;

            if (loadContainers(curContainer->next, truckList->next,
                               usedTrucks + 1, truckLoads, truckCaps))
                return true;

            assignmentCount--;
            truckLoads[newIndex] = 0;
        }
    }

    return false;
}

int main() {
    cout << "===== TASK 1: Freight Convoy Partitioning =====\n";

    int ids[]     = {101, 102, 103, 104, 105, 106};
    int weights[] = {10,  20,  15,  25,  5,   30};
    int n = 6;

    int caps[] = {40, 40, 40};
    int m = 3;

    Container* head = buildContainerList(ids, weights, n);
    Truck* queue = buildTruckQueue(caps, m);

    assignmentCount = 0;

    int truckLoads[20];
    int truckCaps[20];
    for (int i = 0; i < 20; i++) { truckLoads[i] = 0; truckCaps[i] = 0; }

    int totalW = remainingListWeight(head);
    int totalC = remainingTruckCapacity(queue);
    cout << "Total container weight : " << totalW << "\n";
    cout << "Total truck capacity   : " << totalC << "\n\n";

    if (totalW > totalC) {
        cout << "Insufficient truck capacity.\n";
        return 0;
    }

    bool ok = loadContainers(head, queue, 0, truckLoads, truckCaps);

    if (!ok) {
        cout << "Insufficient truck capacity.\n";
    } else {
        cout << "Containers loaded successfully!\n";
        Container* p = head;
        int idx = 0;
        int trucksUsed = 0;
        for (int i = 0; i < assignmentCount; i++) {
            if (assignment[i] + 1 > trucksUsed) trucksUsed = assignment[i] + 1;
        }
        for (int t = 0; t < trucksUsed; t++) {
            cout << "  Truck " << (t + 1) << " (cap " << truckCaps[t] << "): ";
            p = head;
            idx = 0;
            while (p != NULL) {
                if (assignment[idx] == t)
                    cout << "[" << p->id << " w=" << p->weight << "] ";
                p = p->next;
                idx++;
            }
            cout << " | load = " << truckLoads[t] << "\n";
        }
    }

    return 0;
}