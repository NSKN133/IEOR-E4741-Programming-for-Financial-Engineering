#include <iostream>
#include <fstream>
#include "HW1_head_orderbook.h"

using namespace std;

int main() {

    // TASK 4 (STRETCH):
    // Read the snapshots from a small input file
    // rather than hard-coding them.

    ifstream inputFile("snapshots.txt");

    if (!inputFile) {
        cout << "Error" << '\n';
        return 1;
    }

    int numberOfSnapshots;
    inputFile >> numberOfSnapshots;


    // TASK 3:
    // Given a short sequence of snapshots, print how mid and OBI evolve;
    // comment on what a rising OBI suggests.

    for (int snapshot = 1; snapshot <= numberOfSnapshots; snapshot++) { // Running for amount of Snapshots(3)

        // TASK 1:
        // Read a top-of-book snapshot (arrays of (price, qty) for bids and asks)
        // and compute best bid/ask, mid, and spread.

        Level bids[3];
        Level asks[3];

        // Read 3 bid levels from the file
        for (int i = 0; i < 3; i++) {
            inputFile >> bids[i].price >> bids[i].quantity;
        }

        // Read 3 ask levels from the file
        for (int i = 0; i < 3; i++) {
            inputFile >> asks[i].price >> asks[i].quantity;
        }

        OrderBook book(bids, 3, asks, 3);

        Level bid = book.bestBid();
        Level ask = book.bestAsk();


        // TASK 2:
        // Compute the microprice (size-weighted touch) and
        // order-book imbalance (OBI in [-1, 1]).
        // Prints everything

        cout << "\nSnapshot " << snapshot << endl;
        cout << "Best bid: " << bid.price << " x " << bid.quantity << endl;
        cout << "Best ask: " << ask.price << " x " << ask.quantity << endl;
        cout << "Mid: " << book.mid() << endl;
        cout << "Spread: " << book.spread() << endl;
        cout << "OBI: " << book.imbalance() << endl;
        cout << "Microprice: " << book.microprice() << endl;
    }

    inputFile.close();


    cout << "\nObservation:" << endl;
    cout << "A increasing OBI means the quantity at the best bid is becoming larger relative to the quantity at the best ask." << endl;
    cout << "This suggests increasing buy-side pressure, but it does not guarantee that the price will rise!" << endl;

    return 0;
}