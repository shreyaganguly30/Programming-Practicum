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

// Test for total EVs sum
bool test_totalEVs() {
    State aStates[10];
    int totalVotes;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;
    aStates[6].electoralVotes = 2;
    aStates[7].electoralVotes = 15;

    // First 5 states should add up to 38 electoral votes
    printf(" Checking totalEVs() for 5 States\n");
    totalVotes = totalEVs(aStates, 5);
    if (totalVotes != 38) {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n", totalVotes);
        return false;
    }

    // First 6 states should add up to 48 electoral votes
    printf(" Checking totalEVs() for 6 States\n");
    totalVotes = totalEVs(aStates, 6);
    if (totalVotes != 48) {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n", totalVotes);
        return false;
    }

    // First 7 states should add up to 50 electoral votes
    printf(" Checking totalEVs() for 7 States\n");
    totalVotes = totalEVs(aStates, 7);
    if (totalVotes != 50) {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10, 2\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n", totalVotes);
        return false;
    }

    // First 8 states should add up to 65 electoral votes
    printf(" Checking totalEVs() for 8 States\n");
    totalVotes = totalEVs(aStates, 8);
    if (totalVotes != 65) {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10, 2, 15\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n",totalVotes);
        return false;
    }

    // if all test cases pass, return true
    return true;
}

// Test for correct PVs sum
bool test_totalPVs() {
    State aStates[10];
    int totalVotes;

    aStates[0].popularVotes = 10;
    aStates[1].popularVotes = 15;
    aStates[2].popularVotes = 20;
    aStates[3].popularVotes = 25;

    // First 2 states should add up to 25 popular votes
    printf(" Checking totalPVs() for 2 States\n");
    totalVotes = totalPVs(aStates, 2);
    if (totalVotes != 25) {
        printf("  individual state PVs are 10, 15\n");
        printf("  expected total PVs = 25, actual total EVs = %d\n", totalVotes);
        return false;
    }

    // First 3 states should add up to 45 popular votes
    printf(" Checking totalPVs() for 3 States\n");
    totalVotes = totalPVs(aStates, 3);
    if (totalVotes != 45) {
        printf("  individual state PVs are 10, 15, 20\n");
        printf("  expected total PVs = 45, actual total EVs = %d\n", totalVotes);
        return false;
    }

    
    // First 4 states should add up to 70 popular votes
    printf(" Checking totalPVs() for 4 States\n");
    totalVotes = totalPVs(aStates, 4);
    if (totalVotes != 70) {
        printf("  individual state PVs are 10, 15, 20, 25\n");
        printf("  expected total PVs = 70, actual total EVs = %d\n", totalVotes);
        return false;
    }

    // if all test cases pass, return true
    return true;
}

