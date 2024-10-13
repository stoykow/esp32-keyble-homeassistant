#include <math.h>
#include <string>
#include <sstream>
#include "eQ3_message.h"

using namespace std;

/**
 * Checks if a certain bit in a value is set.
 * @param {number} value - The value.
 * @param {number} bit_index - The zero-based index of the bit to check. 0 means the least-significant bit.
 * @returns {boolean} True if the bit is set, false otherwise.
 */
bool is_bit_set(char value, int bit_index)
{
  return ((value & (1 << bit_index)) != 0);
}

// -----------------------------------------------------------------------------
// --[getStatusByte]------------------------------------------------------------
// -----------------------------------------------------------------------------
char eQ3Message::MessageFragment::getStatusByte() {
    return data[0];
}

// -----------------------------------------------------------------------------
// --[getRemainingFragmentCount]------------------------------------------------
// -----------------------------------------------------------------------------
int eQ3Message::MessageFragment::getRemainingFragmentCount() {
    return getStatusByte() & 0x7F;
}

// -----------------------------------------------------------------------------
// --[isFirst]------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::MessageFragment::isFirst() {
    return static_cast<bool>(getStatusByte() & (1 << 7));
}

// -----------------------------------------------------------------------------
// --[isLast]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::MessageFragment::isLast() {
    return getRemainingFragmentCount() == 0;
}

// -----------------------------------------------------------------------------
// --[isComplete]---------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::MessageFragment::isComplete() {
    return isFirst() && isLast();
}

// -----------------------------------------------------------------------------
// --[getType]-----------------------------------------......-------------------
// -----------------------------------------------------------------------------
char eQ3Message::MessageFragment::getType() {
    if (isFirst()) {
        return data[1];
    }
    return false;
}

// -----------------------------------------------------------------------------
// --[getData]------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string eQ3Message::MessageFragment::getData() {
    return data.substr(1);
}

// -----------------------------------------------------------------------------
// --[encode]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::Message::Message(string data) {
    this->data = data;
}

string eQ3Message::Message::encode(ClientState *state) {
    return "";
}

// -----------------------------------------------------------------------------
// --[isSecure]-----------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::Message::isSecure() {
    return static_cast<bool>(id & (1 << 7));
}

// -----------------------------------------------------------------------------
// --[isTypeSecure]-------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::Message::isTypeSecure(char type) {
    return static_cast<bool>(type & (1 << 7));
}

// -----------------------------------------------------------------------------
// --[decode]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
void eQ3Message::Message::decode() {
}

// -----------------------------------------------------------------------------
// --[Connection_Info_Message]--------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::Connection_Info_Message::Connection_Info_Message() {
    id = 0x03;
}

// -----------------------------------------------------------------------------
// --[getUserId]----------------------------------------------------------------
// -----------------------------------------------------------------------------
char eQ3Message::Connection_Info_Message::getUserId() {
    return data[1];
}

// -----------------------------------------------------------------------------
// --[getRemoteSessionNonce]----------------------------------------------------
// -----------------------------------------------------------------------------
string eQ3Message::Connection_Info_Message::getRemoteSessionNonce() {
    return data.substr(2, 8);
}

// -----------------------------------------------------------------------------
// --[getBootloaderVersion]-----------------------------------------------------
// -----------------------------------------------------------------------------
char eQ3Message::Connection_Info_Message::getBootloaderVersion() {
    return data[11];
}

// -----------------------------------------------------------------------------
// --[getAppVersion]------------------------------------------------------------
// -----------------------------------------------------------------------------
char eQ3Message::Connection_Info_Message::getAppVersion() {
    return data[12];
}

// -----------------------------------------------------------------------------
// --[Status_Changed_Message]---------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::Status_Changed_Message::Status_Changed_Message() {
    id = 0x05;
}

// -----------------------------------------------------------------------------
// --[Status_Info_Message]------------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::Status_Info_Message::Status_Info_Message() {
    id = 0x83; // TODO: only fits in unsigned char
}

