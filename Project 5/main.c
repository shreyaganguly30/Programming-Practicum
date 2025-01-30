/*-------------------------------------------
Program 5: Find the Shortest Word Ladder

Description: This program finds the shortest word ladder between two words from a given
dictionary. That is, for each word from the start to final word, the program will change
one letter at a time until it arrives at the final word. This program also allows for
command line arguments for testing, so if you use the -t flag, you can test each indiviual
function to locate any errors. This program was an introduction into linked lists as it
includes a WordNode and LadderNode linked list. The WordNode linked list holds the ladder
from the start to find word. The LadderNode linked list holds the WordNode ladders.

Course: CS 211, Fall 2024, UIC

Author: Shreya Ganguly
------------------------------------------- */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

int countWordsOfLength(char* filename, int wordSize) { 
    // open file
    FILE* inFile = fopen(filename, "r");
    // if file cannot be opened, return -1
    if(inFile == NULL){
        return -1;
    }
    int count = 0;
    char word[100];
    // if file opened, while you can extract another word
    while(fscanf(inFile, "%s", word) == 1){
        // if the word's length is what you want it to be,
        if(strlen(word) == wordSize){
            // increment count
            count++;
        }
    }
    // close the file
    fclose(inFile);
    // return the number of words with the correct length
    return count;
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    // open the file
    FILE* inFile = fopen(filename, "r");
    // if the file doesn't open, return false
    if(inFile == NULL){
        return false;
    }
    // if the file doesn't have the same number of words with the size we want,
    if(countWordsOfLength(filename, wordSize) != numWords){
        // close the file
        fclose(inFile);
        // return false
        return false;
    }
    char word[50];
    int i = 0;
    // while you can get another word
    while(fscanf(inFile, "%s", word) == 1){
        // if the word has the correct length
        if(strlen(word) == wordSize){
            // add the word to your array
            strcpy(words[i], word);
            // increment the array index
            i++;
        }
    }
    // close the file
    fclose(inFile);
    // return true if the scan was successful
    return true;
}

int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    while(loInd <= hiInd){
        // midInd is the index between loInd and hiInd
        int midInd = (hiInd + loInd) / 2;
        // if the word at that index is the word, return the word
        if(strcmp(words[midInd], aWord) == 0){
            return midInd;
        }
        // if words[midInd] is before aWord, set low to the element after mid
        else if(strcmp(words[midInd], aWord) < 0){
            loInd = midInd + 1;
        }
        // if words[midInd] is after aWord, set high to the element before mid
        else{
            hiInd = midInd - 1;
        }
    }
    // if the word isn't found, return -1
    return -1;
}

void freeWords(char** words, int numWords) {
    // free each word in the words array
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    // free the words array
    free(words);
}

void insertWordAtFront(WordNode** ladder, char* newWord) {
    // if the parameter is null, return null
    if(newWord == NULL){
        return;
    }
    // allocate space for a new [WordNode]
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    // set its [myWord] subitem using <newWord>
    newNode->myWord = newWord;
    // connect the original array
    newNode->next = *ladder;
    // set the original head to our new head
    *ladder = newNode;
}

int getLadderHeight(WordNode* ladder) {
    // set the count to 0
    int count = 0;
    // while your ladder has another node increment count and move to the next node
    while(ladder != NULL){
        count++;
        ladder = ladder->next;
    }
    // return the count
    return count;
}

WordNode* copyLadder(WordNode* ladder) {
    // Step 1: check for null list
    // if list is null, return null
    if(ladder == NULL){
        return NULL;
    }

    // Step 2: copy the head
    // allocate space for a new node head
    WordNode* newHead = (WordNode*)malloc(sizeof(WordNode));
    // copy the head node
    newHead->myWord = ladder->myWord;
    // set next to null
    newHead->next = NULL;

    // Step 3: copy the rest of the list
    // make a new linked list point to the head
    WordNode* newLadder = newHead;
    // set next to null
    newLadder->next = NULL;
    // move the orignal list to the next node
    WordNode* parse = ladder->next;
    // while your original list's node is not empty, copy
    while(parse != NULL){
        // create a new node and allocate space for it
        WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
        // copy into the new node
        newNode->myWord = parse->myWord;
        // set next to null
        newNode->next = NULL;
        // append newNode to newLadder
        newLadder->next = newNode;
        // move newLadder to next node
        newLadder = newNode;
        // move to the next node (original list)
        parse = parse->next;
    }

    // return your head of the copied ladder
    return newHead;
}

