#include "AutoCompleteImpl.h"
#include <time.h>

#define NUMBER_OF_WORDS (354935)
#define INPUT_WORD_SIZE (100)

char *receiveInput(char *s) {
    scanf("%99s", s);
    return s;
}

int main() {
    int word_count = 0;
    char* words[NUMBER_OF_WORDS];

    FILE *fp = fopen("resources/dictionary.txt", "r");
    if (fp == 0){
        fprintf(stderr, "Error while opening dictionary file");
        exit(1);
    }
    words[word_count] = malloc(INPUT_WORD_SIZE);
    while (fgets(words[word_count], INPUT_WORD_SIZE, fp)) {
        word_count++;
        words[word_count] = malloc(INPUT_WORD_SIZE);
    }

    //TODO populate tree with word list
    TrieNode *root = createTrieNode();
    // printf("%p\n",(void*)root );
    root -> label = "*";	//Marking the root node
    int i;

    //To get time to insert
    clock_t start = clock();

    for (i = 0; i < NUMBER_OF_WORDS; i++){
        char *word = words[i];
        int length = strcspn(word, "\r\n"); // get the length of the string eliminating CR and LF
        word[length] = 0; // trim the word length by inseting null character
        insert(root, word);
        // printf("%s\n", word);
    }

    clock_t end = clock();

    double insertionTime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time to insert: %lf\n", insertionTime);

    while (1) {
        printf("Enter keyword: ");
        char str[100];
        receiveInput(str);
        printf("\n==========================================================\n");
        printf("\n********************* Possible Words ********************\n");

        //TODO traverse the tree and provide possible word list
        
        char prefix [100]; // to collect the prefixes while searching
        prefix[0] = '\0';

        //Time calculation for traverse
        start = clock();

        TrieNode *subRoot = search(root, str, prefix); // get the nearest root with matching prefix

        if(subRoot == NULL){
            printf("\nNo matching word found!!!.......\n");
        }

        if(subRoot != NULL){
            strcat(prefix, subRoot->label); // add the nearest root's label to the prefix
        }

        traverse(subRoot, prefix); //go throgh all possible words in the subtree and print

        printf("==========================================================\n");
        
        end = clock();

        double traversalTime = (double)(end - start) / CLOCKS_PER_SEC;

        printf("Time to traverse: %lf\n", traversalTime);
    }
}