// -----------------------------------------------------------------------------
// --[getLockStatus]------------------------------------------------------------
// -----------------------------------------------------------------------------
LockStatus eQ3Message::Status_Info_Message::getLockStatus() {
    return (LockStatus)(data[2] & 0x07);
}

// -----------------------------------------------------------------------------
// --[getUserRightType]---------------------------------------------------------
// -----------------------------------------------------------------------------
int eQ3Message::Status_Info_Message::getUserRightType() {
    return (data[0] & 0x30) >> 4;
}

// -----------------------------------------------------------------------------
// --[getBatteryStatus]---------------------------------------------------------
// -----------------------------------------------------------------------------
BatteryStatus eQ3Message::Status_Info_Message::getBatteryStatus() {
    return (BatteryStatus)(is_bit_set(data[1], 7));
}

// -----------------------------------------------------------------------------
// --[StatusRequestMessage]-----------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::StatusRequestMessage::StatusRequestMessage() {
    id = 0x82; // TODO: only fits in unsigned char
}

// -----------------------------------------------------------------------------
// --[encode]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string eQ3Message::StatusRequestMessage::encode(ClientState *state) {
    std::stringstream ss;
    time_t theTime = time(nullptr);
    struct tm *aTime = localtime(&theTime);
    ss.put((char) (aTime->tm_year - 2000));
    ss.put((char) (aTime->tm_mon + 1));
    ss.put((char) (aTime->tm_mday));
    ss.put((char) (aTime->tm_hour));
    ss.put((char) (aTime->tm_min));
    ss.put((char) (aTime->tm_sec));
    return ss.str();
}

// -----------------------------------------------------------------------------
// --[Connection_Close_Message]-------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::Connection_Close_Message::Connection_Close_Message() {
    id = 0x06;
}

// -----------------------------------------------------------------------------
// --[Connection_Request_Message]-----------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::Connection_Request_Message::Connection_Request_Message() {
    id = 0x02;
}

// -----------------------------------------------------------------------------
// --[encode]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string eQ3Message::Connection_Request_Message::encode(ClientState *state) {
    std::stringstream ss;
    ss.put(state->user_id);
    ss << state->local_session_nonce;
    return ss.str();
}

// -----------------------------------------------------------------------------
// --[CommandMessage]-----------------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::CommandMessage::CommandMessage(char command) {
    this->command = command;
    id = 0x87; // TODO: only fits in unsigned char
}

// -----------------------------------------------------------------------------
// --[encode]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string eQ3Message::CommandMessage::encode(ClientState *state) {
    std::stringstream ss;
    ss.put(this->command);
    return ss.str();
}

// -----------------------------------------------------------------------------
// --[AnswerWithoutSecurityMessage]---------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::AnswerWithoutSecurityMessage::AnswerWithoutSecurityMessage() {
    id = 0x01;
}

// -----------------------------------------------------------------------------
// --[AnswerWithSecurityMessage]------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::AnswerWithSecurityMessage::AnswerWithSecurityMessage() {
    id = 0x81; // TODO: only fits in unsigned char
}

// -----------------------------------------------------------------------------
// --[getA]---------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::AnswerWithSecurityMessage::getA() {
    return is_bit_set(data[0], 6);
}

// -----------------------------------------------------------------------------
// --[getB]---------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool eQ3Message::AnswerWithSecurityMessage::getB() {
    return is_bit_set(data[0], 7);
}

// -----------------------------------------------------------------------------
// --[PairingRequestMessage]----------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::PairingRequestMessage::PairingRequestMessage() {
    id = 0x04;
}

// -----------------------------------------------------------------------------
// --[encode]-------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string eQ3Message::PairingRequestMessage::encode(ClientState *state) {
    return data;
}

// -----------------------------------------------------------------------------
// --[FragmentAckMessage]-------------------------------------------------------
// -----------------------------------------------------------------------------
eQ3Message::FragmentAckMessage::FragmentAckMessage(char fragment_id) {
    std::stringstream ss;
    ss << id;
    ss << fragment_id;
    data = ss.str();
}
