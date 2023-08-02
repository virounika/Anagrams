#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <chrono> // Include the chrono library for time measurement
using namespace std;

const int MAXRESULTS = 20; // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

// Helper function to read words from the dict file and store them in the dict array
// Returns the number of words read into dict
int vocabularyCreatorHelper(istream& dictfile, string dict[], int count) {
    string word;
    if (count >= MAXDICTWORDS || !getline(dictfile, word)) {
        // Base case: reached the maximum limit / unable to read more words
        return count;
    }
    dict[count] = word;
   //  cout << "Read word from dictionary: " << word << endl; // Debug output

    // Recursively read in next word
    return vocabularyCreatorHelper(dictfile, dict, count + 1);
}

// Recursive function to clear dict array if not already empty
void clearDictArray(string dict[], int size) {
    if (size <= 0) {
        return;
    }
    dict[size - 1] = ""; // Set the last element to an empty string
    clearDictArray(dict, size - 1); // Recursively clear the previous element
}

// Function to call the helper function with initial count as 0
int vocabularyCreator(istream& dictfile, string dict[]) {
    // Clear before adding to array
    clearDictArray(dict, MAXDICTWORDS);
    return vocabularyCreatorHelper(dictfile, dict, 0);
}

// Helper function to check if the word is found in the dictionary
bool isWordInDictionary(const string& word, const string dict[], int size, int i = 0) {
    if (i >= size) {
        return false;
    }

    if (word == dict[i]) {
        return true;
    }

    return isWordInDictionary(word, dict, size, i + 1);
}


// Function to check if a permutation is a duplicate in the results array
bool isDuplicate(const string& newPrefix, const string results[], int resultCount, int i = 0) {
    if (i >= resultCount) {
        return false; // Base case: No duplicates found in the results array
    }

    if (results[i] == newPrefix) {
        return true; // Found a duplicate
    }
    return isDuplicate(newPrefix, results, resultCount, i + 1); // Check the next element
}

// Recursive loop function
void Loop(int i, int max, string prefix, string rest, const string dict[], int size, string results[], int& resultCount, int wordLength) {
    if (i >= max) {
        return;
    }

    // Add the character at i to end of prefix
    string newPrefix = prefix + rest[i];

    // Create the updated rest string but excluding the current character
    string newRest = rest.substr(0, i) + rest.substr(i + 1);

    if (newPrefix.length() == wordLength) {
        // Base case: Display the prefix string if it has the same length as the original word
        if (isWordInDictionary(newPrefix, dict, size) && !isDuplicate(newPrefix, results, resultCount) && resultCount < MAXRESULTS) {
            results[resultCount] = newPrefix;
            resultCount++;
        }
        Loop(i + 1, max, prefix, rest, dict, size, results, resultCount, wordLength);
        return;
    }

    // Use recursion to generate permutations with the updated values for prefix and rest
    Loop(0, newRest.size(), newPrefix, newRest, dict, size, results, resultCount, wordLength);
    Loop(i + 1, max, prefix, rest, dict, size, results, resultCount, wordLength);
}

// Function to generate permutations and store valid ones in results array
void printPermutationsHelper(string prefix, string rest, const string dict[], int size, string results[], int& resultCount, int wordLength) {
    Loop(0, rest.size(), prefix, rest, dict, size, results, resultCount, wordLength);
}

// Recursive function to clear results array
void clearResultsArray(string results[], int size) {
    if (size <= 0) {
        return;
    }
    results[size - 1] = ""; // Set the last element to an empty string
    clearResultsArray(results, size - 1); // Recursively clear the previous element
}

// Function to find potential sequences of a word in the dictionary
int potentialSequences(string word, const string dict[], int size, string results[]) {
    clearResultsArray(results, MAXRESULTS);
    int resultCount = 0;
    printPermutationsHelper("", word, dict, size, results, resultCount, word.length());
    return resultCount;
}

    // get a word
    // generate permutation and check if that permutation is in the dict
    // if the permutation is in the dict, add it to results and increment size
    // continue until all permutations have been generated

// Helper function to display the strings from results recursively
void discloseOutcomeHelper(const string results[], int size, int currentIndex) {
    if (currentIndex >= size) {
        return;
    }

    cout << "Matching word " << results[currentIndex] << endl;
    discloseOutcomeHelper(results, size, currentIndex + 1);
}

void outcomeDisclosure(const string results[], int size) {
    discloseOutcomeHelper(results, size, 0);
}

int main()
{
    string results[MAXRESULTS];
    string dict[MAXDICTWORDS];
    ifstream dictfile; // file containing the list of words
    int nwords; // number of words read from dictionary
    string word;
    dictfile.open("/Users/virounikamina/Desktop/CS32/p3/p3/words.txt");
    if (!dictfile) {
        cout << "File not found!" << endl;
        return (1);
    }
    
    nwords = vocabularyCreator(dictfile, dict);
    // cout << nwords;
    
    cout << "Please enter a string for an anagram: ";
    cin >> word;
    auto start = std::chrono::high_resolution_clock::now();
    
    int numMatches = potentialSequences(word, dict, nwords, results);
    // Stop the timer
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    if (!numMatches) {
        cout << "No matches found" << endl;
    }
    else {
        outcomeDisclosure(results, numMatches);
    }
    cout << "Execution time: " << duration.count() / 1000.0 << " seconds" << endl;
    
    return 0;
}
  
 /*
    // Test case for clearing 'results' array
       string results[5] = { "apple", "banana", "orange", "grape", "kiwi" };

       cout << "Before clearing 'results' array: ";
       for (int i = 0; i < 5; i++) {
           cout << results[i] << " ";
       }
       cout << endl;

       clearResultsArray(results, 5);

       cout << "After clearing 'results' array: ";
       for (int i = 0; i < 5; i++) {
           cout << results[i] << " ";
       }
       cout << endl;

       // Test case for clearing 'dict' array
       string dict[5] = { "dog", "cat", "bird", "fish", "rabbit" };

       cout << "Before clearing 'dict' array: ";
       for (int i = 0; i < 5; i++) {
           cout << dict[i] << " ";
       }
       cout << endl;

       clearDictArray(dict, 5);

       cout << "After clearing 'dict' array: ";
       for (int i = 0; i < 5; i++) {
           cout << dict[i] << " ";
       }
       cout << endl;

       return 0;
   }

int main()
{
    const string customDict[MAXDICTWORDS] = {
        
        "cat",
        "bird",
        "fish",
        "rabbit",
        "apple",
        "banana",
        "orange",
        "grape",
        "kiwi"
    };
    string results[MAXRESULTS] = {
        "dog",
        "cat",
        "bird",
        "fish",
        "rabbit",
        "apple",
        "banana",
        "orange",
        "grape",
        "kiwi"
    };
       string word;

       // Clear the 'results' array before using it
       clearResultsArray(results, MAXRESULTS);

       cout << "Please enter a string for an anagram: ";
       cin >> word;

       int numMatches = potentialSequences(word, customDict, MAXDICTWORDS, results);

       if (!numMatches) {
           cout << "No matches found" << endl;
       }
       else {
           cout << "Matching words:" << endl;
           for (int i = 0; i < numMatches; i++) {
               cout << results[i] << endl;
           }
       }

       // Test for checking if "apple" is an anagram of any word in the dictionary
       if (isWordInDictionary("apple", customDict, MAXDICTWORDS)) {
           cout << "The word 'apple' is an anagram of some word in the dictionary." << endl;
       }
       else {
           cout << "The word 'apple' is NOT an anagram of any word in the dictionary." << endl;
       }

       return 0;
}
 */
