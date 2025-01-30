/*-------------------------------------------
Program 4: Popular Vote Minimizer Application

Description: This program allows a user to compile the program with flags that impact the output that is diaplyed.
For instance, you can use the flag -q while compling to prevent the program from  printing the election data for all
the states. After the user compiles and runs the program, depending on the flags, the program may output the election
data for each of the 51 states. Then the program will calculate and print a statistical summary of the election data
for a given year. A major part of this output is the Minimum Percentage of Popular Vote to Win Election, which answers
the central question. After printing, the program will write the subset of states needed to win to a file.

Course: CS 211, Fall 2024, UIC

Author: Shreya Ganguly
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

// counts number of electoral votes for specified number of states
int totalEVs(State* states, int szStates) {
    int numEVs = 0;
    // for the number of states, count the number of electoral votes
    for(int i = 0; i < szStates; i++){
        numEVs += states[i].electoralVotes;
    }
    // return number of electoral votes
    return numEVs;
}

// counts number of popular votes for specified number of states
int totalPVs(State* states, int szStates) {
    int numPVs = 0;
    // for the number of states, count the number of popular votes
    for(int i = 0; i < szStates; i++){
        numPVs += states[i].popularVotes;
    }
    // return number of popular votes
    return numPVs;
}

// sets the year, quiet mode, and fast mode settings
bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    // setting variables
    *year = 0; 
    *quietMode = false; 
    *fastMode = false;
    bool returnValue = true;
    // go thru the command-line arguments 
    for(int i = 1; i < argc; i++){
        // if -f is there, turn on fast mode
        if(strcmp(argv[i], "-f") == 0){
            *fastMode = true;
        }
        // if -q is there, turn on quiet mode
        else if(strcmp(argv[i], "-q") == 0){
            *quietMode = true;
        }
        // if -y is there, set the year
        else if(strcmp(argv[i], "-y") == 0){
            // if there is no next value, then returnValue = false
            if(i == argc - 1){
                returnValue = false;
            }
            // if the next input is a flag or is not divisble by 4, set year to 0
            else if(strcmp(argv[i+1], "-f") == 0 || strcmp(argv[i+1], "-q") == 0 || atoi(argv[i+1]) % 4 != 0){
                *year = 0; 
                i++; // increment to next
            }
            // if the next input is a valid year (divisible by 4), set the year
            else if(atoi(argv[i+1]) % 4 == 0 && atoi(argv[i+1]) > 0){
                *year = atoi(argv[i+1]);
                i++; // increment to next
            }
        }else{
            returnValue = false;
        }
    }

    // the return value should return true unless a bad year or a bad flag was inputted
    return returnValue;
}

// set the input file name to data/ + year + .csv
void inFilename(char* filename, int year) {
    // sprintf prints to a c-string
    sprintf(filename, "data/%d.csv", year);
}

// set the input file name to toWin/ + year + _win.csv
void outFilename(char* filename, int year) {
    sprintf(filename, "toWin/%d_win.csv", year);
}

bool parseLine(char* line, State* myState) {
    // via instructors answer to question @441 from Piazza
    // sscanf ignores \n chars
    // %[^,] reads until the defined char after ^
    // include the commas in the "" so that they aren't read
    int numAssigned = sscanf(line, "%[^,],%[^,],%d,%d", myState->name, myState->postalCode, &myState->electoralVotes, &myState->popularVotes);
    // sscanf returns the number of assigned variable, so if all 4 are assigned, return true
    if(numAssigned == 4){
        return true;
    }
    // else return false
    else{
        return false;
    }
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0; //required initialization prior to incrementing. 
    // open the file
    FILE* file = fopen(filename, "r");
    // if the file doesn't exist, return false
    if(file == NULL){
        return false;
    }
    char line[100];
    bool stateAdded;
    // while you can get another line...
    while(fgets(line, 100, file)){
        // call parseLine with that line and the x index of allStates
        stateAdded = parseLine(line, &allStates[*nStates]);
        // if a state was added, increment the index of allStates
        if(stateAdded){
            (*nStates)++;
        }
    }
    // close the file
    fclose(file);
    // return true bc a state was added
    return true;
}

// inputs: array of states, number of states, start index for states, additional electoral votes needed from the subset
MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    // Initalize to 0
    MinInfo base;
    base.szSomeStates = 0;
    base.subsetPVs = 0;

    // base case 1: if you need 0 or less electoral votes, set sufficient to true and return MinInfo
    if(EVs <= 0){
        base.sufficientEVs = true;
        return base;
    }
    //base case 2: if you have reached the last state, you don't have enough votes
    // set sufficient to false and return
    if(start == szStates){
        base.sufficientEVs = false;
        return base;
    }

    // Path 1: exclude the current state by incrementing your start and not decreasing the number of electoral votes needed
    MinInfo excludeState = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    // Path 2: include the current state then incrementing your start and decreasing the number of electoral votes needed
    MinInfo includeState = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
    // we only need half of the popular votes for the state plus 1 (majority)
    int neededPopularVotes = (states[start].popularVotes / 2) + 1;
    // increment the popular votes for the subset
    includeState.subsetPVs += neededPopularVotes;
    // increment the number of states in the subset
    includeState.szSomeStates++;
    // get the index of state that we are adding (# states added - 1)
    int stateIndex = includeState.szSomeStates - 1;
    // add the included state to the states array
    includeState.someStates[stateIndex] = states[start];
    
    // if both have enough EVs, return the one with the fewer popular votes
    if(includeState.sufficientEVs && excludeState.sufficientEVs){
        if(includeState.subsetPVs < excludeState.subsetPVs){
            return includeState;
        }
        else{
            return excludeState;
        }
    }
    // else if include has enough EVs, return include
    else if(includeState.sufficientEVs){
        return includeState;
    }
    // else return exlude
    else{
        return excludeState;
    }
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    // initialize variables
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    // call minPopVoteAtLeast
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    // Initalize to 0
    MinInfo base;
    base.szSomeStates = 0;
    base.subsetPVs = 0;

    // base case 1: if you need 0 or less electoral votes, set sufficient to true and return MinInfo
    if(EVs <= 0){
        base.sufficientEVs = true;
        return base;
    }
    //base case 2: if you have reached the last state, you don't have enough votes
    // set sufficient to false and return
    if(start == szStates){
        base.sufficientEVs = false;
        return base;
    }

    // if your current index for the past memory array has a valid value, return that value
    if(memo[start][EVs].subsetPVs != -1){
        return memo[start][EVs];
    }

    // Path 1: exclude the current state by incrementing your start and not decreasing the number of electoral votes needed
    MinInfo excludeState = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    // Path 2: include the current state then incrementing your start and decreasing the number of electoral votes needed
    MinInfo includeState = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    // we only need half of the popular votes for the state plus 1 (majority)
    int neededPopularVotes = (states[start].popularVotes / 2) + 1;
    // increment the popular votes for the subset
    includeState.subsetPVs += neededPopularVotes;
    // increment the number of states in the subset
    includeState.szSomeStates++;
    // get the index of state that we are adding (# states added - 1)
    int stateIndex = includeState.szSomeStates - 1;
    // add the included state to the states array
    includeState.someStates[stateIndex] = states[start];
    
    // if both have enough EVs, return the one with the fewer popular votes
    if(includeState.sufficientEVs && excludeState.sufficientEVs){
        if(includeState.subsetPVs < excludeState.subsetPVs){
            memo[start][EVs] = includeState; // save includeState
            return includeState;
        }
        else{
            memo[start][EVs] = excludeState; // save excludeState
            return excludeState;
        }
    }
    // else if include has enough EVs, return include
    else if(includeState.sufficientEVs){
        memo[start][EVs] = includeState; // save includeState
        return includeState;
    }
    // else return exlude
    else{
        memo[start][EVs] = excludeState; // save excludeState
        return excludeState;
    }
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    // initialize variables
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    // create memoization variable and initalize the uninitalized variables to -1
    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    // call minPopVoteAtLeastFast
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);
    
    // Free the memo malloc space
    for(int i = 0; i < szStates + 1; i++){
        free(memo[i]);
    }
    free(memo);

    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    // open file for writing
    FILE* writingFile = fopen(filenameW, "w");
    // check if file was opened
    if(writingFile == NULL){
        return false; // if file was not opened
    }
    // print first line to "toWin/[year]_win.csv" with [TotalEVs],[TotalPVs],[EVsWon],[PVsWon]
    fprintf(writingFile, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs,toWin.subsetPVs);
    // for toWin.szSomeStates times, print the [stateName],[postalCode],[electoralVotes],[popularVotesToWinState]
    for(int i = toWin.szSomeStates - 1; i >= 0; i--){
        int popularVotesInState = (toWin.someStates[i]).popularVotes;
        fprintf(writingFile, "%s,%s,%d,%d\n", (toWin.someStates[i]).name, (toWin.someStates[i]).postalCode, (toWin.someStates[i]).electoralVotes, (popularVotesInState / 2) + 1);
    }
    // close the file
    fclose(writingFile);
    // return true after printing to file
    return true;
}
