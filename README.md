# Ordered-Word-Ladders
An ordered word ladder ('owl') is an alphabetically-ordered sequence of words where each word in the sequence 
differs from its predecessor by one action: 

1. changing one letter, e.g. barn→born
2. adding or removing one letter, e.g. band→brand and bran→ran

The following are examples of word ladders of different length:
- ban→bar→boar→boat→goat, length 5
- an→can→cane→dane→date→mate→mite→site→size, length 9

## Input
The words in the dictionary would be read from stdin. There will be whitespace
between the words (i.e. spaces, tabs, newlines) in lower-case letters also in dictionary order. 

For example: 
```
an ban bean ben hen mean men
```

## Output
Including the sorted output of words in the dictionary, and the graph that satisfies the requirement. 
```
Dictionary
0: an
1: at
2: in
3: it
4: on
Ordered Word Ladder Graph
V=5, E=6
<0 1> <0 2> <0 4>
<1 0> <1 3>
<2 0> <2 3> <2 4>
<3 1> <3 2>
<4 0> <4 2>
Longest ladder length: 3
Longest ladders:
1: an -> at -> it
2: an -> in -> it
3: an -> in -> on
```
This is actually a graph like
```
         0
      /  |  \
     1   2 — 4
      \ /
       3
```
