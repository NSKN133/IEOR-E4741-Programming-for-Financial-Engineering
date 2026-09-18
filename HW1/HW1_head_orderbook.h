#ifndef HW1_HEAD_ORDERBOOK_H
#define HW1_HEAD_ORDERBOOK_H

// Single price lvl in the OB
struct Level { 
    double price;
    int quantity;
};

class OrderBook {
private:
Level bids[10];
Level asks[10];

int bidSize;
int askSize;
public:
    OrderBook(Level inputBids[], int numberOfBids,
        Level inputAsks[], int numberOfAsks);

    Level bestBid();
    Level bestAsk();


    double mid();
    double spread();
    double imbalance();
    double microprice();
};

#endif