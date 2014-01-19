Boggle Solver
=============
## A Simple Boggle Solver in C
This program takes a list of characters representing a Boggle board as input and produces a list of words that can be made on that board. It uses a custom hashset to manage the dictionary and it is free from memory leaks as reported by Valgrind. Note you'll need to provide your own "words.txt" (as it is called in the program) in order for everything to work. I do not have such a list available to push up onto this repository publicly. The hashset uses a string hashing function called "djb2" found [here](http://www.cse.yorku.ca/~oz/hash.html). Eventually I hope to add a gtk front end to this project.

A good board to test is "serspatglinesers"