// Tests for correct boolean inialization for flags
bool test_setSettings() {
    // other variables
    int year = 0;
    bool fastMode = false;
    bool quietMode = false;

    // Test 1 - all good inputs
    int argc = 5;
    char* argv1[] = {"./app.exe", "-f", "-q", "-y", "2020"};
    bool output = setSettings(argc, argv1, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -f, -q, -y, 2020}\n");
    if(output != true || fastMode != 1 || quietMode != 1 || year != 2020){
        printf("  all valid inputs inputted\n");
        printf("  expected output = true, actual output = false\n");
        return false;
    }
    
    // Test 2 - invalid input
    argc = 5;
    char* argv2[] = {"./app.exe", "-f", "-v", "-y", "2020"};
    output = setSettings(argc, argv2, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -f, -v, -y, 2020}\n");
    if(output != false){
        printf("  -v is an invalid input\n");
        printf("  expected output = false, actual output = true\n");
        return false;
    }
    
    // Test 3 - all valid but -y is the last input with nothing after
    argc = 4;
    char* argv3[] = {"./app.exe", "-f", "-q", "-y"};
    output = setSettings(argc, argv3, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -f, -q, -y}\n");
    if(output != false || fastMode != 1 || quietMode != 1 || year != 0){
        printf("  -y has no year argument and is the last input\n");
        printf("  expected output = false, actual output = true\n");
        return false;
    }
  
    // Test 4 - all valid inputs without -f
    argc = 4;
    char* argv4[] = {"./app.exe", "-y", "55", "-q"};
    output = setSettings(argc, argv4, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -y, 55, -q}\n");
    if(output != true || fastMode != 0 || quietMode != 1 || year != 0){
        printf("  all valid inputs inputted\n");
        printf("  expected output = true, actual output = false\n");
        return false;
    }

    // Test 5 - all valid inputs but -f should not be initalized bc it's the year
    argc = 4;
    char* argv5[] = {"./app.exe", "-y", "-f", "-q"};
    output = setSettings(argc, argv5, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -y, -f, -q}\n");
    if(output != true){
        printf("  all valid inputs inputted\n");
        printf("  expected output = true, actual output = false\n");
        return false;
    }

    // Test 6 - no inputs
    argc = 1;
    char* argv6[] = {"./app.exe"};
    output = setSettings(argc, argv6, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe}\n");
    if(output != true){
        printf("  all valid inputs inputted\n");
        printf("  expected output = true, actual output = false\n");
        return false;
    }

    // Test 7 - only invalid input
    argc = 2;
    char* argv7[] = {"./app.exe", "-w"};
    output = setSettings(argc, argv7, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -w}\n");
    if(output != false || fastMode != 0 || quietMode != 0 || year != 0){
        printf("  -w is invalid\n");
        printf("  expected output = false, actual output = true\n");
        return false;
    }

    // Test 8 - just year with no initalization
    argc = 2;
    char* argv8[] = {"./app.exe", "-y"};
    output = setSettings(argc, argv8, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -y}\n");
    if(output != false){
        return false;
    }

    // Test 9 - year and initalized with a non voting year
    argc = 3;
    char* argv9[] = {"./app.exe", "-y", "2023"};
    output = setSettings(argc, argv9, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -y, 2023}\n");
    if(output != true){
        printf("  all valid inputs inputted\n");
        printf("  expected output = true, actual output = false\n");
        return false;
    }

    // Test 10 - year inialized with a flag
    argc = 3;
    char* argv10[] = {"./app.exe", "-y", "-f"};
    output = setSettings(argc, argv10, &year, &fastMode, &quietMode);
    printf(" Checking setSettings() for {./app.exe, -y, -f}\n");
    if(output != true){
        printf("  all valid inputs inputted\n");
        printf("  expected output = true, actual output = false\n");
        return false;
    }

    // if all test cases pass, return true
    return true;
}

// Correctly creates the name for a file to be read
bool test_inFilename() {
    // infile name for 2020 created correctly
    char filename1[25];
    int year1 = 2020;
    inFilename(filename1, year1);
    printf(" Checking inFilename() for year 2020\n");
    if(strcmp(filename1, "data/2020.csv") != 0){
        printf("  expected filename = 'data/2020.csv', actaul filename = '%s'\n", filename1);
        return false;
    }

    // infile name for 2024 created correctly
    char filename2[25];
    int year2 = 2024;
    inFilename(filename2, year2);
    printf(" Checking inFilename() for year 2024\n");
    if(strcmp(filename2, "data/2024.csv") != 0){
        printf("  expected filename = 'data/2024.csv', actaul filename = '%s'\n", filename2);
        return false;
    }

    // if all test cases pass, return true
    return true;
}

// Correctly creates the name for a file to be outputted to
bool test_outFilename() {

    // outfile name for 2020 created correctly
    char filename1[25];
    int year1 = 2020;
    outFilename(filename1, year1);
    printf(" Checking outFilename() for year 2020\n");
    if(strcmp(filename1, "toWin/2020_win.csv") != 0){
        printf("  expected filename = 'toWin/2020_win.csv', actaul filename = '%s'\n", filename1);
        return false;
    }

    // outfile name for 2024 created correctly
    char filename2[25];
    int year2 = 2024;
    outFilename(filename2, year2);
    printf(" Checking outFilename() for year 2024\n");
    if(strcmp(filename2, "toWin/2024_win.csv") != 0){
        printf("  expected filename = 'toWin/2024_win.csv', actaul filename = '%s'\n", filename2);
        return false;
    }

    // if all test cases pass, return true
    return true;
}

