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

using namespace std;

const int DECK_SZ = 52;

// Function Prototypes
void initializeDeck(string[]);
void shuffleDeck(string[]);
string drawCard(string[], int &);
int calculateHandValue(const string[], int);
bool isBusted(int);
void displayHand(const string[], int, bool);
void playGame(ofstream &);

int main() {
    // File stream for saving game results
    ofstream outFile("game_results.txt", ios::app); // Append mode

    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "Welcome!" << endl;

    char play;

    do {
        playGame(outFile);

        cout << "\nDo you want to play again? (y/n): ";
        cin >> play;
    } while (play == 'y');

    cout << "Thanks for playing!" << endl;

    // Close the file stream
    outFile.close();

    return 0;
}

void initializeDeck(string deck[]) {
    int deckIndex = 0;
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
            deck[deckIndex++] = rnk + " of " + suit;
        }
    }
}

void shuffleDeck(string deck[]) {
    for (int i = 0; i < DECK_SZ; ++i) {
        int j = rand() % DECK_SZ;
        swap(deck[i], deck[j]);
    }
}

string drawCard(string deck[], int &deckIndex) {
    return deck[--deckIndex];
}

int calculateHandValue(const string hand[], int handSize) {
    int value = 0;
    int aces = 0;
    for (int i = 0; i < handSize; ++i) {
        string card = hand[i];
        string rank = card.substr(0, card.find(" "));
        if (rank == "Ace") {
            value += 11;
            aces++;
        } else if (rank == "Jack" || rank == "Queen" || rank == "King") {
            value += 10;
        } else {
            value += stoi(rank);
        }
    }
    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    return value;
}

bool isBusted(int value) {
    return value > 21;
}

void displayHand(const string hand[], int handSize, bool isDealer) {
    if (isDealer) {
        cout << "\nDealer's hand:" << endl;
        for (int i = 0; i < handSize; ++i) {
            cout << hand[i] << endl;
        }
    } else {
        cout << "Your hand:" << endl;
        for (int i = 0; i < handSize; ++i) {
            cout << hand[i] << endl;
        }
    }
}

void playGame(ofstream &outFile) {
    bool pbusted, pwins;
    string deck[DECK_SZ];
    int deckIndex = DECK_SZ;

    initializeDeck(deck);
    shuffleDeck(deck);

    string phand[DECK_SZ], dhand[DECK_SZ];
    int phandSz = 0, dhandSz = 0;

    // Initial deal
    phand[phandSz++] = drawCard(deck, deckIndex);
    dhand[dhandSz++] = drawCard(deck, deckIndex);
    phand[phandSz++] = drawCard(deck, deckIndex);
    dhand[dhandSz++] = drawCard(deck, deckIndex);

    pbusted = false;
    char choice;
    pwins = false;

    do {
        cout << "Your turn:" << endl;
        cout << endl;
        displayHand(phand, phandSz, false);
        cout << "\nDealer's hand:" << endl;
        cout << "First card: " << dhand[0] << endl;

        cout << "\nDo you want to hit (h) or stand (s)? ";
        cin >> choice;

        if (choice == 'h') {
            phand[phandSz++] = drawCard(deck, deckIndex);
            int pval = calculateHandValue(phand, phandSz);
            if (isBusted(pval)) {
                pbusted = true;
                break;
            }
        }
    } while (choice == 'h');

    // Dealer's turn
    while (true) {
        int dval = calculateHandValue(dhand, dhandSz);
        if (dval >= 17) break;
        dhand[dhandSz++] = drawCard(deck, deckIndex);
    }

    // Check for winner
    if (!pbusted) {
        int pval = calculateHandValue(phand, phandSz);
        if (pval > 21) {
            pbusted = true;
        } else {
            int dval = calculateHandValue(dhand, dhandSz);
            if (dval > 21 || pval > dval) {
                pwins = true;
            }
            if (dval > pval && dval <= 21) {
                cout << "\nDealer wins!" << endl;
                outFile << "Dealer wins!" << endl;
            } else if (dval > 21 || pval > dval) {
                cout << "\nYou win!" << endl;
                outFile << "You win!" << endl;
            }
        }
    }

    // Display final hands
    cout << "Final hands:" << endl;
    cout << endl;
    displayHand(phand, phandSz, false);
    displayHand(dhand, dhandSz, true);

    // Display result
    if (pbusted) {
        cout << endl;
        cout << "Oh No! You went over 21!" << endl;
        cout << "\nDealer wins!" << endl;
        outFile << "Dealer wins!" << endl;
    } else {
        cout << "\nYou win!" << endl;
        outFile << "You win!" << endl;
    }
}