void freeLadder(WordNode* ladder) {
    // if the parameter is null, return null
    if(ladder == NULL){
        return;
    }
    // create a new WordNode* to parse through the ladder
    WordNode* current = ladder;
    // create another WordNode* to temporarily hold the data for the rest of the linked list so we don't lose access
    WordNode* pointerToNext;
    // while your node is not null
    while(current != NULL){
        // store the rest of the linkied list
        pointerToNext = current->next;
        // free the current node
        free(current);
        // reassign your current node variable
        current = pointerToNext;
    }
}

// Test cases that need freeLadder:
bool test_countWordsOfLength(){
    // delcare function parameters
    char filename[20] = "simple3.txt";
    int wordSize = 3;
    // call function
    int numberWords = countWordsOfLength(filename, wordSize);
    // return true if function correctly returns 26 words
    return (numberWords == 26);
}

bool test_buildWordArray(){
    // delcare function parameters
    char filename[20] = "simple3.txt";
    int wordSize = 3;
    int numWords = 26;
    // allocate space for words array
    char** words = (char**)malloc(numWords * sizeof(char*));
    for (int i = 0; i < numWords; i++) {
        words[i] = (char*)malloc((wordSize + 1) * sizeof(char));
    }
    // call function
    bool result = buildWordArray(filename, words, numWords, wordSize);
    // free each word in the words array
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    // free the words array
    free(words);
    // return true if function correctly returns true
    return (result == true);
}

bool test_findWord(){
    // delcare function parameters
    char filename[20] = "simple3.txt";
    int wordSize = 3;
    int numWords = 26;
    // allocate space for words array
    char** words = (char**)malloc(numWords * sizeof(char*));
    for (int i = 0; i < numWords; i++) {
        words[i] = (char*)malloc((wordSize + 1) * sizeof(char));
    }
    // call function to initalize words array
    buildWordArray(filename, words, numWords, wordSize);
    // call fucntion to test for correct index
    int index = findWord(words, "jam", 0, numWords - 1);
    // free each word in the words array
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    // free the words array
    free(words);
    return (index == 10);
}

bool test_insertWordAtFront(){
    // create an empty WordNode ladder
    WordNode* ladder = NULL;
    // insert a word to the front
    insertWordAtFront(&ladder, "new");
    // str compare the top word of the node with the word it is supposed to be
    // if it's the right word, result will be assigned with true, else false
    bool result = (strcmp(ladder->myWord, "new") == 0);
    // free the WordNode
    freeLadder(ladder);
    // return the result
    return result;
}

bool test_getLadderHeight(){
    // make a WordNode ladder with 3 words
    WordNode* ladder = NULL;
    insertWordAtFront(&ladder, "word3");
    insertWordAtFront(&ladder, "word2");
    insertWordAtFront(&ladder, "word1");
    // make a bool variable true if the function correctly returns a hight of 3, else make it false
    bool result = (getLadderHeight(ladder) == 3);
    // free the WordNode
    freeLadder(ladder);
    // return the boolean result
    return result;
}

bool test_copyLadder(){
    // create a WordNode ladder and insert words
    WordNode* ladder = NULL;
    insertWordAtFront(&ladder, "word3");
    insertWordAtFront(&ladder, "word2");
    insertWordAtFront(&ladder, "word1");
    // copy that ladder into another ladder
    WordNode* copiedLadder = copyLadder(ladder);
    // create pointers to parse throuh both linked lists
    WordNode* parse_ladder = ladder;
    WordNode* parse_copiedLadder = copiedLadder;
    // while the original ladder is not NULL
    while(parse_ladder != NULL){
        // if the words don't match, free both WordNodes and return false
        if(strcmp(ladder->myWord, copiedLadder->myWord) != 0){
            freeLadder(ladder);
            freeLadder(copiedLadder);
            return false;
        }
        // if they match, go to the next node
        parse_ladder = parse_ladder->next;
        parse_copiedLadder = parse_copiedLadder->next;
    }
    // free both WordNodes
    freeLadder(ladder);
    freeLadder(copiedLadder);
    // if it didn't return false, then all the words match so return true
    return true;
}

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    // create new LadderNode and allocate space for it
    LadderNode* newList = (LadderNode*)malloc(sizeof(LadderNode));
    // add the newLadder to the new LadderNode
    newList->topWord = newLadder;
    newList->next = NULL;
    // Edge Case: list is empty
    // derefernce list to edit the content inside it
    if(*list == NULL){
        // assign list to the single new element
        *list = newList;
    }
    // if list has stuff in it, we want to append to the end
    else{
        // create a new LadderNode (pointer to list) to traverse to the end of the list
        LadderNode* parseList = *list;
        // while the next element is not empty, go to it
        while(parseList->next != NULL){
            parseList = parseList->next;
        }
        // set the last element to the newList node
        parseList->next = newList;
    }
}

