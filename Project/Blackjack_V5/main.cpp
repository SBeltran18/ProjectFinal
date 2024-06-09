/*
 * File Name: main.ccp
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

int main() {
    // File stream for saving game results
    ofstream outFile("game_results.txt", ios::app); // Append mode

    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "Welcome!" << endl;

    char play;
    bool pbusted, pwins; // Declare pbusted and pwins here

    string deck[DECK_SZ]; // Declare deck here
    int deckIndex = 0; // Declare deckIndex here

    // Deck initialization
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

    // Shuffle the deck
    for (int i = 0; i < DECK_SZ; ++i) {
        int j = rand() % DECK_SZ;
        swap(deck[i], deck[j]);
    }

    do {
        string phand[DECK_SZ], dhand[DECK_SZ];
        int phandSz = 0, dhandSz = 0;

        // Initial deal
        phand[phandSz++] = deck[--deckIndex];
        dhand[dhandSz++] = deck[--deckIndex];
        phand[phandSz++] = deck[--deckIndex];
        dhand[dhandSz++] = deck[--deckIndex];

        pbusted = false;
        char choice;
        pwins = false;

        do {
            cout << "Your turn:"<<endl;
            cout<<endl;
            cout << "Your hand:"<<endl;
            for (int i = 0; i < phandSz; ++i) {
                cout << phand[i] << endl;
            }
            cout << "\nDealer's hand:" << endl;
            cout << "First card: " << dhand[0] << endl;

            cout << "\nDo you want to hit (h) or stand (s)? ";
            cin >> choice;

            if (choice == 'h') {
                phand[phandSz++] = deck[--deckIndex];
                int pval = 0;
                int aces = 0;
                for (int i = 0; i < phandSz; ++i) {
                    string cd = phand[i];
                    string r = cd.substr(0, cd.find(" "));
                    if (r == "Ace") {
                        pval += 11;
                        aces++;
                    } else if (r == "Jack" || r == "Queen" || r == "King") {
                        pval += 10;
                    } else {
                        pval += stoi(r);
                    }
                }
                while (pval > 21 && aces > 0) {
                    pval -= 10;
                    aces--;
                }
                if (pval > 21) {
                    pbusted = true;
                    break;
                }
            }
        } while (choice == 'h');

        // Dealer's turn
        while (true) {
            int dval = 0;
            int aces = 0;
            for (int i = 0; i < dhandSz; ++i) {
                string cd = dhand[i];
                string r = cd.substr(0, cd.find(" "));
                if (r == "Ace") {
                    dval += 11;
                    aces++;
                } else if (r == "Jack" || r == "Queen" || r == "King") {
                    dval += 10;
                } else {
                    dval += stoi(r);
                }
            }
            while (dval > 21 && aces > 0) {
                dval -= 10;
                aces--;
            }
            if (dval >= 17) break;
            dhand[dhandSz++] = deck[--deckIndex];
        }

        // Check for winner
        if (!pbusted) {
            int pval = 0;
            int aces = 0;
            for (int i = 0; i < phandSz; ++i) {
                string cd = phand[i];
                string r = cd.substr(0, cd.find(" "));
                if (r == "Ace") {
                    pval += 11;
                    aces++;
                } else if (r == "Jack" || r == "Queen" || r == "King") {
                    pval += 10;
                } else {
                    pval += stoi(r);
                }
            }
            while (pval > 21 && aces > 0) {
                pval -= 10;
                aces--;
            }

            if (pval > 21) {
                pbusted = true;
                } else {
                int dval = 0;
                int aces = 0;
                for (int i = 0; i < dhandSz; ++i) {
                    string cd = dhand[i];
                    string r = cd.substr(0, cd.find(" "));
                    if (r == "Ace") {
                        dval += 11;
                        aces++;
                    } else if (r == "Jack" || r == "Queen" || r == "King") {
                        dval += 10;
                    } else {
                        dval += stoi(r);
                    }
                }
                while (dval > 21 && aces > 0) {
                    dval -= 10;
                    aces--;
                }
                if (dval > 21 || pval > dval) {
                    pwins = true;
                    
                // Check if dealer's hand value is greater than player's
                if (dval > pval && dval <= 21) {
                    cout << "\nDealer wins!" << endl;
                    outFile << "Dealer wins!" << endl; // Write to file
                    break;
                } else if (dval > 21 || pval > dval) {
                    cout << "\nYou win!" << endl;
                    outFile << "You win!" << endl; // Write to file
                }

                }
            }
        }

        // Display final hands
        cout << "Final hands:" << endl;
        cout<<endl;
        cout << "Your hand:" << endl;
        for (int i = 0; i < phandSz; ++i) {
            cout << phand[i] << endl;
        }
        cout << "\nDealer's hand:" << endl;
        for (int i = 0; i < dhandSz; ++i) {
            cout << dhand[i] << endl;
        }
        
// Display result
if (pbusted) {
    cout<<endl;
    cout<<"Oh No! You went over 21!"<<endl;

    cout << "\nDealer wins!" << endl;
    outFile << "Dealer wins!" << endl; // Write to file
} else {
    cout << "\nYou win!" << endl;
    outFile << "You win!" << endl; // Write to file
}

cout << "\nDo you want to play again? (y/n): ";
cin >> play;

} while (play == 'y');

cout << "Thanks for playing!" << endl;

// Close the file stream
outFile.close();

return 0;
}