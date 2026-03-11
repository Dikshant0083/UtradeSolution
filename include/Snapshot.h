#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "KVStore.h"
#include <string>

void saveSnapshot(KVStore &store, std::string filename);

void loadSnapshot(KVStore &store, std::string filename);

#endif