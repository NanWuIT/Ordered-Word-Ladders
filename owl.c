// Solvability of the owl puzzle
//
// owl.c
//
// Nan Wu
//
// Edit date: 2019.08.06
//
// This is the main program that give the answer of whether a
// Owl puzzle is solvable or not. Besides, we need to give the
// Answer.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Graph.h"
#include "Quack.h"

#define MAX 1024
// the length of the word is not over 20, and the last 1 is to save '\0'
#define WORD 21
#define WHITESPACE 100

// Before the comparing, the default first is the word with shorter length
bool differByOne(char *first, char *next) {
    int lengthDiff = strlen(next) - strlen(first);
    int diff = 0;
    // when the length difference of two words are larger than one, then just
    // return false
    if (lengthDiff > 2) {
        return false;
    }
    // when the two words have same length, then compare their each character.
    else if (lengthDiff == 0) {
        for( int i = 0; i < strlen(next); i++ ) {
            if ( *(first + i) != *(next + i) ) {
                diff++;
            }
            if (diff > 1) {
                return false;
            }
        }
    }
    // when the length difference of two words is one, then we comparing each
    // character, when there is difference, we took the different one in next
    // and comparing the present one in first word.
    else {
        for (int j = 0; j < strlen(next); j++) {
            if (diff < 1) {
                if (*(first + j) != *(next + j)) {
                    diff++;
                }
            }
            else {
                if (*(first + j - 1) != *(next + j)) {
                    return false;
                }
            }
        }
    }
    return true;
}

// callGraph will print the graph, and return the new graph with vertices
Graph callGraph(Graph graph, int numV, char list[MAX][WORD]) {
    // use two for loops to check all the elements in the matrix, if two words
    // are differByOne, then add the edge
    for (int x = 0; x < numV; x++){
        for (int y = 0; y < numV; y++) {
        // make sure words are not comparing with themselves
            if ( x != y ) {
                // making sure the next word is the word with longer length
                if (strlen(list[x]) >= strlen(list[y])) {
                    if (differByOne(list[y], list[x])) {
                        // create a new edge firstly
                        Edge e = newEdge(y, x);
                        // then insert this new edge into the graph
                        insertEdge(e, graph);
                    }
                }
                else {
                    // do the same thing, the difference is the length of word
                    if (differByOne(list[x], list[y])) {
                        Edge e = newEdge(x, y);
                        insertEdge(e, graph);
                    }
                }
            }
        }
    }
    showGraph(graph);
    return graph;
}


// order: the sequence of the path, longest: the longest path value, x: current
// line value, y: current value of column, numW: number of words, visit: the 
// visit state of each word, matrix: is a table saved with the path information
// list: all the words dictionary
void printPath(int *order, int longest, int x, int y, int numW, int visit[numW],
             int matrix[numW][numW], char list[MAX][WORD]) {
    if (longest == 1) {
        // print the informaiton as required format
        printf("%2d: ",*order);
        visit[x] = 1;
        visit[y] = 1;
        // change visited value as ture
        int count = numW;
        for (int i = 0; i < numW; i++) {
            if (visit[i] == 1) {
                // if the word is the last word in the path, then do not 
                // print ->
                if ( count == numW - 1 ){
                    printf("-> %s ", list[i]);    
                }
                else {
                    printf("%s", list[i]);
                    count--;    
                }
            }
        }
        // print \n after each path
        printf("\n");
        // change visited value as false
        visit[y] = 0;
        visit[x] = 0;
        // add up the order of path
        (*order)++;
    }
    else {
            for (int i = 0; i < numW; i++){
                if (matrix[y][i] == longest) {
                    visit[x] = 1;
                    visit[y] = 1;
                    // call function recursivly
                    printPath(order, longest - 1, y, i, numW, visit, matrix, list);
                    visit[y] = 0;
                    visit[x] = 0;
                }
            }
    }
    return;
}


