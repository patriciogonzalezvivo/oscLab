#pragma once

#include <string>
#include "lo/lo_cpp.h"
#include "libopz/opz_device.h"

std::string trackUniformName(size_t _track);

void send(const std::string& _folder, float _value);
void send(const std::string& _folder, const std::string& _value);
void send(T3::opz_event_id _id, int _value);
void send(T3::midi_id _id, size_t channel, size_t _key, size_t _value);