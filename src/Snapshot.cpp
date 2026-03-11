#include "../include/Snapshot.h"
#include <fstream>
#include <iostream>

void saveSnapshot(KVStore &store, std::string filename) {

    auto data = store.getAll();

    std::ofstream file(filename);

    file << "{\n";

    bool first = true;

    for(auto &p : data) {

        if(!first)
            file << ",\n";

        file << "\"" << p.first << "\":{\"value\":\""
             << p.second.value << "\",\"expiry\":"
             << p.second.expiry << "}";

        first = false;
    }

    file << "\n}";

    file.close();

    std::cout << "Snapshot saved\n";
}

void loadSnapshot(KVStore &store, std::string filename) {

    std::ifstream file(filename);

    if(!file.is_open()) {

        std::cout << "Snapshot not found\n";

        return;
    }

    std::cout << "Load snapshot not fully implemented\n";
}