// use findLongest function to get the longest path length in the
// graph
void findLongest(int numW, int longest, int matrix[numW][numW], Graph graph,
                 char list[MAX][WORD]){
    // in the descendant order to find the longest path, from the last
    // element to the first element, every time there is an edge, update
    // the longest path information
    // the lineMax array's value is used to record the max value
    // in each line
    int lineMax[numW];
    for (int t = 0; t < numW; t++) {
        lineMax[t] = 0;
    }
    for (int x = numW - 1; x >= 0; x--) {
        for (int y = 0; y < numW; y++) {
            // if x is larger than y, then it means it would not be a 
            // path that could be used, but still, we need to update it
            Edge e = newEdge(x, y);
            if (x > y) {
                if (isEdge(e, graph)) {
                    matrix[x][y] = 1;
                    // update the max value in this line
                    if (matrix[x][y] > lineMax[x]) {
                        lineMax[x] = matrix[x][y];
                    }
                }
            }
            // making sure x is not equal to y
            else if (x < y){
                if (isEdge(e, graph)) {
                    if (matrix[x][y] < lineMax[y]) {
                        // add up the path length value
                        matrix[x][y] = lineMax[y] + 1;
                        // record the max value in each line
                        if (matrix[x][y] > lineMax[x]) {
                            lineMax[x] = matrix[x][y];
                        }
                        // also, update the longest value
                        if (matrix[x][y] > longest) {
                            longest = matrix[x][y];
                        }
                    }
                }
            }
        }
    }
    printf("Longest ladder length: %d\n", longest);
    printf("Longest ladders: \n");
    // if there is no edge, then print all the words once
    if (longest < 1) {
        printf("%s\n", list[0]);
        for (int z = 0; z < numW; z++) {
            printf("%d: %s\n", z, list[z]);
        } 
    }
    else {
        // loop in the matrix, when we find a value that equal to
        // the max length, we print the path
        // first of all, set the order value of each line
        int order = 1;
        for (int x = 0; x < numW; x++) {
           for (int y = x + 1; y < numW; y++) {
                if (matrix[x][y] == longest) {
                    // initialise an array call visit to record the visited
                    // condition, in the beginning, each element is 0, which
                    // is not visited
                    int visit[numW];
                    for (int v = 0; v < numW; v++) {
                        visit[v] = 0;
                    } 
                    // when we find the start point with the max length
                    // call printPath function to print the path
                    printPath(&order, longest - 1, x, y, numW, visit, matrix, list);
                }
            }
        }
    }
    return;
}


int main() {
    // we are reading the file from stdin, thus, no need to use argv
    // initialising the value, word for each word, list to save all words
    char c = 0;
    char list[MAX][WORD];
    char word[WORD];
    int i = 0;
    int j = 0;
    int numWord = 0;
    // if the input is an EOF, that is the end of the file, we stop
    while ( c != EOF ) {
        // update c value, make it equal to the next input character
        c = getchar();
        int var = c;
        // when we read a character that is not a word,
        // then save the word into the word list
        if ( !isalpha(var) ) {
            // if word is exist, then save it into list
            if (strlen(word)>0) {
                // make every word end with '\0'
                word[j] = '\0';
                strcpy(list[i], word);
                // comparing each two words in the order of input, when there is
                // duplication, ignore this word
                if ( list[1] != NULL ) {
                    // when the two words are not equal
                    if (strcmp(list[i-1], list[i]) != 0) {
                        // move to the next value
                        i++;
                        // record the length of the list, which is the number
                        // of words
                        numWord++;
                    }
                    // in other conditon, which is the two words are the same, 
                    // we can just ignore it and read the next word
                }
                // when we facing with the first word, there is no need to compare
                // but still, we need to 'clean' the word, move to the next value,
                // and record the number of words
                else {
                    i++;
                    numWord++;
                }
            }
            // also, 'clean' the word array to save future word
            strcpy(word, "");
            j = 0;
        }
        else {
            word[j] = c;
            j++;
        }
    }
    // step by step, print the required information
    printf("Dictionary\n");
    // print every word in the dictionary
    for (int k = 0; k < numWord; k++) {
        printf("%d: %s\n", k, list[k]);
    }
    printf("Ordered Word Ladder Graph\n");
    // create a new graph that can contain all the word information firstly
    Graph graphNew = newGraph(numWord);
    // then print the graph
    Graph graphV = callGraph(graphNew, numWord, list);
    // create a new matrix to find the longest path, this matrix will save
    // all the path information, and initialize the matrix that all the value
    // would be 0
    int matrixP[numWord][numWord];
    for (int x = 0; x<numWord; x++) {
        for (int y = 0; y<numWord; y++) {
            matrixP[x][y] = 0;
        }
    }
    // set a value call longest to update the longest path length information
    int longest = 0;
    // make longest as the global variant, use function findLongest
    // get the the longest path value in the graph, also, update the matrixP
    // to save the path information
    findLongest(numWord, longest, matrixP, graphV, list);
    
    // free the graph
    freeGraph(graphV);
    return 0;
}
