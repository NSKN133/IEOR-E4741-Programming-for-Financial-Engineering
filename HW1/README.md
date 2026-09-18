# HW1 NDF 2117 Order Book Metrics in C++

## Overview

With this program I implement Order Book (OB) metrics in Cpp.
Each snapshot includes: bid and ask levels (where each level has a price and quantity).
Additionally, I calculate the best bid, best ask, midpoint, spread, order book imbalance (OBI), and microprice

I also used a short sequence of three snapshots to observe how the midpoint and OBI can change over time. 
For the "stretch" task, the snaps r read from snapshots.txt instead of  being just hard-coded into main.cpp.

## Metrics

### Best Bid & Best Ask

The best bid is the highest bid price in the order book. The best ask is the lowest ask price in the order book.

### Midpoint

The midpoint is the average of the best bid and best ask: mid = (best bid + best ask) / 2

### Spread

The spread is the difference between the best ask and best bid: spread = best ask - best bid

### Order Book Imbalance

Order Book imbalance compares the quantity at the best bid with the quantity at the best ask: OBI = (bid quantity - ask quantity) / (bid quantity + ask quantity)
OBI ranges from -1 to 1. A positive OBI means there is more quantity at the best bid, while a negative OBI means there is more quantity at the best ask!

### Microprice

The microprice is a size-weighted price at the best bid and ask: microprice = (ask price * bid quantity + bid price * ask quantity) / (bid quantity + ask quantity)

## Results

Tested on three snapshots which I stored in snapshots.txt:

Snapshot 1
Best bid: 100 x 40
Best ask: 101 x 60
Mid: 100.5
Spread: 1
OBI: -0.2
Microprice: 100.4

Snapshot 2
Best bid: 100 x 60
Best ask: 101 x 40
Mid: 100.5
Spread: 1
OBI: 0.2
Microprice: 100.6

Snapshot 3
Best bid: 100.5 x 80
Best ask: 101 x 20
Mid: 100.75
Spread: 0.5
OBI: 0.6
Microprice: 100.9

## Mid and OBI Evolution

OBI changes from: Snap1 of -0.20 to Snap2 of  0.20 to  0.60 in Snap3

The midpoint changes from: 100.50 -> 100.50 -> 100.75

Rising OBI means that the quantity at best bid is becoming larger relative to the quantity at the best ask. 
This suggests increasing the buy side pressure. However, a rising OBI does not guarantee that the price will rise.

## How I handled the Input File

For the stretch task, the OB snaps are stored in snapshots.txt and read using ifstream.

The first value in the file gives the number of snapshots (3). For each snapshot, the program reads three bid levels followed by three ask levels. 
Each level contains a price n quantity.

## Files

* main.cpp -> Reads the snapshots and prints the calculated metrics
* HW1_head_orderbook.h -> Def the lvl structure and OrderBook class
* HW1_orderbook.cpp -> Implements the OB metric calculations
* snapshots.txt -> Contains the input data for the three test snapshots


Compiling with: g++ main.cpp HW1_orderbook.cpp -o hw1
and running using: ./hw1

The program reads the snapshots from snapshots.txt and eventually prints the metrics for each snapshot!
