#include "AutoCompleteImpl.h"

TrieNode *createTrieNode() {
    //TODO implement logic for creating an Trie node

    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

    //Initializing the node components
    node -> children = (TrieNode **)malloc(sizeof(TrieNode *));
    node -> childrenCount = 0;
    node -> label = NULL;
    node -> isEndOfWord = false;

    return node;
}

// Function to compare two strings and get the last index of the matching prefix
int getBreakPoint(char *label, char *word){

    int lenLabel = strlen(label);
    int lenWord = strlen(word);
    int length;

    if (lenWord > lenLabel){
        length = lenLabel;
    } else {
        length = lenWord;
    }

    int breakPoint = 0, i;
    for (i = 0; i < length; i++){
        if (label[i] == word[i]){
            breakPoint++;
        } else {
            break;
        }
    }

    return breakPoint;
}

// Function to split the given string from given index upto a given length
char * splitString(char * word, int start, int length){

    char *subString = (char *)malloc(length + 1);
    memcpy(subString, &word[start], length);
    subString[length] = '\0';

    return subString;
}

// Function to split the given string from given index upto a given length
void addAsChild(TrieNode *parent, TrieNode *child){

    int n = parent -> childrenCount;
    parent -> children = (TrieNode **)realloc(parent -> children, (n + 1)* sizeof(TrieNode *));
    parent -> children[n] = child;
    parent -> childrenCount = n + 1;
}

void insert(TrieNode *root, char *word) {
    //TODO implement logic for inserting a word to the tree

    if (root == NULL){
        return;
    }

    TrieNode * currentRoot = root;

    while(true){

        bool match = false; //  variable to keep track matching prefix found or not
        int i;

        for (i = 0; i < currentRoot -> childrenCount; i++){

            // Access each child
            TrieNode * child = currentRoot -> children[i];
            
            // Get the last index of matching prefix comparing with the word to be inserted and the label in the child node
            int breakPoint = getBreakPoint(child -> label, word);

            if (breakPoint > 0){    //Matching prefix found

                if (breakPoint == strlen(child -> label)){  //Entire label of child node matches
                    
                    if (breakPoint == strlen(word)){    // Entire word matches with the child label
                        
                        child -> isEndOfWord = true;    //characters for the word already exists, hence mark as an end of a word
                        return; // insert finished
                    }

                    // part of the word remains without matching. 
                    // Hence, check the next set of children of this child node to find matching prefix
                    char * nextWord = splitString(word, breakPoint, strlen(word) - breakPoint);    //get the remaining part of the word to be matched
                    word = nextWord;    //Make the current word equal to the next word to be found
                    currentRoot = child;   
                    match = true; // match found
                    break;

                } else {    // Part of the label in child node matches

                    // get the remaining part of the child's label
                    char *labelRest = splitString(child -> label, breakPoint, strlen(child -> label) - breakPoint);
                    
                    // create new child node and insert the remaining part of the child's label 
                    TrieNode *nextChild = createTrieNode();
                    nextChild -> label = labelRest;
                    //Copy rest of data from child's node to new next child node
                    nextChild -> children = malloc(sizeof(TrieNode*) * child -> childrenCount);
                    memcpy(nextChild -> children, child -> children, sizeof(TrieNode*) * child -> childrenCount);
                    nextChild -> childrenCount = child -> childrenCount;
                    nextChild -> isEndOfWord = child -> isEndOfWord;

                    // Arrange the child node as a new node

                    // Replace the current child's label with the matched prefix
                    child -> label = splitString(child -> label, 0, breakPoint); 

                    child -> children = (TrieNode**) malloc(sizeof(TrieNode*)); // allocate memory for new children
                    child -> childrenCount = 0;     // No children at this time
                    child -> isEndOfWord = false;   // not an end of word
   
                    // Make the current child as the parent of the created new nextChild 
                    addAsChild(child, nextChild);

                    if (breakPoint == strlen(word)){    // Entire word matches with the part of the label

                        child -> isEndOfWord = true;    // mark as end of a word
                        return; // finish insert
                    }

                    // When there is a part of the word to be inserted remaining

                    // get the reamaing part of the word
                    char *wordRest = splitString(word, breakPoint, strlen(word) - breakPoint);

                    // Create new child node and enter the rest of the remaining word
                    TrieNode *newChild = createTrieNode();
                    newChild -> label = wordRest;
                    newChild -> isEndOfWord = true;  // Mark as end of word

                    // Make the current child as the parent of the created new child
                    addAsChild(child, newChild);
                    return;

                }
            }
        }

        if (!match){    //No matching prefix found

            // Word is new. Hence insert as a label in a new node
            TrieNode * newNode = createTrieNode();
            newNode -> label = word;
            newNode -> isEndOfWord = true; // Mark as end of word

            // Add the new node as a child to the current child node
            addAsChild(currentRoot, newNode);
            return; // Finish insert   
        }
    }     
}

TrieNode *search(TrieNode *root, char *word, char *prefix) {
    //TODO implement search logic for Tries tree.
    //TODO This function should return last node of the node sequence where we found given word
    if (root == NULL){
        return NULL;
    }

    TrieNode * currentRoot = root;
    
    while (strlen(word) > 0){   //search till the end of word

        bool match = false;
        int i;

        for (i = 0; i < currentRoot -> childrenCount; i++){ //Search through all children

            TrieNode *child = currentRoot -> children[i];
            int breakPoint = getBreakPoint(child -> label, word);   

            if (breakPoint > 0){
                
                if (breakPoint == strlen(word)){    // Entire word matches
                    match = true;
                    // printf("return child\n");
                    return child;   // return current node

                } else {

                    char * matchedPrefix = splitString(word, 0, breakPoint); // get the matched prefix from word
                    strcat(prefix, matchedPrefix);    // add the matching prefix from the word to the current prefix
                    word = splitString(word, breakPoint, strlen(word) - breakPoint);    // make the new word equivalent to the remaining part of the word
                    currentRoot = child;
                    match = true;
                    break;  // continue search with new word 
                }
            }
        }
            
        if (!match){
            return NULL;
        }
    }

    return currentRoot;
}

void traverse(TrieNode *root, char prefix[]) {
    //TODO implement tree traversal logic here. Use this to traverse underneath tree
    //TODO TIP: use this function to print words once you find the node in search function
    if (root == NULL){
        return;
    }

    TrieNode * currentRoot = root;
    int i;

    if(currentRoot -> isEndOfWord){
        // for (i = 0; i < size; i++){
        printf("%s\n", prefix);
        // }
        // printf("\n");
    }

    int endPoint = strlen(prefix);

    for (i = 0; i < currentRoot -> childrenCount; i++){
        
        TrieNode *child = currentRoot -> children[i];
        
        if (child != NULL){

            prefix[endPoint] = '\0';
            strcat(prefix, child -> label); // insert the current nodes label to the prefix
            // printf("%s\n", prefix);
            traverse(child, prefix); // traverse the rest of the tree

        }
    }
    
}


