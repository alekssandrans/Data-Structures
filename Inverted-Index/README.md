# Inverted Index [Homework]

## Definition

An index data structure that maps content to 1 or more documents. The purpose is to optimize time needed for search queries at the
cost of more time needed to process the documents.

For every word in all of the documents we will save a list of all the documents that contain it so the problem "Which documents contain 
the word X?" is reduced to searching among the much smaller and indexed set of used words rather than the entire text.

## Problem

Implement a rudimentary inverted index, that can answer simple queries.

## Subproblems

### 1 - Parsing a Folder of Documents

Given a string that represents a path to a folder, add every name of a file in that folder to an array/vector.

### 2 - Parse the Documents

- Parse the documents in the vector from subproblem 1 and read them word by word
- Change capital letters to lower-case ones
- You can assume that the documents won't contain punctuation marks unless they are part of the word (e.g. star-crossed)
- Create a struct that contains a word and a linked list with all of the indices of documents which contain that word.
- Document indices are taken from the vector from subproblem 1.
- Keep all of these structs in a sorted vector (sorted by word)
- Use binary search to find the correct position of new words
- Make sure you don't include the same index multiple times in one list.

**Example:**

```
doc1.txt: aa bb aa cc
doc2.txt: aa cc dd
doc3.txt: aa

Inverted Index:
{
	{aa, {1, 2, 3,}}
	{bb, {1,}}
	{cc, {1,2,}}
	{dd, {2,}}
}
```

### 3 - Simplest Queries

Given a string, output the names of every file that contains this string. If the word is not found in any file
- output "No results found". Use binary search to find the given word.

**Examples:**

```
Using the documents from the previous example

aa -> doc1.txt, doc2.txt, doc3.txt
cc -> doc1.txt, doc2.txt
ee -> No results found
```

### 4 - Advanced Queries

Support queries with simple operations and brackets.

- Assume that every query is valid
- Assume that every token in a query is separated by a single space
- Implement an optimal algorithm to find the intersection, union and difference of the lists
- Implement an optimal algorithm to parse the queries

**Grammar**

```
<query> := <word> | <query> <operation> <query> | ( <query> )
<operation> := + | * | -
```

**Precedence**

queries in brackets > * > + and -

**Values of Queries**

```
<word> is the same as the simple query from the previous subproblem
<query1> + <query2> outputs a list of all files that match either query1 or query2
<query1> * <query2> outputs a list of all files that match both query1 and query2
<query1> - <query2> outputs a list of all files that match query1 but don't match query2
```
**Examples:**

```
Continuing the example started in subproblem 2

aa -> doc1.txt, doc2.txt, doc3.txt
bb + dd -> doc1.txt, doc2.txt
aa - cc -> doc3.txt
aa * bb -> doc1.txt
bb * dd -> No Results Found
aa - cc * bb -> doc2.txt, doc3.txt
( aa - cc ) * bb -> No Results Found
```


