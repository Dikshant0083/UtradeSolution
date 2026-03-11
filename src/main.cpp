#include "../include/KVStore.h"
#include "../include/CommandParser.h"
#include "../include/Snapshot.h"

#include <iostream>
#include <thread>
#include <chrono>

int main() {

    KVStore store;

    std::thread cleaner([&]() {

        while(true) {

            std::this_thread::sleep_for(std::chrono::seconds(1));

            store.cleanupExpired();
        }
    });

    cleaner.detach();

    std::string line;

    while(std::getline(std::cin, line)) {

        auto cmd = parseCommand(line);

        if(cmd.empty())
            continue;

        if(cmd[0] == "SET") {

            int ex = -1;

            if(cmd.size() == 5 && cmd[3] == "EX")
                ex = stoi(cmd[4]);

            store.set(cmd[1], cmd[2], ex);

            std::cout << "OK\n";
        }

        else if(cmd[0] == "GET")
            std::cout << store.get(cmd[1]) << std::endl;

        else if(cmd[0] == "DEL")
            std::cout << store.del(cmd[1]) << std::endl;

        else if(cmd[0] == "KEYS") {

            auto result = store.keys(cmd[1]);

            for(auto &k : result)
                std::cout << k << std::endl;
        }

        else if(cmd[0] == "TTL")
            std::cout << store.ttl(cmd[1]) << std::endl;

        else if(cmd[0] == "SAVE")
            saveSnapshot(store,"data/snapshot.json");

        else if(cmd[0] == "LOAD")
            loadSnapshot(store,"data/snapshot.json");

        else if(cmd[0] == "STATS")
            store.stats();
    }
}