/*-------------------------------------------
Program 3: Bit Big Bug Tug - Word Ladder Builder

Description: This program prints out a word ladder where each word is of the same length
and where each word higher up on the ladder has one different char than the pervious
word. The program first takes in all of the words of the desired length from a dictionary
and puts it into a word array while dynamically allocating space as needed. Then the program
creates a ladder with the words, where each next word only has one  char difference; this
ladder array is also allocated space for the number of valid words the user inputs. 

Course: CS 211, Fall 2024, UIC

Author: Shreya Ganguly
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Return the number of different chars between 2 strings
int strCmpCnt(char* word1, char* word2) {
    int differences = 0;
    // first compare each char for the number of overlapping lengths
    while (*word1 != '\0' && *word2 != '\0') {
        if (*word1 != *word2) {
            differences++;
        }
        word1++;
        word2++;
    }
    // if word1 is longer, increase the number of differneces by 1 for each different char
    while (*word1 != '\0') {
        differences++;
        word1++;
    }
    // if word2 is longer, increase the number of differneces by 1 for each different char
    while (*word2 != '\0') {
        differences++;
        word2++;
    }
    // return the number of differences between the words
    return differences;
}

// Return first index of different chars
int strCmpInd(char* word1, char* word2) {
    int index = 0;
    // while both words have not reached the null terminating character
    // if the chars are different, return that index
    while (*word1 != '\0' && *word2 != '\0') {
        if (*word1 != *word2) {
            return index;
        }
        word1++;
        word2++;
        index++;
    }
    // if one word is longer than the other, return the next index
    if(*word1 != '\0' || *word2 != '\0'){
        return index;
    }
    // else return -1
    return -1;
}

// append a new word to the end of the pWords array
// dynamically allocate more space max number of words has been reached
void appendWord(char*** pWords, int* pNumWords, int* pMaxWords, char* newWord){
    // if the number of words in the array has reached the max number of words
    if (*pNumWords == *pMaxWords){
        // create a new array with twice the space of the last
        char** pWordsNew = (char**)malloc((*pMaxWords * 2) * sizeof(char*));
        // transfer all the strings over to the new array
        for(int i = 0; i < *pMaxWords; i++){
            pWordsNew[i] = (*pWords)[i];
        }
        // free the original array and point it to the new array
        free(*pWords);
        *pWords = pWordsNew;
        // change the max words parameter, that was passed by pointer, to be twice its original size
        *pMaxWords *= 2;
    }
    // allocate space for the length of the new string plus an extra char for the null terminating char
    char* word = (char*)malloc((strlen(newWord) + 1) * sizeof(char));
    // copy the new word parameter into the a dynamically allocated char*
    strcpy(word, newWord);
    (*pWords)[*pNumWords] = word;
    // change the num words parameter, that was passed by pointer, to be one greater
    *pNumWords += 1;
}

// locate findWord in the words array and return the index
// if findWord is not in the array, return -99
int linSearchForWord(char** words, int numWords, char* findWord) { 
    // parse through the array and comapre each word to findWord
    for(int i = 0; i < numWords; i++){
        // if findWord is in the array, return the index
        if(strcmp(words[i], findWord) == 0){
            return i;
        }
    }
    // if findWord is not the array, return -99
    return -99;
}

// a valid word is wordLen long, is in the dictionary, and must have exactly one char different from the last word
// check to see if those requirements are met in that order, if not, return false and print the error statement
bool checkForValidWord(char** words, int numWords, int wordLen, char** ladder, int ladderHeight, char* aWord) {
    // first check if the word is DONE. If so, then stop adding words and leave the array incomplete
    if(strcmp(aWord, "DONE") == 0){
        printf("Stopping with an incomplete word ladder...\n");
        return true;
    }
    // if the word length does not match, return false because we will not add it to the ladder
    else if(strlen(aWord) != wordLen){
        printf("Entered word does NOT have the correct length. Try again...\n");
        return false;
    }
    // if the word is not in the dictionary, return false because we will not add it to the ladder
    else if(linSearchForWord(words, numWords, aWord) == -99){
        printf("Entered word NOT in dictionary. Try again...\n");
        return false;
    }
    // if the word does not have exactly one different char, return false because we will not add it to the ladder
    else if(strCmpCnt(ladder[ladderHeight - 1], aWord) != 1){
        printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
        return false;
    }
    // if you get to the else loop, then you have a valid word, so return true to add to the ladder
    else{
        printf("Entered word is valid and will be added to the word ladder.\n");
        return true;
    }
}

// returns a boolean that represents whether the ladder is complete (meaning the final word has been added)
bool isLadderComplete(char** ladderWords, int height, char* finalWord) {
    // if the final word is the last word in the array, then the ladder is complete, so return true
    if(strcmp(ladderWords[height - 1], finalWord) == 0){
        return true;
    }else{ // else, return false
        return false;
    }
}

// display the incomplete ladder (meaning the final word HAS NOT been inputted)
void displayIncompleteLadder(char** ladderWords, int height) {
    // an incomplete ladder will print 3 lines of ellipses
    printf("  ...\n");
    printf("  ...\n");
    printf("  ...\n");
    // after the three lines, print the words in the ladder
    // this for loop starts at the top of the ladder and prints down to the first word
    for(int i = height - 1; i >= 0; i--){
        printf("  %s\n", ladderWords[i]);
    }
}

// display the complete ladder (meaning the final word HAS been inputted)
void displayCompleteLadder(char** ladderWords, int height) {
    // the for loop starts at the top of the ladder and prints down to the first word
    for(int i = height - 1; i > 0; i--){
        // get the index of the first different letter between the words
        int index = strCmpInd(ladderWords[i], ladderWords[i-1]);
        // get the length of the string because we need to loop through each index to either print a ^ or a space
        int wordLength = strlen(ladderWords[i]);
        // print the word at index i, a new line, and 2 spaces
        printf("  %s\n  ", ladderWords[i]);
        // for each index in the word...
        for(int j = 0; j < wordLength; j++){
            // ...if the index matches j, then print the carrot
            if(j == index){
                printf("^");
            }
            // ...else, print a space
            else{
                printf(" ");
            }
        }
        // end with a new line
        printf("\n");
    }
    // we must print the last word separately outside the for-loop because it does not require the carrot if statement
    printf("  %s\n", ladderWords[0]);
}


int main(int argc, char* argv[]) {

    printf("\nProcessing command-line arguments...\n");

    int wordLen = 0;
    int maxLadder = 1;
    char dict[100] = "notAfile";     
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid"; 

    // Loop through all the command line arguments updating the variables
    // Loop runs for if there is a variable and a next variable (you need two inputs for any command -letter and input)
    // remember that argv is an array of chars, so for our integer variables, we must use atoi to convert the string input into an integer
    for (int i = 1; i < argc && i + 1 < argc; i = i + 2) {
        // if the input is -n, update the input length
        if (strcmp(argv[i], "-n") == 0) {
            int inputLength = atoi(argv[i + 1]);
            if (inputLength >= 2 && inputLength <= 20) {
                wordLen = inputLength;
            } else { // if the command line input is invalid, prompt the user again
                scanf("%d", &wordLen);
            }
        }
        // if the input is -m, update the max ladder height
        else if (strcmp(argv[i], "-m") == 0) {
            int ladderHeight = atoi(argv[i + 1]);
            if (ladderHeight >= 2) {
                maxLadder = ladderHeight;
            } else { // if the command line input is invalid, prompt the user again
                scanf("%d", &maxLadder);
            }
        }
        // if the input is -d, update the dictionary file
        else if (strcmp(argv[i], "-d") == 0) {
            strcpy(dict, argv[i + 1]);
        }
        // if the input is -s, update the starting word
        else if (strcmp(argv[i], "-s") == 0) {
            strcpy(startWord, argv[i + 1]);
        }
        // if the input is -f, update the final word
        else if (strcmp(argv[i], "-f") == 0) {
            strcpy(finalWord, argv[i + 1]);
        }
    }

    printf("Interactively (re)setting invalid parameters:\n");

    // set word length using interactive user-input if necessary
    int numInputs = 1;
    while (numInputs > 0 && (wordLen < 2 || wordLen > 20)) {
        printf("Invalid word length = %d\n", wordLen);
        printf("Enter a valid word length (must be 2-20): ");
        numInputs = scanf("%d",&wordLen);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }

    // set max ladder height using interactive user-input if necessary
    numInputs = 1;
    while (numInputs > 0 && maxLadder < 2) {
        printf("Invalid max ladder height = %d\n", maxLadder);
        printf("Enter a valid max ladder height (must be >1): ");
        numInputs = scanf("%d",&maxLadder);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    FILE* fileTry = fopen(dict, "r");
    numInputs = 1;
    while (numInputs > 0 && fileTry == NULL) {
        printf("Dictionary file %s not found...\n", dict);
        printf("Enter filename for dictionary: ");
        numInputs = scanf("%s", dict);
        printf("\n");
        fileTry = fopen(dict,"r");
    }
    fclose(fileTry);
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }
    
    // build the [words] array, a heap-allocated array of C-strings
    // where the words are a read-in from the dictionary file
    // and only those words of the desired length [wordLen] 

    int numWords = 0; // initially, no words in the array
    int maxWords = 4; // initially, capacity = 4

    // words is a array that stores all the words, from the dictionary, of size wordLen
    char** words = (char**)malloc(maxWords*sizeof(char*));
    
    // open the dictionary file and create a buffer that will hold each word
    FILE* inFILE = fopen(dict, "r");
    char word[20];
    // fscanf takes in input without the ending null char or new line
    // fscanf returns 1 when an extraction is successful
    while(fscanf(inFILE, "%s", word) == 1){
        // check if the input length of each word in the dictionary matches the length that we want
        int inputLength = strlen(word);
        // if the lengths match, then append the word to the words array
        if(inputLength == wordLen){
            appendWord(&words, &numWords, &maxWords, word);
        }
    }
    // close the dictionary file
    fclose(inFILE);

    printf("The dictionary contains %d words of length %d.\n",numWords, wordLen);
    printf("Max size of the dynamic words array is %d.\n", maxWords);
    printf("\n");

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("Dictionary %s contains insufficient %d-letter words...\n",dict,wordLen);
        printf("Terminating program...\n");
        return -1;
    }

    // resetting start word using interactive user-input if necessary
    numInputs = 1;
    while (numInputs > 0 && linSearchForWord(words,numWords,startWord) < 0) {
        printf("Start word %s is not valid...\n", startWord);
        printf("Enter a valid start word: ");
        numInputs = scanf("%s", startWord);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }

    // resetting final word using interactive user-input if necessary
    numInputs = 1;
    while (numInputs > 0 && linSearchForWord(words,numWords,finalWord) < 0 ) {
        printf("Final word %s is not valid...\n", finalWord);
        printf("Enter a valid final word: ");
        numInputs = scanf("%s", finalWord);
        printf("\n");
    }
    if (numInputs == 0) {
        printf("Invalid input. Cannot set program parameters. Terminating program...\n");
        return 0;
    }


    printf("\nWelcome to the CS 211 Word Ladder Builder!\n");
    printf("\n");

    printf("Your goal is to make a word ladder between two ");
    printf("%d-letter words: \n  %s -> %s\n\n",wordLen, startWord,finalWord);  
    
    // Allocating the word ladder for the maximum allowed height
    char** ladder = (char**)malloc(maxLadder*sizeof(char*));

    int ladderHeight = 0; // initially, the ladder is empty
    
    // add the start word to the ladder, i.e. the "bottom rung"
    appendWord(&ladder, &ladderHeight, &maxLadder, startWord);
    
    char aWord[30] = "XYZ";
    printf("\n");

    // while the ladderHeight has not reached the max ladder height, the user has not inputted DONE, and the ladder is still incomplete,
    // display the ladder and the ladder info and scan for a new word
    while (ladderHeight != maxLadder && strcmp(aWord, "DONE") != 0 && !isLadderComplete(ladder, ladderHeight, finalWord)) {
        printf("The goal is to reach the final word: %s\n",finalWord);
        printf("The ladder is currently: \n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Current ladder height: %d\n",ladderHeight);
        printf("Maximum ladder height: %d\n",maxLadder);
        printf("Enter the next word (or DONE to stop): ");
        scanf("%s", aWord);
        printf("\n");

        // Make sure the entered word is valid for the next ladder rung;
        // if not, repeatedly allow user to enter another word until one is valid
        while (!checkForValidWord(words, numWords, wordLen, ladder, ladderHeight, aWord)) {
            printf("Enter another word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
        }

        // add the entered word to the ladder (unless it is "DONE")
        if (strcmp(aWord,"DONE") != 0) {
            appendWord(&ladder,&ladderHeight,&maxLadder,aWord);
        }
        printf("\n");

    }

    // Check if the built word ladder is complete and 
    // display the word ladder appropriately. 
    if (isLadderComplete(ladder, ladderHeight, finalWord)) {
        printf("Word Ladder complete!\n");
        displayCompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you find a shorter Word Ladder next time??? \n");
    } else {
        if (ladderHeight == maxLadder) printf("The Word Ladder reached the maximum height.\n");
        printf("The final Word Ladder is incomplete:\n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you complete the Word Ladder next time??? \n");
    }
    
    // free heap memory for words by freeing each word in the array and the pointer
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    free(words);

    // free heap memory for ladder by freeing each word in the array and the pointer
    for(int i = 0; i < ladderHeight; i++){
        free(ladder[i]);
    }
    free(ladder);

    return 0;
}