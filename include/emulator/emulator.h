#pragma once

#include <vector>
#include <string>
#include <proto/faabric.pb.h>

#include <faabric/state/State.h>

unsigned int setEmulatedMessage(const faabric::Message &msg);

void resetEmulator();

void setEmulatorUser(const char *user);

faabric::state::State *getEmulatorState();

void setEmulatorState(faabric::state::State *state);

std::vector<uint8_t> getEmulatorOutputData();

std::string getEmulatorOutputDataString();

std::string getEmulatorUser();