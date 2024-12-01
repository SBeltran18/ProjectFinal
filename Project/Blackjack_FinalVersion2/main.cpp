/*
 * File Name: main.cpp
 * Author: Saul Beltran-Salazar
 * Created on: 5/1/2024 at 9:37PM
 * Purpose:  Blackjack
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

const int DECK_SZ = 52;

// Function Prototypes
void initDec(string[]);
void shufDec(string[]);
string drawCrd(string[], int &);
int calcVal(const string[], int);
bool isBust(int);
void dispHnd(const string[], int, bool);
void playGm(ofstream &);
void hndl1DArr();
void hndlParArr();
void hndl2DArr();
void hndlVec();
void passArr(string[], int);
void passVec(vector<string> &);
void bubSort(int[], int);
void selSort(int[], int);
int linSrch(const int[], int, int);
int binSrch(const int[], int, int);
void splitPrs(string[], int&, string[], int&, string[], int&);
void dblDown(string[], int&, string[], int&, bool &);
bool insrnc(const string[], int, const string[]);
void trivGm();
void guessGm();
void miniGm(string[], int&, string[], int&);

int main() {
    // File stream for saving game results
    ofstream outFile("game.txt", ios::app); // Append mode

    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "Welcome!" << endl;

    char play;
    
    do {
        playGm(outFile);

        cout << "\nDo you want to play again? (y/n): ";
        cout<<endl;
        cin >> play;
    } while (play == 'y');

    cout << "Thanks for playing!" << endl;

    // Close the file stream
    outFile.close();

    // Demonstrating Chapter 7 requirements
    hndl1DArr();
    hndlParArr();
    hndl2DArr();
    hndlVec();

    // Demonstrating Chapter 8 requirements
    int arr[10] = {10, 3, 5, 7, 2, 8, 6, 9, 1, 4};

    // Bubble Sort
    cout << "\nBubble Sort:" << endl;
    bubSort(arr, 10);
    for (int i = 0; i < 10; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Selection Sort
    int arr2[10] = {10, 3, 5, 7, 2, 8, 6, 9, 1, 4};
    cout << "\nSelection Sort:" << endl;
    selSort(arr2, 10);
    for (int i = 0; i < 10; ++i) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    // Linear Search
    int val = 7;
    cout << "\nLinear Search for value " << val << ":" << endl;
    int linIdx = linSrch(arr, 10, val);
    if (linIdx != -1) {
        cout << "Value found at index " << linIdx << endl;
    } else {
        cout << "Value not found" << endl;
    }

    // Binary Search
    cout << "\nBinary Search for value " << val << ":" << endl;
    int binIdx = binSrch(arr, 10, val);
    if (binIdx != -1) {
        cout << "Value found at index " << binIdx << endl;
    } else {
        cout << "Value not found" << endl;
    }

    return 0;
}

// Function to initialize the deck with card values
void initDec(string deck[]) {
    int idx = 0;
    for (int r = 1; r <= 13; ++r) {
        for (int s = 0; s < 4; ++s) {
            string rnk, suit;
            if (r == 1) {
                rnk = "Ace";
            } else if (r == 11) {
                rnk = "Jack";
            } else if (r == 12) {
                rnk = "Queen";
            } else if (r == 13) {
                rnk = "King";
            } else {
                rnk = to_string(r);
            }
            if (s == 0) {
                suit = "Clubs";
            } else if (s == 1) {
                suit = "Diamonds";
            } else if (s == 2) {
                suit = "Hearts";
            } else if (s == 3) {
                suit = "Spades";
            }
            deck[idx++] = rnk + " of " + suit;
        }
    }
}

// Function to shuffle the deck
void shufDec(string deck[]) {
    for (int i = 0; i < DECK_SZ; ++i) {
        int j = rand() % DECK_SZ;
        swap(deck[i], deck[j]);
    }
}

// Function to draw a card from the deck
string drawCrd(string deck[], int &idx) {
    return deck[--idx];
}

// Function to calculate the value of a hand
int calcVal(const string hand[], int hSz) {
    int val = 0;
    int aces = 0;
    for (int i = 0; i < hSz; ++i) {
        string card = hand[i];
        string rnk = card.substr(0, card.find(" "));
        if (rnk == "Ace") {
            val += 11;
            aces++;
        } else if (rnk == "Jack" || rnk == "Queen" || rnk == "King") {
            val += 10;
        } else {
            val += stoi(rnk);
        }
    }
    while (val > 21 && aces > 0) {
        val -= 10;
        aces--;
    }
    return val;
}

// Function to check if the hand value is over 21
bool isBust(int val) {
    return val > 21;
}

// Function to display a hand
void dispHnd(const string hand[], int hSz, bool isDeal) {
    if (isDeal) {
        cout << "\nDealer's hand:" << endl;
        for (int i = 0; i < hSz; ++i) {
            cout << hand[i] << endl;
        }
    } else {
        cout << "Your hand:" << endl;
        for (int i = 0; i < hSz; ++i) {
            cout << hand[i] << endl;
        }
    }
}

// Function to play the game
void playGm(ofstream &outFile) {
    bool pBust, pWins;
    string deck[DECK_SZ];
    int idx = DECK_SZ;

    initDec(deck);
    shufDec(deck);

    string pHand[DECK_SZ], dHand[DECK_SZ];
    int pSz = 0, dSz = 0;

    // Initial deal
    pHand[pSz++] = drawCrd(deck, idx);
    dHand[dSz++] = drawCrd(deck, idx);
    pHand[pSz++] = drawCrd(deck, idx);
    dHand[dSz++] = drawCrd(deck, idx);

    pBust = false;
    char choice;
    pWins = false;

    // Insurance option
    if (insrnc(dHand, dSz, pHand)) {
        cout << "Insurance taken.\n";
        cout<<endl;
    } else {
        cout << "No insurance taken.\n";
        cout<<endl;
    }

    do {
        cout << "Your turn:" << endl;
        cout << endl;
        dispHnd(pHand, pSz, false);
        cout << "\nDealer's hand:" << endl;
        cout<<endl;
        cout << "First card: " << dHand[0] << endl;

        cout << "\nDo you want to hit (h), stand (s), split pairs (p), or double down (d)? ";
        cin >> choice;

        if (choice == 'h') {
            pHand[pSz++] = drawCrd(deck, idx);
            int pVal = calcVal(pHand, pSz);
            if (isBust(pVal)) {
                pBust = true;
                break;
            }
        } else if (choice == 'p' && pSz == 2 && pHand[0].substr(0, pHand[0].find(" ")) == pHand[1].substr(0, pHand[1].find(" "))) {
            string hand1[DECK_SZ], hand2[DECK_SZ];
            int h1Sz = 0, h2Sz = 0;
            splitPrs(pHand, pSz, hand1, h1Sz, hand2, h2Sz);
        } else if (choice == 'd') {
            dblDown(pHand, pSz, deck, idx, pBust);
            break;
        }
    } while (choice == 'h');

    if (!pBust) {
        while (calcVal(dHand, dSz) < 17) {
            dHand[dSz++] = drawCrd(deck, idx);
        }
    }

    cout << "\nFinal hands:" << endl;
    cout<<endl;
    dispHnd(pHand, pSz, false);
    dispHnd(dHand, dSz, true);

    int dVal = calcVal(dHand, dSz);
    int pVal = calcVal(pHand, pSz);

    if (pBust || (!isBust(dVal) && dVal > pVal)) {
        cout << "\nDealer wins!" << endl;
        outFile << "Dealer wins" << endl;
    } else if (dVal == pVal) {
        cout << "\nIt's a tie!" << endl;
        outFile << "It's a tie" << endl;
    } else {
        cout << "\nYou win!" << endl;
        outFile << "You win" << endl;
        pWins = true;
    }

    if (pWins) {
        cout << "\nCongratulations!" << endl;
    } else {
        cout << "\nBetter luck next time." << endl;
    }

    // Mini games after each round
    miniGm(deck, idx, pHand, pSz);
}

// Function to handle splitting pairs
void splitPrs(string pHand[], int &pSz, string hand1[], int &h1Sz, string hand2[], int &h2Sz) {
    hand1[h1Sz++] = pHand[0];
    hand2[h2Sz++] = pHand[1];
    pSz = 0;
    cout << "Hands have been split into two hands:" << endl;
    cout << "Hand 1: " << hand1[0] << endl;
    cout << "Hand 2: " << hand2[0] << endl;
}

// Function to handle doubling down
void dblDown(string pHand[], int &pSz, string deck[], int &idx, bool &pBust) {
    pHand[pSz++] = drawCrd(deck, idx);
    int pVal = calcVal(pHand, pSz);
    if (isBust(pVal)) {
        pBust = true;
    }
}

// Function to handle insurance
bool insrnc(const string dHand[], int dSz, const string pHand[]) {
    if (dHand[0].find("Ace") != string::npos) {
        char insr;
        cout << "Dealer's face-up card is an Ace. Do you want insurance? (y/n): ";
        cout<<endl;
        cin >> insr;
        if (insr == 'y') {
            return true;
        }
    }
    return false;
}

// Function to handle 1D array
void hndl1DArr() {
    cout << "\nHandling 1D array" << endl;
    string arr[5] = {"Ace", "King", "Queen", "Jack", "Ten"};
    for (int i = 0; i < 5; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Function to handle parallel arrays
void hndlParArr() {
    cout << "\nHandling parallel arrays" << endl;
    string suits[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
    int vals[4] = {1, 2, 3, 4};
    for (int i = 0; i < 4; ++i) {
        cout << suits[i] << ": " << vals[i] << endl;
    }
}

// Function to handle 2D array
void hndl2DArr() {
    cout << "\nHandling 2D array" << endl;
    int arr[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to handle vectors
void hndlVec() {
    cout << "\nHandling vectors" << endl;
    vector<string> vec = {"Ace", "King", "Queen", "Jack", "Ten"};
    for (const string &val : vec) {
        cout << val << " ";
    }
    cout << endl;
}

// Function for passing an array to a function
void passArr(string arr[], int size) {
    cout << "\nPassing array to function" << endl;
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Function for passing a vector to a function
void passVec(vector<string> &vec) {
    cout << "\nPassing vector to function" << endl;
    for (const string &val : vec) {
        cout << val << " ";
    }
    cout << endl;
}

// Bubble sort implementation
void bubSort(int arr[], int size) {
    bool swpd;
    do {
        swpd = false;
        for (int i = 0; i < size - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swpd = true;
            }
        }
    } while (swpd);
}

// Selection sort implementation
void selSort(int arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

// Linear search implementation
int linSrch(const int arr[], int size, int val) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == val) {
            return i;
        }
    }
    return -1;
}

// Binary search implementation
int binSrch(const int arr[], int size, int val) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == val) {
            return mid;
        } else if (arr[mid] < val) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// Function for a trivia mini-game
void trivGm() {
    cout << "\nTrivia mini-game!" << endl;
    cout<<endl;
    cout << "What is the highest hand in Blackjack? (Hint: two words, first letter B)" << endl;
    string ans;
    cin.ignore();
    getline(cin, ans);
    if (ans == "Blackjack" || ans == "blackjack") {
        cout << "Correct!" << endl;
    } else {
        cout << "Wrong! The correct answer is Blackjack." << endl;
    }
}

// Function for a guessing mini-game
void guessGm() {
    cout << "\nGuessing mini-game!" << endl;
    int num = rand() % 10 + 1;
    int guess;
    cout << "Guess a number between 1 and 10: ";
    cout<<endl;
    cin >> guess;
    if (guess == num) {
        cout << "Correct! You guessed the number." << endl;
    } else {
        cout << "Wrong! The correct number was " << num << "." << endl;
    }
}

// Function to choose a mini-game
void miniGm(string deck[], int &idx, string hand[], int &hSz) {
    cout << "\nChoose a mini-game: 1 for trivia, 2 for guessing: ";
    int choice;
    cin >> choice;
    if (choice == 1) {
        trivGm();
    } else if (choice == 2) {
        guessGm();
    }
}
