#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Function prototypes
bool isSafeState(vector<vector<int>>& allocation, vector<vector<int>>& max, vector<vector<int>>& need,
                 vector<int>& available, vector<int>& safeSequence);

// Main Function
int main() {
    // File variables
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return 1;
    }

    // Variables
    int numProcesses, numResources;

    // Read number of processes and resource types
    inputFile >> numProcesses >> numResources;

    // Read Available resources
    vector<int> available(numResources);
    for (int i = 0; i < numResources; ++i) {
        inputFile >> available[i];
    }

    // Read Allocation matrix
    vector<vector<int>> allocation(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            inputFile >> allocation[i][j];
        }
    }

    // Read Max matrix
    vector<vector<int>> max(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            inputFile >> max[i][j];
        }
    }

    inputFile.close();

    // Calculate the Need matrix
    vector<vector<int>> need(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Safe sequence to store the result
    vector<int> safeSequence;

    // Run the safety algorithm
    if (isSafeState(allocation, max, need, available, safeSequence)) {
        cout << "The system is in a safe state.\nSafe sequence: ";
        for (int process : safeSequence) {
            cout << "P" << process << " ";
        }
        cout << endl;
    } else {
        cout << "The system is NOT in a safe state." << endl;
    }

    return 0;
}

// Function to check if the system is in a safe state
bool isSafeState(vector<vector<int>>& allocation, vector<vector<int>>& max, vector<vector<int>>& need,
                 vector<int>& available, vector<int>& safeSequence) {
    int numProcesses = allocation.size();
    int numResources = available.size();

    vector<bool> finish(numProcesses, false);
    vector<int> work = available;

    for (int count = 0; count < numProcesses; ++count) {
        bool found = false;

        for (int i = 0; i < numProcesses; ++i) {
            if (!finish[i]) {
                // Check if process i can be allocated
                bool canAllocate = true;
                for (int j = 0; j < numResources; ++j) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                // If process i can be allocated
                if (canAllocate) {
                    // Simulate allocation
                    for (int j = 0; j < numResources; ++j) {
                        work[j] += allocation[i][j];
                    }

                    safeSequence.push_back(i);
                    finish[i] = true;
                    found = true;
                }
            }
        }

        // If no process could be allocated, the system is not in a safe state
        if (!found) {
            return false;
        }
    }

    return true;
}