WordNode* popLadderFromFront(LadderNode** list) {
    // if the parameter is null, return null
    if(*list == NULL){
        return NULL;
    }
    // get start from list
    LadderNode* originalStart = *list;
    // get the first item in the list
    WordNode* firstItem = (*list)->topWord;
    // move forward in the original list (we can never go back)
    *list = (*list)->next;
    // free the space saved for the head node
    free(originalStart);
    // return the first WordNode
    return firstItem;
}

void freeLadderList(LadderNode* myList) {
    // create a new LadderNode* to parse through the ladder
    LadderNode* current = myList;
    // create another LadderNode* to temporarily hold the data for the rest of the linked list so we don't lose access
    LadderNode* pointerToNext;
    // while your node is not null
    while(current != NULL){
        // store the rest of the linkied list
        pointerToNext = current->next;
        // free the current WordNode by callingfreeLadder
        freeLadder(current->topWord);
        // free the node too
        free(current);
        // reassign your current LadderNode =
        current = pointerToNext;
    }
}

WordNode* findShortestWordLadder(char** words, bool* usedWord, int numWords, int wordSize, char* startWord, char* finalWord) {
    // create a LadderNode and WordNode variable
    LadderNode* myList = NULL;
    WordNode* myLadder = NULL;
    // insert the start word into your WordNode linked list
    insertWordAtFront(&myLadder, startWord);
    // insert your WordNode linked list into your LadderNode linked list
    insertLadderAtBack(&myList, myLadder);

    // create a string to hold the different variations of your top word
    char currentWordTest[wordSize + 1];

    // while your ladder is not empty
    while(myList != NULL){
        // make a new linked list that points to the top of your ladder
        WordNode* currentLadder = popLadderFromFront(&myList);

        // for the length of the word
        for(int i = 0; i < wordSize; i++){
            // copy the top word into your string
            strcpy(currentWordTest, currentLadder->myWord);
            
            // for each possible letter
            for(char letter = 'a'; letter <= 'z'; letter++){
                // switch the letter at index i
                currentWordTest[i] = letter;
                // get the index of the word from dictionary
                int wordLocation = findWord(words, currentWordTest, 0, numWords - 1);

                // if the index exists (aka it's not -1) and the word has not been used already
                if(wordLocation != -1 && !usedWord[wordLocation]){
                    // if its the final word
                    if(strcmp(currentWordTest, finalWord) == 0){
                        // insert the word to the front of your WordNode linked list
                        insertWordAtFront(&currentLadder, words[wordLocation]);
                        // free your original LadderNode
                        freeLadderList(myList);
                        // set that you used the word
                        usedWord[wordLocation] = true;
                        // return your WordNode linked list
                        return currentLadder;

                    }
                    // if it's not the final word
                    else{
                        // copy your original ladder
                        WordNode* anotherLadder = copyLadder(currentLadder);
                        // insert the word to the front of your copied ladder
                        insertWordAtFront(&anotherLadder, words[wordLocation]);
                        // put this new ladder at the back of your LadderNode
                        insertLadderAtBack(&myList, anotherLadder);
                        // set that you used the word
                        usedWord[wordLocation] = true;
                    }
                }
            }
        }
        // at the end of every while iteration, free your current ladder
        freeLadder(currentLadder);
    }
    // if no ladder exists, free your LadderNode
    freeLadderList(myList);
    // and return NULL
    return NULL;
}

// Test cases that need freeLaddderList:
bool test_insertLadderAtBack(){
    // create a WordNode ladder
    WordNode* ladder1 = NULL;
    insertWordAtFront(&ladder1, "ladder1_word3");
    insertWordAtFront(&ladder1, "ladder1_word2");
    insertWordAtFront(&ladder1, "ladder1_word1");
    // create another WordNode ladder
    WordNode* ladder2 = NULL;
    insertWordAtFront(&ladder2, "ladder2_word3");
    insertWordAtFront(&ladder2, "ladder2_word2");
    insertWordAtFront(&ladder2, "ladder2_word1");
    // create a LadderNode list
    LadderNode* list = NULL;
    // add both WordNodes
    insertLadderAtBack(&list, ladder1);
    insertLadderAtBack(&list, ladder2);
    // test if the first lists's top word is ladder1_word1
    WordNode* firstWordNode = list->topWord;
    if(strcmp(firstWordNode->myWord, "ladder1_word1") != 0){
        // if it's not, return false
        return false;
    }
    // additional test, check to see if the second node was added to the back
    WordNode* secondWordNode = list->next->topWord;
    if(strcmp(secondWordNode->myWord, "ladder2_word1") != 0){
        // if not, return false
        return false;
    }
    // free the list
    freeLadderList(list);
    // return true if all tests pass
    return true;
}