// Correctly initalizes the elements of State with a line
bool test_parseLine() {
    // myState1 should be initalized correctly with the info for Illinois
    State myState1;
    char illinoisLine[] = "Illinois,IL,19,100";
    bool valid = parseLine(illinoisLine, &myState1);
    printf(" Checking parseLine() for Illinois\n");
    if(valid != true || strcmp(myState1.name, "Illinois") != 0 || strcmp(myState1.postalCode, "IL") != 0 || myState1.electoralVotes != 19|| myState1.popularVotes != 100){
        printf("  expected information: Illinois IL 19 100\n");
        printf("  actual information: %s %s %d %d\n", myState1.name, myState1.postalCode, myState1.electoralVotes, myState1.popularVotes);
        return false;
    }

    // myState2 should be initalized correctly with the info for Texas
    State myState2;
    char texasLine[] = "Texas,TX,40,385";
    valid = parseLine(texasLine, &myState2);
    printf(" Checking parseLine() for Texas\n");
    if(valid != true || strcmp(myState2.name, "Texas") != 0 || strcmp(myState2.postalCode, "TX") != 0 || myState2.electoralVotes != 40|| myState2.popularVotes != 385){
        printf("  expected information: Texas TX 40 385\n");
        printf("  actual information: %s %s %d %d\n", myState2.name, myState2.postalCode, myState2.electoralVotes, myState2.popularVotes);
        return false;
    }

    // myState3 should be initalized correctly with the info for State
    State myState3;
    char stateLine[] = "State,ST,1";
    valid = parseLine(stateLine, &myState3);
    printf(" Checking parseLine() for State\n");
    if(valid != false){
        printf("  invalid input, should not update data\n");
        return false;
    }

    // if all test cases pass, return true
    return true;
}

// Opens a file and correctly initalizes the states
bool test_readElectionData() {
    // call for valid filename, so the elements within it should be initalized
    char filename1[20] = "data/1828.csv";
    int numStates1 = 0;
    State allStates1[51];
    bool successfulReading1 = readElectionData(filename1, allStates1, &numStates1);
    printf(" Checking readElectionData() for data/1828.csv\n");
    if(successfulReading1 != true || numStates1 != 24){
        printf("  Valid file - should have been read");
        return false;
    }
    if(strcmp(allStates1[0].name, "Alabama") != 0 || strcmp(allStates1[0].postalCode, "AL") != 0 || allStates1[0].electoralVotes != 5 || allStates1[0].popularVotes != 18618){
        printf("  Invalid intialization");
        return false;
    }

    // valid file so it should open and have the right number of states
    char filename2[20] = "data/2020.csv";
    int numStates2 = 0;
    State allStates2[51];
    bool successfulReading2 = readElectionData(filename2, allStates2, &numStates2);
    printf(" Checking readElectionData() for data/2020.csv\n");
    if(successfulReading2 != true || numStates2 != 51){
        printf("  Valid file - should have been read\n");
        return false;
    }

    // call for invalid filename, so the number of states in the sarray should still be 0
    char filename3[20] = "data/random.csv";
    int numStates3 = 0;
    State allStates3[51];
    bool successfulReading3 = readElectionData(filename3, allStates3, &numStates3);
    printf(" Checking readElectionData() for data/random.csv\n");
    if(successfulReading3 != false || numStates3 != 0){
        printf("  Invalid file - should not have been read");
        return false;
    }

    // if all test cases pass, return true
    return true;
}

