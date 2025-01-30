/*-------------------------------------------
Program 1: Totalistic Cellular Automaton
Course: CS 211, Fall 2024, UIC
Author: Shreya Ganguly
-------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>

const int WORLD_SIZE = 65;
int ternary[7]; // Declare global array that holds 7 status values

typedef struct cell_struct{
    int localSum; // total sum of local cells, [left] + [me] + [right]: possible values are 0-6
    int status;   // this cell's current status: 0, 1, or 2
    int count;    // running accumulated count of this cell's active status for all generations
} cell;

//return true if input rule is valid (0-2186)
//return false if input rule is invalid
bool setValArray(int valArray[7], int rule) {
    int remainder = 0;
    if(rule < 0 || rule > 2186){
        return false;
    }else{
        // If the rule number is valid, create ternary array while storing backwards
        printf("\nThe value array for rule #%d is ", rule);
        for(int i = 0; i < 7; i++){
            remainder = rule % 3;
            rule = rule / 3;
            valArray[i] = remainder;
        }
        // print the ternary values in opposite (correct) direction
        for(int i = 6; i >= 0; i--){
            if(valArray[i] != '\0'){
                printf("%d", valArray[i]);
            }else{
                printf("0");
            }
        }
        printf("\n");
        return true;
    }

}

// Sets the local sum = the sum of the left current and right statuses
void setSums(cell world[WORLD_SIZE]) {
    for(int i = 0; i < WORLD_SIZE; i++){
        if(i == 0){
            world[i].localSum = world[WORLD_SIZE - 1].status + world[i].status + world[i+1].status;
        }else if(i == WORLD_SIZE - 1){
            world[i].localSum = world[i-1].status + world[i].status + world[0].status;
        }else{
            world[i].localSum = world[i-1].status + world[i].status + world[i+1].status;
        }
    }

    return;
}

// update status using the ternary value at the local sum
// update count by adding the status
// update totalSum by adding the staus values for all the world members
int evolveWorld(cell world[WORLD_SIZE], int ruleValArray[7]) {
    int totalSum = 0;
    for(int i = 0; i < WORLD_SIZE; i++){
        world[i].status = ruleValArray[world[i].localSum];
        world[i].count += world[i].status;
        totalSum += world[i].status;
    }
    return totalSum;
}

int main() {
    cell world[WORLD_SIZE];
    int ruleNumber;
    bool validRuleNumber = true;

    printf("Welcome to the Totalistic Cellular Automaton!\n");

    // Prompt user for rule number 
    printf("Enter the rule # (0-2186): \n");
    scanf("%d", &ruleNumber);
    validRuleNumber = setValArray(ternary, ruleNumber);

    // Prompts user until a valid rule number is entered
    while(!validRuleNumber){
        printf("Enter the rule # (0-2186): \n");
        scanf("%d", &ruleNumber);
        validRuleNumber = setValArray(ternary, ruleNumber);
    }
    printf("\n");

    // Hardcoded: Print the evolution of all possible states
    printf("The evolution of all possible states are as follows:\n");
    printf("Local Sum:   6       5       4       3       2       1       0      \n");
    printf("States:    |+++|   |++-|   |++ |   |+- |   |-- |   |-  |   |   |    \n");
    printf("                   |+-+|   |+ +|   |+ -|   |- -|   | - |            \n");
    printf("                   |-++|   | ++|   |-+ |   | --|   |  -|            \n");
    printf("                           |+--|   | +-|   |+  |                    \n");
    printf("                           |-+-|   |- +|   | + |                    \n");
    printf("                           |--+|   | -+|   |  +|                    \n");
    printf("                                   |---|                            \n");
    
    // Print the New Status using the ternary representation
    // 0 - empty  1 - minus  2 - plus
    printf("New Status: ");
    for(int i = 6; i >= 0; i--){
        if(ternary[i] == 0){
            printf("| |     ");
        }else if(ternary[i] == 1){
            printf("|-|     ");
        }else{
            printf("|+|     ");
        }
    }
    printf("\n\n");

    // Prompts user for numGen between 1 and 49
    printf("Enter the number of generations (1-49): ");     
    int numGen;
    scanf("%d", &numGen);

    // While input is not between 1 and 49, prompt the user again
    while(numGen < 1 || numGen > 49){
        printf("\nEnter the number of generations (1-49): ");
        scanf("%d", &numGen);
    }
    
    // Prompts user for inital active cell value between 1 and 2
    printf("\nEnter the value (1 or 2) for the initial active cell: ");
    int initialStatusValue;
    scanf("%d", &initialStatusValue);

    // Prompts user until valid active cell value is entered
    while(initialStatusValue != 1 && initialStatusValue != 2){
        printf("\nEnter the value (1 or 2) for the initial active cell: ");
        scanf("%d", &initialStatusValue);
    }
    printf("\n\n");

    printf("Initializing world & evolving...\n");
    
    // Set initial status value for middle cell and 0 for other cells
    for(int i = 0; i < WORLD_SIZE; i++){
        if(i == WORLD_SIZE / 2){
            world[i].status = initialStatusValue;
        }else{
            world[i].status = 0;
        }
        // Set localSum
        setSums(world);
    }

    // Initalize count variable to 0
    for(int i = 0; i < WORLD_SIZE; i++){
        world[i].count = 0;
    }

    // Initalize count for middle cell and print the first generation
    if(initialStatusValue == 1){
        world[WORLD_SIZE / 2].count = initialStatusValue;
        printf("                                -                                 %d \n", world[WORLD_SIZE / 2].count);
    }else{
        world[WORLD_SIZE / 2].count = initialStatusValue;
        printf("                                +                                 %d \n", world[WORLD_SIZE / 2].count);
    }

    int totalSum;
    // Prints each generation based on status values
    for(int i = 0; i < numGen - 1; i++){
        // Calls evolveWorld to advance to next generation
        totalSum = evolveWorld(world, ternary);
        for(int j = 0; j < WORLD_SIZE; j++){
            if(world[j].status == 0){
                printf(" ");
            }else if(world[j].status == 1){
                printf("-");
            }else if(world[j].status == 2){
                printf("+");
            }
        }
        printf(" %d \n", totalSum);
        // Call setSums() to reset cell status
        setSums(world);
    }

    // Prints horizontal break line
    for(int i = 0; i < WORLD_SIZE; i++){
        printf("_");
    }
    printf("\n");

    // Print tens place of count variable
    for(int i = 0; i < WORLD_SIZE; i++){
        if(world[i].count / 10 != 0){
            printf("%d", world[i].count / 10);
        }else{
            printf(" ");
        }
    }
    printf("\n");

    // Print ones place of count variable
    for(int i = 0; i < WORLD_SIZE; i++){
        if(world[i].count / 10 == 0 && world[i].count % 10 == 0){
            printf(" ");
        }else{
            printf("%d", world[i].count % 10);
        }
    }

    printf("\n"); // end program with a newline

    return 0;
}
