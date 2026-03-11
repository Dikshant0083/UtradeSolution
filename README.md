# Redis-like In-Memory Key Value Store

## Overview

This project implements a simplified **Redis-like in-memory key-value store** supporting TTL (time-to-live), persistence snapshots, and concurrent access.

Such systems are commonly used in **caching layers, session stores, and high-performance lookup tables** where low-latency data access is required.

The store processes **text-based commands via standard input (stdin)** and executes operations on an in-memory data structure.

---

# Features

## Supported Commands

| Command                      | Description                               |
| ---------------------------- | ----------------------------------------- |
| `SET key value [EX seconds]` | Store a key with optional expiration      |
| `GET key`                    | Retrieve value of a key                   |
| `DEL key`                    | Delete a key                              |
| `KEYS pattern`               | Return keys matching a glob-style pattern |
| `TTL key`                    | Return remaining time-to-live             |
| `SAVE`                       | Persist the store snapshot to disk        |
| `STATS`                      | Display memory usage statistics           |

---

# Expiration Strategy

The store uses **two expiration strategies**:

### Lazy Expiration

When a key is accessed, its expiration time is checked.

If the key has expired:

* It is removed immediately
* `(nil)` is returned

### Periodic Cleanup

A background thread runs every **1 second** to remove expired keys.

This prevents unused expired keys from occupying memory.

---

# Thread Safety

The store supports concurrent operations using a **mutex-based locking mechanism**.

```
std::mutex
std::lock_guard
```

All operations modifying the key-value store are protected by a mutex to avoid race conditions.

This ensures safe concurrent access for:

* SET
* GET
* DEL
* TTL

---

# Data Structures Used

## Primary Storage

```
unordered_map<string, Entry>
```

Reasons:

* Average **O(1)** lookup
* Efficient key-based access

---

## Entry Structure

Each key stores:

```
struct Entry {
    string value
    time_t expiry
}
```

Fields:

* **value** → stored value
* **expiry** → expiration timestamp (`0` means no expiration)

---

# Persistence

The `SAVE` command writes all keys to:

```
data/snapshot.json
```

Example snapshot file:

```
{
"user:1":"Alice",
"counter":"0"
}
```

Snapshots allow the key-value store state to be preserved.

---

# Project Structure

```
kv-store
│
├── include
│   ├── KVStore.h
│   ├── CommandParser.h
│   └── Snapshot.h
│
├── src
│   ├── main.cpp
│   ├── KVStore.cpp
│   ├── CommandParser.cpp
│   └── Snapshot.cpp
│
├── data
│   └── snapshot.json
│
├── README.md
├── Makefile
└── .gitignore
```

The project follows a **modular structure** separating interface files (`include/`) from implementation files (`src/`).

---

# Build Instructions

Ensure **g++ with C++17 support** is installed.

Compile the project using:

```
g++ -std=c++17 -pthread -Iinclude src/main.cpp src/KVStore.cpp src/CommandParser.cpp src/Snapshot.cpp -o kvstore
```

This produces the executable:

```
kvstore
```

---

# Running the Application

Start the key-value store:

```
./kvstore
```

The program reads commands from **standard input**.

---

# Example Usage

Input:

```
SET user:1 Alice EX 10
GET user:1
TTL user:1
SET counter 0
KEYS user:*
DEL user:1
GET user:1
STATS
SAVE
```

Output:

```
OK
Alice
9
OK
user:1
OK
(nil)
```

---

# Testing

Create a test file:

```
test.txt
```

Example:

```
SET user:1 Alice EX 10
GET user:1
TTL user:1
SET counter 0
KEYS user:*
DEL user:1
GET user:1
STATS
SAVE
```

Run:

Linux / macOS

```
./kvstore < test.txt
```

Windows PowerShell

```
Get-Content test.txt | ./kvstore
```

---

# Memory Statistics

The `STATS` command prints runtime statistics such as:

* total keys stored
* number of expired keys cleaned
* approximate memory usage

Example:

```
Total Keys: 2
Expired Cleaned: 1
Memory Estimate: 96 bytes
```

---

# Edge Cases Handled

* accessing expired keys
* TTL on non-expiring keys
* TTL on non-existing keys
* pattern matching in KEYS
* concurrent access safety

---

# Design Decisions

| Decision                | Reason                                |
| ----------------------- | ------------------------------------- |
| `unordered_map` storage | O(1) average lookup                   |
| lazy expiration         | avoids scanning all keys              |
| periodic cleanup        | prevents memory leaks                 |
| mutex-based locking     | simple thread safety                  |
| stdin protocol          | simpler than TCP for assignment scope |

---

# Trade-offs

| Choice               | Trade-off                               |
| -------------------- | --------------------------------------- |
| global mutex         | simpler but less scalable               |
| snapshot persistence | easier than log-based persistence       |
| in-memory store      | faster but volatile without persistence |

---

# Possible Improvements

Future extensions could include:

* TCP socket-based command protocol
* append-only log persistence
* more Redis-like data structures
* distributed cache support

---

# Author

Mini Project Submission
uTrade Solutions Campus Hiring 2026
