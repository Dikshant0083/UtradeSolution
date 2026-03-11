#include "../include/KVStore.h"
#include <iostream>
#include <regex>

void KVStore::set(std::string key, std::string value, int ex) {

    std::lock_guard<std::mutex> lock(mtx);

    Entry e;

    e.value = value;

    if(ex > 0)
        e.expiry = time(nullptr) + ex;
    else
        e.expiry = 0;

    store[key] = e;
}

std::string KVStore::get(std::string key) {

    std::lock_guard<std::mutex> lock(mtx);

    if(store.find(key) == store.end())
        return "(nil)";

    Entry &e = store[key];

    if(e.expiry && e.expiry < time(nullptr)) {

        store.erase(key);
        expiredCleaned++;

        return "(nil)";
    }

    return e.value;
}

std::string KVStore::del(std::string key) {

    std::lock_guard<std::mutex> lock(mtx);

    if(store.erase(key))
        return "OK";

    return "(nil)";
}

std::vector<std::string> KVStore::keys(std::string pattern) {

    std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> result;

    std::string regexPattern = std::regex_replace(pattern, std::regex("\\*"), ".*");

    std::regex r(regexPattern);

    for(auto &p : store) {

        if(std::regex_match(p.first, r))
            result.push_back(p.first);
    }

    return result;
}

int KVStore::ttl(std::string key) {

    std::lock_guard<std::mutex> lock(mtx);

    if(store.find(key) == store.end())
        return -2;

    Entry &e = store[key];

    if(e.expiry == 0)
        return -1;

    int remaining = e.expiry - time(nullptr);

    if(remaining < 0)
        return -2;

    return remaining;
}

void KVStore::cleanupExpired() {

    std::lock_guard<std::mutex> lock(mtx);

    time_t now = time(nullptr);

    for(auto it = store.begin(); it != store.end();) {

        if(it->second.expiry && it->second.expiry < now) {

            it = store.erase(it);

            expiredCleaned++;

        }
        else
            ++it;
    }
}

void KVStore::stats() {

    std::lock_guard<std::mutex> lock(mtx);

    size_t mem = store.size() * sizeof(Entry);

    std::cout << "Total Keys: " << store.size() << std::endl;
    std::cout << "Expired Cleaned: " << expiredCleaned << std::endl;
    std::cout << "Memory Estimate: " << mem << " bytes" << std::endl;
}

std::unordered_map<std::string, Entry> KVStore::getAll() {

    std::lock_guard<std::mutex> lock(mtx);

    return store;
}

void KVStore::loadAll(std::unordered_map<std::string, Entry> data) {

    std::lock_guard<std::mutex> lock(mtx);

    store = data;
}