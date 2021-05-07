//
//  project2.c
//  Project 2
//
//  Created by Christian Hackelman on 4/28/21.
//  Copyright © 2021 Christian Hackelman. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
struct process
{
    char pLab[8];
    long int memAlc;
    long int start;
    bool isFree;
};

struct process processes[1000];

int main(int argc, char * argv[]) {
    FILE *file; // .txt file from given arguments
    char *alg = NULL; // to store requested algorithm
    long int totalMem = 0; // to store amount of total memory to be allocated
    char *line = NULL; //to store each line from .txt file
    char *ptr1; // to use as pointer for totalMem
    char *ptr2; // to use as pointer for procMem
    size_t size; // to read file
    char procLabel[8]; // to store potential process label
    char procMem[16]; // to store potential amount of memory allocation
    int numProcs = 0; // to track # of processes allocated for
    int index = -1; // to search/step through array
    int lastAlloc = -1; // to store point of last allocation
    int assigned = 0; // to track # of non vacant areas in memory/array
    int algNum; // 0 = FIRSTFIT, 1 = BESTFIT, 2 = WORSTFIT, 3 = NEXTFIT
    long int freeSpace = 0; // to store amount of concurrent "free space"
    char *first = "FIRSTFIT";
    char *best = "BESTFIT";
    char *worst = "WORSTFIT";
    char *request = "REQUEST";
    char *release = "RELEASE";
    char *list = "LIST";
    char *avail = "AVAILABLE";
    char *find = "FIND";
    int bestWorstIndex = -1;
    
    // Store arguments in appropriate variables
    alg = argv[1];
    totalMem = strtoul(argv[2], &ptr1, 10);
    file = fopen(argv[3], "r");
    
    // Start variable for first struct (process) in array - always 0
    processes[0].start = 0;
    processes[0].memAlc = totalMem;
    processes[0].isFree = true;
    
    if (!file) {
        fprintf(stderr, "error opening the given file\n");
    }
    // Find out which algorithm we're dealing with
    if (strcmp(alg, first) == 0) {
        algNum = 0;
    }
    else if (strcmp(alg, best) == 0) {
        algNum = 1;
    }
    else if (strcmp(alg, worst) == 0) {
        algNum = 2;
    }
    else {
        algNum = 3;
    }
    //start reading file
    while (getline(&line, &size, file) > 0) {
        char *cmd1 = strtok(line, " ");
        bool space = false;
        if (strcmp(cmd1, request) == 0) {
            for (int e = 0; e < numProcs+1; ++e) {
                if (processes[e].isFree) {
                    space = true;
                    break;
                }
            }
            // We know this is a REQUEST-esque command and we can add another process to the array of structs
            // Search for instance of a "free space"; if none found, place at end.
            // Check to make sure enough space is allocated for current process
            // First, store variables for current process locally to be used for comparison logic
            strcpy(procLabel, strtok(NULL, " "));
            strcpy(procMem, strtok(NULL, "\n"));
            
            // Start search by looping through array starting at beginning (k = 0)
            // If algorithm is nextfit, adjust index by adding the value of the last entry's index; Check that this value does not exceed/equal current number of processes
            if (numProcs == 0) {
                index = 0;
            }
            // find first occurrnce of large enough "free space" for best/worstfit
            for (int x = 0; x < numProcs; ++x) {
                if (processes[x].isFree && processes[x].memAlc >= strtol(procMem, &ptr2, 10)) {
                    bestWorstIndex = x;
                    break;
                }
            }
            for (int k = 0; k < numProcs; ++k) {
                if (algNum == 3) { // NEXTFIT
                    if (k + lastAlloc < numProcs) {
                        if (processes[k+lastAlloc].isFree && processes[k+lastAlloc].memAlc >= strtol(procMem, &ptr2, 10)) {
                            index = k+lastAlloc;
                            break;
                        }
                    }
                }
                else { // FIRSTFIT, BESTFIT, WORSTFIT
                    if (processes[k].isFree && processes[k].memAlc >= strtol(procMem, &ptr2, 10)) {
                        if (algNum == 0) { // FIRSTFIT
                            index = k;
                            break;
                        }
                        else if (algNum == 1) { // BESTFIT
                            if ((processes[k].memAlc - strtol(procMem, &ptr2, 10)) <= (processes[bestWorstIndex].memAlc - strtol(procMem, &ptr2, 10))) {
                                bestWorstIndex = k;
                                index = k;
                            }
                        }
                        else { // WORSTFIT
                            if ((processes[k].memAlc - strtol(procMem, &ptr2, 10)) >= (processes[bestWorstIndex].memAlc - strtol(procMem, &ptr2, 10))) {
                                bestWorstIndex = k;
                                index = k;
                            }
                        }
                    }
                }
            }
            // If "free space" was found in the array that can hold the current process, store it
            // We also need to "shift" the structs in array after the point of placement to account for new "free space"
            // ^^ IFF the "free space" is larger than the requested allocated space
            if(index >= 0 && processes[index].memAlc > strtol(procMem, &ptr2, 10)) {
                ++numProcs;
                for (int z = numProcs; z > index; --z) {
                    strcpy(processes[z].pLab, processes[z-1].pLab);
                    processes[z].memAlc = processes[z-1].memAlc;
                    processes[z].start = processes[z-1].start;
                    processes[z].isFree = processes[z-1].isFree;
                }
                // Now, configure the variables appropriately for the new "free space"
                // Start is calculated by taking the old "free space" start and adding the amount of memory to be allocated for the to-be-added process
                processes[index+1].start = processes[index].start + strtol(procMem, &ptr2, 10);
                // isFree is set to true to distinguish itself as "free space"
                processes[index+1].isFree = true;
                // MemAlc of new "free space" is calculated by subtracting the amount of memory allocated for the to-be-added process from the memory allocated for the old "free space"
                processes[index+1].memAlc = processes[index].memAlc - strtol(procMem, &ptr2, 10);
                // Now that structs (processes) are shifted and new "free space" is configured we can store the new process in the appropriate space
                strcpy(processes[index].pLab, procLabel);
                processes[index].memAlc = strtol(procMem, &ptr2, 10);
                processes[index].isFree = false;
                printf("ALLOCATED %s %ld\n", processes[index].pLab, processes[index].start);
                lastAlloc = index;
                index = -1;
            }
            // If the memory allocation in "free space" is EQUAL to the requested memory allocation, store it without shifting elements of array or incrementing numProcs var
            else if (index >= 0 && processes[index].memAlc == strtol(procMem, &ptr2, 10)) {
                strcpy(processes[index].pLab, procLabel);
                processes[index].isFree = false;
                printf("ALLOCATED %s %ld\n", processes[index].pLab, processes[index].start);
                if (totalMem == strtol(procMem, &ptr2, 10)) {
                    ++numProcs;
                }
                lastAlloc = index;
                index = -1;
            }
            // At this point, we know a process is requesting memory allocation, but there is no "free space" to store it
            else {
                if (strtol(procMem, &ptr2, 10) <= totalMem - processes[numProcs].start && space) {
                    //need to store remaining space in next index
                    processes[numProcs+1].memAlc = processes[numProcs].memAlc- strtol(procMem, &ptr2, 10);
                    processes[numProcs+1].isFree = processes[numProcs].isFree;
                    processes[numProcs+1].start = processes[numProcs].start + strtol(procMem, &ptr2, 10);
                    //now store in old space
                    strcpy(processes[numProcs].pLab, procLabel);
                    processes[numProcs].memAlc = strtol(procMem, &ptr2, 10);
                    //processes[numProcs].start = (processes[numProcs-1].start + processes[numProcs-1].memAlc);
                    processes[numProcs].isFree = false;
                    if (processes[numProcs+1].memAlc != 0) {
                        lastAlloc = numProcs;
                    } else {
                        lastAlloc = 0;
                        processes[numProcs+1].isFree = false;
                    }
                    printf("ALLOCATED %s %ld\n", processes[numProcs].pLab, processes[numProcs].start);
                    ++numProcs;
                }
                else {
                    printf("FAIL REQUEST %s %s\n", procLabel, procMem);
                }
            }
        } //end of "REQUEST" command
        else if (strcmp(cmd1, release) == 0) {
            // We know we are releasing a process, but which one?
            strcpy(procLabel, strtok(NULL, "\n"));
            for (int j = 0; j < numProcs; ++j) {
                if (strcmp(procLabel, processes[j].pLab) == 0) {
                    if (processes[j].isFree) {
                        index = -1;
                        break;
                    }
                    processes[j].isFree = true;
                    index = j;
                    break;
                }
            }
            if (numProcs == 0) {
                index = -1;
            }
            if (index >= 0) {
                bool shiftNeeded = false;
                // Release was successful
                printf("FREE %s %ld %ld", processes[index].pLab, processes[index].memAlc, processes[index].start);
                for (int f = 0; f < numProcs; ++f) {
                    if (processes[f].isFree) {
                        freeSpace = processes[f].memAlc;
                        if (processes[f+1].isFree) {
                            shiftNeeded = true;
                            index = f;
                            freeSpace += processes[f+1].memAlc;
                            break;
                        }
                        freeSpace = 0;
                    }
                }
                if (shiftNeeded) {
                    for (int g = index+1; g < numProcs; ++g) {
                        strcpy(processes[g].pLab, processes[g+1].pLab);
                        processes[g].isFree = processes[g+1].isFree;
                        processes[g].memAlc = processes[g+1].memAlc;
                        processes[g].start = processes[g+1].start;
                    }
                    processes[index].memAlc = freeSpace;
                    --numProcs;
                }
                index = -1;
            } else {
                printf("FAIL RELEASE %s", procLabel);
            }
            printf("\n");
        } //end of "RELEASE" command
        else if (strcmp(cmd1, list) == 0) {
            // differentiate "LIST AVAILABLE"/"LIST ASSIGNED"
            char *cmd2 = strtok(NULL, "\n");
            if (strcmp(cmd2, avail) == 0) {
                for (int a = 0; a < numProcs; ++a) {
                    if (processes[a].isFree) {
                        freeSpace = processes[a].memAlc;
                        printf("(%ld, %ld) ", processes[a].memAlc, processes[a].start);
                        freeSpace = 0;
                    }
                }
                // Above loop only executes print statement if any "free space" is found
                // Account for "leftover space"
                if (processes[numProcs].memAlc == 0) {
                    bool vacancy = false;
                    for (int h = 0; h < numProcs; ++h) {
                        if (processes[h].isFree) {
                            vacancy = true;
                            break;
                        }
                    }
                    if (vacancy) {
                        // do nothing
                    } else {
                       printf("FULL");
                    }
                } else {
                    printf("(%ld, %ld)", processes[numProcs].memAlc, processes[numProcs].start); //took out -1 from indexes
                }
                printf("\n");
            } // End of "LIST AVAILABLE"
            // We know at this point, command is "LIST ASSIGNED"
            else {
                for (int c = 0; c < numProcs; ++c) {
                    if (!processes[c].isFree) {
                        ++assigned;
                        printf("(%s, %ld, %ld) ", processes[c].pLab, processes[c].memAlc, processes[c].start);
                    }
                }
                if (assigned == 0) {
                    printf("NONE");
                } else {
                    assigned = 0;
                }
                printf("\n");
            } // End of "LIST ASSIGNED"
        } // End of "LIST"
        else if (strcmp(cmd1, find) == 0) {
            // We know this is the "FIND" command
            strcpy(procLabel, strtok(NULL, "\n"));
            // Loop through array to find process with matching name
            bool isFound = false;
            for (int d = 0; d < numProcs; ++d) {
                if (strcmp(processes[d].pLab, procLabel) == 0) {
                    if (!processes[d].isFree) {
                        isFound = true;
                        printf("(%s, %ld, %ld)", processes[d].pLab, processes[d].memAlc, processes[d].start);
                    }
                }
            }
            if (!isFound) {
                printf("FAULT");
                isFound = false;
            }
            printf("\n");
        } // End of "FIND"
        else {
            //do nothing; comment read
        }
    } // End of while(getline)
    return 0;
}
                           
