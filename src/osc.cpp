#include "osc.h"

#define OSC_ADDRESS "localhost"
#define OSC_PORT "8000"

lo_address t = lo_address_new(OSC_ADDRESS, OSC_PORT);

std::string track_uniform_name[] = { 
    "u_kick",   "u_snare",  "u_perc", "u_sample", "u_bass",     "u_lead",   "u_arp",    "u_chord", 
    "u_fx1",    "u_fx2",    "u_tape", "u_master", "u_perform",  "u_module", "u_lights", "u_motion"
};

std::string trackUniformName(size_t _track) {
    return track_uniform_name[_track];
}

void send(const std::string& _folder, float _value) {
    lo_message m = lo_message_new();
    lo_message_add_float(m, _value);
    lo_send_message(t, _folder.c_str(), m);
    lo_message_free(m);
}

void send(const std::string& _folder, float _value1, float _value2) {
    lo_message m = lo_message_new();
    lo_message_add_float(m, _value1);
    lo_message_add_float(m, _value2);
    lo_send_message(t, _folder.c_str(), m);
    lo_message_free(m);
}


void send(const std::string& _folder, const std::string& _value) {
    lo_message m = lo_message_new();
    lo_message_add_string(m, _value.c_str());
    lo_send_message(t, _folder.c_str(), m);
    lo_message_free(m);
}


void send(T3::opz_event_id _id, int _value) {
    if (_id == T3::STEP_ZERO)
        send("streams", "restart");
}

void send(T3::midi_id _id, size_t _channel, size_t _key, size_t _value) {
    if (_id == T3::NOTE_ON)
        send(track_uniform_name[ 1 + _channel], _key/255.0f, _value/255.0f);
    else if (_id == T3::NOTE_ON)
        send(track_uniform_name[ 1 + _channel], 0.0f, 0.0f);
}