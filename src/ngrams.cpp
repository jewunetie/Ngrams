#include <iostream>
#include "console.h"
#include "simpio.h"
#include <fstream>
#include "filelib.h"
#include "map.h"
#include "random.h"
#include "vector.h"
using namespace std;

//prototyping functions
void getUserInput(string& fileName, int& valueN);
int numbRand();
Vector<string> readWord(string textFile);
Map<Vector<string>, Vector <string>> Ngram(Vector<string> wordHolder, int valueN);
string randomizer(Map<Vector<string>, Vector<string>> mapText, int valueN, int numbRandomWords);

//main function
int main() {
    //introductions
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
            "This program makes random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you.\n" << endl;

    //intialize variables
    string fileName;
    int valueN;
    getUserInput(fileName, valueN);
    Vector<string> wordHolder = readWord(fileName);
    int numbRandomWords = numbRand();

    //repeats generating random words until the user quits
    while (numbRandomWords != 0){
        Map<Vector<string>, Vector<string>> mapText = Ngram(wordHolder, valueN);
        string text = randomizer(mapText, valueN, numbRandomWords);
        cout << text << "\n" << endl;
        numbRandomWords = numbRand();
    }
    cout << "Exiting!" << endl;
    return 0;
}

//gets the user input for the file and the value of the N-gram
void getUserInput(string& fileName, int& valueN){
    //gets the file for the text
    fileName = promptUserForFile("Input file name? ");

    //gets value of the N-gram
    valueN = getInteger("Value of N? ");
    while (valueN < 2){
        cout << "Invalid N-gram value. Try again. " << endl;
        valueN = getInteger("Value of N? ");
    }
    cout << "\n" << endl;
}

//asks user for the number of random words to generate or if the user wants to quit
int numbRand(){
    int numbRandomWords = getInteger("# of random words to generate (0 to quit)? ");
    while(numbRandomWords < 0){
        cout << "Invalid integer value. Try again. " << endl;
        numbRandomWords = getInteger("# of random words to generate (0 to quit)? ");
    }
    return numbRandomWords;
}

//stores all the words in the text into a vector of strings
Vector<string> readWord(string textFile, int counter){
    //opens the text file
    ifstream input;
    input.open(textFile);
    string word;
    Vector<string> wordHolder;

    //inserts each word into the vector
    while (input >> word){
        wordHolder.push_back(word);
    }
    return wordHolder;
}

//maps every vector of strings of keys to a vector of strings of values
Map<Vector<string>, Vector<string>> Ngram(Vector<string> wordHolder, int valueN){
    Vector<string> keyPlaceHolder;
    Map<Vector<string>, Vector<string>> mapText;
    int counter = 0;

    //maps the N-1 words to the Nth word and iterates through until it went through the whole text
    while (counter != wordHolder.size() - 1){
        int count = counter;
        for(int i = 0; i < (valueN - 1); i++){
            if (count > (wordHolder.size() - 1)){
                count = 0;
            }
            keyPlaceHolder.add(wordHolder[count]);
            count++;
        }
        if (count > wordHolder.size() - 1){
            count = 0;
        }
        string valuePlaceHolder = wordHolder[count];
        mapText[keyPlaceHolder].add(valuePlaceHolder);
        keyPlaceHolder.clear();
        valuePlaceHolder.clear();
        counter++;
    }
    return mapText;
}

//randomly generates the words based on what words are mapped to the previous words
string randomizer(Map<Vector<string>, Vector<string>> mapText, int valueN, int numbRandomWords){
    //creates a window for the map values to go through
    Vector<Vector<string>> keyHolder = mapText.keys();
    int randKey = randomInteger(0, keyHolder.size()-1);
    Vector<string> window = keyHolder[randKey];
    Vector<string> textVector = window;
    Vector<string> valueVector;
    string valueKey;

    //randomly chooses the value for each key and stores it in a vector
    for(int i = 1; i <= (numbRandomWords - (valueN - 1)); i++){
        if(!mapText.containsKey(window)){
            randKey = randomInteger(0, keyHolder.size()-1);
            window = keyHolder[randKey];
        }
        valueVector = mapText[window];
        int randValue = randomInteger(0,(valueVector.size() - 1));
        valueKey = valueVector[randValue];
        textVector.add(valueKey);
        window.clear();
        window = textVector.subList(i, (valueN - 1));
    }

    //converts the vector to a string
    string text;
    for(int i = 0; i < textVector.size(); i++){
        text += textVector[i] + " ";
    }
    return text;
}
