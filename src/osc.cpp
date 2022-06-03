#include "osc.h"

#define OSC_ADDRESS "localhost"
#define OSC_PORT "8000"

lo_address t = lo_address_new(OSC_ADDRESS, OSC_PORT);

std::string track_uniform_name[] = { 
    "u_kick",   "u_snare",  "u_perc", "u_sample", "u_bass",     "u_lead",   "u_arp",    "u_chord", 
    "u_fx1",    "u_fx2",    "u_tape", "u_master", "u_perform",  "u_module", "u_lights", "u_motion"
};

size_t track_counter[16]; 

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

void send(const std::string& _folder, float _value1, float _value2, float _value3) {
    lo_message m = lo_message_new();
    lo_message_add_float(m, _value1);
    lo_message_add_float(m, _value2);
    lo_message_add_float(m, _value3);
    lo_send_message(t, _folder.c_str(), m);
    lo_message_free(m);
}

void send(const std::string& _folder, float _value1, float _value2, float _value3, float _value4) {
    lo_message m = lo_message_new();
    lo_message_add_float(m, _value1);
    lo_message_add_float(m, _value2);
    lo_message_add_float(m, _value3);
    lo_message_add_float(m, _value4);
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
    if (_channel == 0)
        return;
        
    if (_id == T3::NOTE_ON) {
        track_counter[_channel - 1]++;
        send(track_uniform_name[_channel - 1], _key/255.0f, 1.0f, track_counter[_channel - 1] * 0.001f);
    }
    else if (_id == T3::NOTE_OFF)
        send(track_uniform_name[_channel - 1], 0.0f, 0.9f, track_counter[_channel - 1] * 0.001f);
}