#include "HW1_head_orderbook.h"

OrderBook::OrderBook(Level inputBids[], int numberOfBids,
                     Level inputAsks[], int numberOfAsks) {

    bidSize = numberOfBids;
    askSize = numberOfAsks;

    for (int i = 0; i < bidSize; i++) {
        bids[i] = inputBids[i];
    }

    for (int i = 0; i < askSize; i++) {
        asks[i] = inputAsks[i];
    }
}

// TASK 1:
// Read a top-of-book snapshot (arrays of (price, qty) for bids and asks)
// and compute best bid/ask, mid, and spread.

Level OrderBook::bestBid() {

    Level best = bids[0];

    for (int i = 1; i < bidSize; i++) {
        if (bids[i].price > best.price) {
            best = bids[i];
        }
    }
        return best;
}

Level OrderBook::bestAsk() {

    Level best = asks[0];

    for(int i = 1; i < askSize; i++){
        if(asks[i].price < best.price)
        best= asks[i];
    }
    return best;
}

double OrderBook::mid() {
    Level bid = bestBid();
    Level ask = bestAsk();

    return (bid.price + ask.price) / 2.0;
}

double OrderBook::spread() {
    Level bid = bestBid();
    Level ask = bestAsk();

    return ask.price - bid.price;
}

// TASK 2:
// Compute the microprice (size-weighted touch) and
// order-book imbalance (OBI in [-1, 1]).

double OrderBook::imbalance() {
    Level bid = bestBid();
    Level ask = bestAsk();

    return static_cast<double>(bid.quantity - ask.quantity)/(bid.quantity + ask.quantity);
}

double OrderBook::microprice() {
    Level bid = bestBid();
    Level ask = bestAsk();

    return static_cast<double>((ask.price * bid.quantity) + (bid.price * ask.quantity)) / (bid.quantity + ask.quantity);
}