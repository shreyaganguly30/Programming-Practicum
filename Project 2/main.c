/*-------------------------------------------
Program 2: Food Web Analysis
Course: CS 211, Fall 2024, UIC
Author: Shreya Ganguly
-------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20]; // organism name
    int* prey; // dynamic array of indices  
    int numPrey; // number of preys
} Org;

// Function definitions
void printApex(Org* pWeb, int numOrg);
void printProducer(Org* pWeb, int numOrg);
void printFlexibleEaters(Org* pWeb, int numOrg);
void printHeights(Org* pWeb, int numOrg);
void classifyOrganisms(Org* pWeb, int numOrg);

// Adds to organisms to food web
void addOrgToWeb(Org** ppWeb, int* pNumOrg, char* newOrgName) {
    if(*ppWeb != NULL){
        Org* pWeb = (Org*)malloc((*pNumOrg + 1) * sizeof(Org));
        for(int i = 0; i < *pNumOrg; i++){
            pWeb[i] = (*ppWeb)[i];
        }
        free(*ppWeb);
        *ppWeb = pWeb;
        strcpy((*ppWeb)[*pNumOrg].name, newOrgName);
        (*ppWeb)[*pNumOrg].prey = NULL;
        (*ppWeb)[*pNumOrg].numPrey = 0;
    }else{
        *ppWeb = (Org*)malloc(sizeof(Org));
        strcpy((*ppWeb)[0].name, newOrgName);
        (*ppWeb)[0].prey = NULL;
        (*ppWeb)[0].numPrey = 0;
    }
    (*pNumOrg)++; // update pNumOrgs
}

bool addRelationToWeb(Org* pWeb, int numOrg, int predInd, int preyInd) {
    // If the indexes are not in bounds, return false
    if(predInd >= numOrg || predInd < 0 || preyInd >= numOrg || preyInd < 0 || predInd == preyInd){
        printf("Invalid predator and/or prey index. No relation added to the food web.");
        return false;
    }
    // If the prey index is already part of the pWeb[predInd].prey array, return false
    int arraySize = pWeb[predInd].numPrey; // size of prey array at predInd
    for(int i = 0; i < arraySize; i++){
        // if the prey array at i already holds the preyInd, return false
        if(pWeb[predInd].prey[i] == preyInd){
            printf("Duplicate predator/prey relation. No relation added to the food web.");
            return false;
        }
    }
    // If the prey array is not empty...
    if(arraySize != 0){
        int* pArray = (int*)malloc((arraySize + 1) * sizeof(int));
        // go thru every element of web until you find
        for(int i = 0; i < arraySize; i++){
            pArray[i] = pWeb[predInd].prey[i];
        }
        free(pWeb[predInd].prey);
        pArray[arraySize] = preyInd;
        pWeb[predInd].prey = pArray;
    }else{
        // If the prey array is empty...
        pWeb[predInd].prey = (int*)malloc(sizeof(int)); 
        pWeb[predInd].prey[0] = preyInd;
    }
    pWeb[predInd].numPrey++;
    return true;
}
    
bool removeOrgFromWeb(Org** ppWeb, int* pNumOrg, int index) {
    // Edge cases:
    // if invalid index, return false
    if(index < 0 || index >= *pNumOrg){
        return false;
    }
    // if ppWeb only has one organism, it is extinct and the food web is empty
    if(*pNumOrg == 1){
        free(*ppWeb);
        *ppWeb = NULL;
        *pNumOrg = 0;
        return true;
    }

    // if valid input, find organism and remove
    Org* pWeb = (Org*)malloc(((*pNumOrg) - 1) * sizeof(Org));
    int arrayIndex = 0; // need this because new array index is not same as for loop
    for(int i = 0; i < *pNumOrg; i++){
        if(i != index){
            pWeb[arrayIndex] = (*ppWeb)[i];
            arrayIndex++;
        }else{ // free the prey memory of the index, so there's no memory leaks
            if((*ppWeb)[i].prey != NULL){
                free((*ppWeb)[i].prey);
            }
        }
    }
    free(*ppWeb);
    (*ppWeb) = pWeb;
    (*pNumOrg)--;

    // Remove prey from web
    for(int i = 0; i < *pNumOrg; i++){ // go to each organism
        for(int j = 0; j < (*ppWeb)[i].numPrey; j++){ // go to each prey
            if((*ppWeb)[i].prey[j] == index){ // if they prey is the organism that needs to be removed...
                // create a new array pointer
                int* newPreys;
                if((*ppWeb)[i].numPrey > 1){ // prey array needs to have at least 1 organism
                    newPreys = (int*)malloc(((*ppWeb)[i].numPrey - 1) * sizeof(int));
                }else{
                    newPreys = NULL;
                }
                int position = 0;
                for(int k = 0; k < ((*ppWeb)[i].numPrey); k++){
                    if((*ppWeb)[i].prey[k] != index){
                        newPreys[position] = (*ppWeb)[i].prey[k];
                        position++;
                    }
                }
                free((*ppWeb)[i].prey);
                (*ppWeb)[i].prey = newPreys;
                ((*ppWeb)[i].numPrey)--;
            }
        }
    }

    // fix prey index
    for(int i = 0; i < *pNumOrg; i++){
        for(int j = 0; j < (*ppWeb)[i].numPrey; j++){
            if((*ppWeb)[i].prey[j] > index){
                (*ppWeb)[i].prey[j]--;
            }
        }
    }
    return true;
}

void freeWeb(Org* pWeb, int numOrg) {
    for(int i = 0; i < numOrg; i++){
        free(pWeb[i].prey);
    }
    free(pWeb);
}

// Print the Food Web Organisms with what they eat (i.e. prey)
void printWeb(Org* pWeb, int numOrg) {
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey == 0){
            printf("  (%d) %s", i, pWeb[i].name);
        }else{
            printf("  (%d) %s eats ", i, pWeb[i].name);
            for(int j = 0; j < pWeb[i].numPrey; j++){
                int preyIndex = pWeb[i].prey[j];
                if(j == pWeb[i].numPrey - 1){
                    printf("%s", pWeb[preyIndex].name);
                }else{
                    printf("%s, ", pWeb[preyIndex].name);
                }
            }
        }
        printf("\n");
    }
}

// Finds and prints the apex predators
void printApex(Org* pWeb, int numOrg){
    for(int i = 0; i < numOrg; i++){ // index of organism
        int isApexPred = true;
        for(int j = 0; j < numOrg; j++){ // index of all other organsims
            for(int k = 0; k < pWeb[j].numPrey; k++){ // index of all prey per organism
                if(pWeb[i].name == pWeb[pWeb[j].prey[k]].name){
                    isApexPred = false;
                }
            }
        }
        if(isApexPred){
            printf("  %s\n", pWeb[i].name);
        }
    }
}

// Prints producers
void printProducer(Org* pWeb, int numOrg){
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey == 0){
            printf("  %s\n", pWeb[i].name);
        }
    }
}

// Prints organism with the most prey
void printFlexibleEaters(Org* pWeb, int numOrg){
    int maxPrey = 0;
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey > maxPrey){
            maxPrey = pWeb[i].numPrey;
        }
    }
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey == maxPrey){
            printf("  %s\n", pWeb[i].name);
        }
    }
}

// Print organism eaten the most number of times
void printTastiest(Org* pWeb, int numOrg){
    int numTimesEaten[numOrg];
    for(int i = 0; i < numOrg; i++){
        numTimesEaten[i] = 0;
    }
    int maxEaten = 0;
    for(int i = 0; i < numOrg; i++){ //Organism1
        for(int j = 0; j < numOrg; j++){ //Organsim2
            for(int k = 0; k < pWeb[j].numPrey; k++){ //num of orgamism2 prey
                int preyIndex = pWeb[j].prey[k];
                if(pWeb[i].name == pWeb[preyIndex].name){
                    numTimesEaten[i] += 1;
                }
            }
        }
        if(numTimesEaten[i] > maxEaten){
            maxEaten = numTimesEaten[i];
        }
    }
    for(int i = 0; i < numOrg; i++){
        if(numTimesEaten[i] == maxEaten){
            printf("  %s\n", pWeb[i].name);
        }
    }
}

// Function to find the height of an organism recursively
int findPreyHeight(Org* pWeb, int* heights, int index) {
    // If the height is calculated (aka not the inital -1), return it
    if (heights[index] != -1) {
        return heights[index];
    }

    // If the organism is a producer (aka no preys), set height with height 0
    if (pWeb[index].numPrey == 0) {
        heights[index] = 0;
        return 0;
    }

    // Increment for the current organsim's number of preys 
    int maxPreyHeight = 0;
    for (int i = 0; i < pWeb[index].numPrey; i++) {
        // Find the index of each prey
        int preyIndex = pWeb[index].prey[i];
        // For each prey, call the function again to find it's height
        // The function will the called over and over until the producer is reached and go back from there
        int preyHeight = findPreyHeight(pWeb, heights, preyIndex);
        // Then for each organism with a greater height, the max height will be reset
        if (preyHeight > maxPreyHeight) {
            maxPreyHeight = preyHeight;
        }
    }

    // The height of the organism will be one more than the max height
    heights[index] = maxPreyHeight + 1;
    return heights[index];
}

// Recursive Approach: 
// Create an array of heights
// Initialize all heights to -1 to indicate that the height has not been set (this works because 0 is the smallest valid height)
//
// Create a recursive function that sets the height of each organism
// In the function, a for loop will increments numPrey times
// For each organism, if the prey has a valid integer, set the organism height to 1 + that, else call the set height function with the prey with invalid height.
void printHeights(Org* pWeb, int numOrg) {
    // Set all heights to -1
    int heights[numOrg];
    for (int i = 0; i < numOrg; i++) {
        heights[i] = -1;
    }

    // Call findPreyHeight for each organism
    for (int i = 0; i < numOrg; i++) {
        findPreyHeight(pWeb, heights, i);
    }

    // print food web heights
    for(int i = 0; i < numOrg; i++){
        printf("  %s: %d\n", pWeb[i].name, heights[i]);
    }
}

void classifyOrganisms(Org* pWeb, int numOrg){
    // Print producers
    printf("  Producers:\n");
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey == 0){
            printf("    %s\n", pWeb[i].name);
        }
    }
    // Print herbivores
    printf("  Herbivores:\n");
    for(int i = 0; i < numOrg; i++){
        bool eatsPlants = false;
        bool eatsAnimals = false;
        for(int j = 0; j < pWeb[i].numPrey; j++){
            int preyIndex = pWeb[i].prey[j];
            // eats plants
            if(pWeb[preyIndex].numPrey == 0){
                eatsPlants = true;
            }
            // eats animals
            if(pWeb[preyIndex].numPrey != 0){
                eatsAnimals = true;
            }
        }
        if(eatsPlants && !eatsAnimals){
            printf("    %s\n", pWeb[i].name);
        }
    }
    // Print omnivores
    printf("  Omnivores:\n");
    for(int i = 0; i < numOrg; i++){
        bool eatsPlants = false;
        bool eatsAnimals = false;
        for(int j = 0; j < pWeb[i].numPrey; j++){
            int preyIndex = pWeb[i].prey[j];
            // eats plants
            if(pWeb[preyIndex].numPrey == 0){
                eatsPlants = true;
            }
            // eats animals
            if(pWeb[preyIndex].numPrey != 0){
                eatsAnimals = true;
            }
        }
        if(eatsPlants && eatsAnimals){
            printf("    %s\n", pWeb[i].name);
        }
    }
    // Print carnivores
    printf("  Carnivores:\n");
    for(int i = 0; i < numOrg; i++){
        bool eatsPlants = false;
        bool eatsAnimals = false;
        for(int j = 0; j < pWeb[i].numPrey; j++){
            int preyIndex = pWeb[i].prey[j];
            // eats plants
            if(pWeb[preyIndex].numPrey == 0){
                eatsPlants = true;
            }
            // eats animals
            if(pWeb[preyIndex].numPrey != 0){
                eatsAnimals = true;
            }
        }
        if(!eatsPlants && eatsAnimals){
            printf("    %s\n", pWeb[i].name);
        }
    }
}

void displayAll(Org* pWeb, int numOrg, bool modified) {
    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(pWeb, numOrg); 
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
    printApex(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    printProducer(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");
    printFlexibleEaters(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    printTastiest(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    printHeights(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    classifyOrganisms(pWeb, numOrg);
    printf("\n");

}

// Takes user input to set boolean values
bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {
    bool basicChecked = false;
    bool debugChecked = false;
    bool quietChecked = false;
    char temp;
    for(int i = 1; i < argc; i++){
        temp = argv[i][1]; // copies letter input into temp char
        if(temp == 'b' && !basicChecked){
            *pBasicMode = true;
            basicChecked = true;
        }else if(temp == 'd' && !debugChecked){
            *pDebugMode = true;
            debugChecked = true;
        }else if(temp == 'q' && !quietChecked){
            *pQuietMode = true;
            quietChecked = true;
        }else{
            return false;
        }
    }
    return true;
}


void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}


int main(int argc, char* argv[]) {  
    
    // Initalize all the booleans to false
    bool basicMode = false;
    bool debugMode = false;
    bool quietMode = false;

    // If setModes returns false, print invalid statement
    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1;
    }

    // Print out original program settings
    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode);
    printf("  debug mode = ");
    printONorOFF(debugMode);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("\n");

    // Starting Food Web Application Code
    int numOrgs = 0;
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* pWeb = NULL;

    printf("Building the initial food web...\n");
    
    // Prompt user input for organism name
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = "";
    scanf("%s",tempName); 
    if (!quietMode) printf("\n");
    // While user input is not "DONE", keep prompting for organism name
    while (strcmp(tempName,"DONE") != 0) {
        addOrgToWeb(&pWeb,&numOrgs,tempName);
        if (debugMode) {
            printf("DEBUG MODE - added an organism:\n");
            printWeb(pWeb,numOrgs);
            printf("\n");
        }
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName); 
        if (!quietMode) printf("\n");
    }
    if (!quietMode) printf("\n");

    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");
    
    // Prompt user input for predator and prey index
    int predInd, preyInd;
    scanf("%d %d",&predInd, &preyInd);
    if (!quietMode) printf("\n");

    // Prompt user for new predator and prey index while invalid
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(pWeb,numOrgs,predInd,preyInd);
        if (debugMode) {
            printf("DEBUG MODE - added a relation:\n");
            printWeb(pWeb,numOrgs);
            printf("\n");
        }
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
        scanf("%d %d",&predInd, &preyInd);  
        if (!quietMode) printf("\n");
    }
    printf("\n");

    printf("--------------------------------\n\n");
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");
    
    // Print out all the groups of the food web and the organisms that fit within them
    displayAll(pWeb,numOrgs,false);

    // Provide user with input to modify the food web
    if (!basicMode) {
        printf("--------------------------------\n\n");
        printf("Modifying the food web...");
        char opt = '?';

        while (opt != 'q') {
            if (!quietMode) {
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");
            }
            scanf(" %c", &opt);
            printf("\n\n");

            if (opt == 'o') {
                char newName[20];
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName);
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&pWeb,&numOrgs,newName);
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'x') {
                int extInd;
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd);
                if (!quietMode) printf("\n");
                if (extInd >= 0 && extInd < numOrgs) {
                    printf("Species Extinction: %s\n", pWeb[extInd].name);
                    removeOrgFromWeb(&pWeb,&numOrgs,extInd);
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");
                
                if (debugMode) {
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'r') {
                int extInd;
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd);
                if (!quietMode) printf("\n");

                if (addRelationToWeb(pWeb,numOrgs,predInd,preyInd)) {
                    printf("New Food Source: %s eats %s\n", pWeb[predInd].name, pWeb[preyInd].name);
                };
                printf("\n");
                if (debugMode) {
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'p') {
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(pWeb,numOrgs);
                printf("\n");
                
            } else if (opt == 'd') {
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(pWeb,numOrgs,true);

            }
            printf("--------------------------------");
        
        }
        
    }

    // Free all malloc'd pWeb memory
    freeWeb(pWeb,numOrgs);

    //printf("\n\n");

    return 0;
}

