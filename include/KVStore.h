#ifndef KVSTORE_H
#define KVSTORE_H

#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>
#include <ctime>

struct Entry {
    std::string value;
    time_t expiry;
};

class KVStore {

private:

    std::unordered_map<std::string, Entry> store;
    std::mutex mtx;
    int expiredCleaned = 0;

public:

    void set(std::string key, std::string value, int ex = -1);

    std::string get(std::string key);

    std::string del(std::string key);

    std::vector<std::string> keys(std::string pattern);

    int ttl(std::string key);

    void cleanupExpired();

    void stats();

    std::unordered_map<std::string, Entry> getAll();

    void loadAll(std::unordered_map<std::string, Entry> data);
};

#endif