bool test_popLadderFromFront(){
    // create a WordNode ladder
    WordNode* ladder1 = NULL;
    insertWordAtFront(&ladder1, "ladder1_word3");
    insertWordAtFront(&ladder1, "ladder1_word2");
    insertWordAtFront(&ladder1, "ladder1_word1");
    // create another WordNode ladder
    WordNode* ladder2 = NULL;
    insertWordAtFront(&ladder2, "ladder2_word3");
    insertWordAtFront(&ladder2, "ladder2_word2");
    insertWordAtFront(&ladder2, "ladder2_word1");
    // create a LadderNode list and insert the two WordNodes
    LadderNode* list = NULL;
    insertLadderAtBack(&list, ladder1);
    insertLadderAtBack(&list, ladder2);
    // pop the top WordNode
    WordNode* front = popLadderFromFront(&list);
    // the top word of the popped WordNode should be ladder1_word1
    bool result = (front->myWord == "ladder1_word1");
    // free the LadderList
    freeLadderList(list);
    // return if the correct WordNode was popped
    return result;
}

bool test_findShortestWordLadder(){
    // delcare buildWordArray function parameters
    char filename[20] = "simple5.txt";
    int wordSize = 5;
    int numWords = 10;
    // allocate space for words array
    char** words = (char**)malloc(numWords * sizeof(char*));
    for (int i = 0; i < numWords; i++) {
        words[i] = (char*)malloc((wordSize + 1) * sizeof(char));
    }
    // call function to initalize words array
    buildWordArray(filename, words, numWords, wordSize);
    // initalize usedWord array to false
    bool* usedWord = (bool*)malloc(numWords * sizeof(bool));
    for(int i = 0; i < numWords; i++){
        usedWord[i] = false;
    }
    // choose the start and final words
    char* startWord = "share";
    char* finalWord = "start";
    // call the function
    WordNode* shortestLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);
    bool firstWordCorrect = (strcmp(shortestLadder->myWord, "start") == 0);
    bool secondWordCorrect = (strcmp(shortestLadder->next->myWord, "stare") == 0);
    bool thirdWordCorrect = (strcmp(shortestLadder->next->next->myWord, "share") == 0);
    // free malloc space
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    free(words);
    free(usedWord);
    return (firstWordCorrect && secondWordCorrect && thirdWordCorrect);
}

void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

// calls all the test functions and prints explainatory statements
void master_test(){
    printf("\n");
    printf("Checking main.c functions for correctness...\n");
    printf("\n");

    printf("Testing countWordsOfLength():\n");
    if(test_countWordsOfLength()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Wrong count.\n");
    }
    printf("\n");

    printf("Testing buildWordArray():\n");
    if(test_buildWordArray()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Array not built.\n");
    }
    printf("\n");

    printf("Testing findWord():\n");
    if(test_findWord()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Wrong index.\n");
    }
    printf("\n");

    printf("Testing insertWordAtFront():\n");
    if(test_insertWordAtFront()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Did not insert at front.\n");
    }
    printf("\n");

    printf("Testing getLadderHeight():\n");
    if(test_getLadderHeight()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Wrong height.\n");
    }
    printf("\n");

    printf("Testing copyLadder():\n");
    if(test_copyLadder()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Wrong height.\n");
    }
    printf("\n");

    printf("Testing insertLadderAtBack():\n");
    if(test_insertLadderAtBack()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Did not insert correctly.\n");
    }
    printf("\n");

    printf("Testing popLadderFromFront():\n");
    if(test_popLadderFromFront()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Wrong node returned.\n");
    }
    printf("\n");

    printf("Testing findShortestWordLadder():\n");
    if(test_findShortestWordLadder()){
        printf("Successful!\n");
    }else{
        printf("Unsuccessful. Wrong ladder.\n");
    }
}

//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main(int argc, char* argv[]) {
    // search command line arguments for testing flag
    bool testingMode = false;
    for(int i = 1; i < argc; i++){
        // if there is a testing flag, turn on testing mode
        if(strcmp(argv[i], "-t") == 0){
            testingMode = true;
        }
    }

    // if testing mode is on, call the master test function
    if(testingMode){
        master_test();
    }

    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));

    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}