bool test_minPVsSlow() {
    // define the states
    State states[4];
    strcpy(states[0].name, "Illinois");
    strcpy(states[0].postalCode, "IL");
    states[0].electoralVotes = 10;
    states[0].popularVotes = 20;
    strcpy(states[1].name, "Texas");
    strcpy(states[1].postalCode, "TX");
    states[1].electoralVotes = 20;
    states[1].popularVotes = 40;
    strcpy(states[2].name, "Florida");
    strcpy(states[2].postalCode, "FL");
    states[2].electoralVotes = 30;
    states[2].popularVotes = 60;
    strcpy(states[3].name, "California");
    strcpy(states[3].postalCode, "CA");
    states[3].electoralVotes = 55;
    states[3].popularVotes = 100;
    
    // need 50 EVs from the first 3 states - should result in 52 PVs
    MinInfo result = minPopVoteAtLeast(states, 3, 0, 50);
    // need Texas and Florida for 50 votes, so 21 + 31 popular votes
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != true || result.subsetPVs != 52 || result.szSomeStates != 2 || strcmp((result.someStates[1]).name, "Texas") != 0 || strcmp((result.someStates[0]).name, "Florida") != 0){
        printf("  expected popular votes = 52, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }
    
    // need 10 EVs from the first 3 states - should result in 11 PVs
    result = minPopVoteAtLeast(states, 3, 0, 10);
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != true || result.subsetPVs != 11 || result.szSomeStates != 1){
        printf("  expected popular votes = 11, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }

    // base case: start == szStates
    result = minPopVoteAtLeast(states, 0, 0, 50);
    // no popular votes will be added
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != false || result.subsetPVs != 0 || result.szSomeStates != 0){
        printf("  expected popular votes = 0, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }

    // base case : EVs = 0
    result = minPopVoteAtLeast(states, 3, 0, 0);
    // no popular votes will be added
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != true || result.subsetPVs != 0 || result.szSomeStates != 0){
        printf("  expected popular votes = 0, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }

    // reach last index and not enough EVs
    result = minPopVoteAtLeast(states, 3, 0, 100);
    // no popular votes will be added
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != false){
        printf("  expected: sufficientEVs = false\n");
        return false;
    }

    // need 60 EVs from the first 4 states - should result in 62 PVs
    result = minPopVoteAtLeast(states, 4, 0, 60);
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != true || result.subsetPVs != 62 || result.szSomeStates != 2){
        printf("  expected popular votes = 60, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }

    // call the minPopVoteToWin function, which calls minPopVoteAtLeast with it's own parameters
    result = minPopVoteToWin(states, 3);
    printf(" Checking minPopVoteAtLeast() for correct number PVs\n");
    if(result.sufficientEVs != true || result.subsetPVs != 42 || result.szSomeStates != 2){
        printf("  expected popular votes = 42, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }

    // if all test cases pass, return true
    return true;
}

bool test_minPVsFast() {
    // define the states
    State states[3];
    strcpy(states[0].name, "Illinois");
    strcpy(states[0].postalCode, "IL");
    states[0].electoralVotes = 10;
    states[0].popularVotes = 20;
    strcpy(states[1].name, "Texas");
    strcpy(states[1].postalCode, "TX");
    states[1].electoralVotes = 20;
    states[1].popularVotes = 40;
    strcpy(states[2].name, "Florida");
    strcpy(states[2].postalCode, "FL");
    states[2].electoralVotes = 30;
    states[2].popularVotes = 60;

    // call minPopVoteToWinFast function with 2 parameters so that memo is included
    MinInfo result = minPopVoteToWinFast(states, 3);
    // need 11 + 31 popular votes
    printf(" Checking minPopVoteAtLeastFast() for correct number PVs\n");
    if(result.sufficientEVs != true || result.subsetPVs != 42 || result.szSomeStates != 2){
        printf("  expected popular votes = 52, actaul popular votes = %d\n", result.subsetPVs);
        return false;
    }

    // if all test cases pass, return true
    return true;
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}