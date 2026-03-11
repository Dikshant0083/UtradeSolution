# Redis-like In-Memory Key Value Store

This project implements a simple Redis-like key-value store supporting TTL, snapshots, and concurrency.

## Features

Core Features

SET key value [EX seconds]
GET key
DEL key
KEYS pattern
TTL key
SAVE snapshot
STATS memory statistics

Expiration

Lazy expiration on access
Periodic cleanup every 1 second

Thread Safety

Protected using mutex

Bonus Features

INCR / DECR atomic integer operations
LRU eviction when key count exceeds limit
TCP server support (optional extension)

## Data Structures

unordered_map<string, Entry>

Entry
value
expiry timestamp

LRU tracking uses a list structure.

## Build Instructions

Make sure g++ is installed.

Compile using:

g++ -std=c++17 -pthread -Iinclude src/main.cpp src/KVStore.cpp src/CommandParser.cpp src/Snapshot.cpp -o kvstore

## Run

Run the program:

./kvstore

Program reads commands from stdin.

## Example Test

SET user:1 Alice EX 10
GET user:1
TTL user:1
SET counter 0
KEYS user:*
DEL user:1
GET user:1
STATS
SAVE

Expected Output

OK
Alice
9
OK
user:1
OK
(nil)

## Snapshot

SAVE writes snapshot to

data/snapshot.json

## Edge Cases Handled

Expired keys removed
TTL on non-existing keys
Pattern matching
Thread-safe operations

## Tradeoffs

Single mutex used for simplicity
Snapshot persistence instead of append-only log
stdin protocol used instead of TCP for assignment simplicity