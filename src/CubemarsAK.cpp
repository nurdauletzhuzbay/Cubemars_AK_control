#include "CubemarsAK.h"


CubemarsAK::CubemarsAK(uint8_t csPin) : mcp2515(csPin){}

CubemarsAK::~CubemarsAK()
{}

void CubemarsAK::initializeCAN() {
    mcp2515.reset(); 
    mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ); 
    mcp2515.setNormalMode(); 
}

uint32_t CubemarsAK::canId(int id, AKMode Mode_set) {
    return (uint32_t)(id | Mode_set << 8);
}


void CubemarsAK::comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len) {
    canMsg2.can_id = id | CAN_EFF_FLAG;
    canMsg2.can_dlc = (len > 8) ? 8 : len;
    for (uint8_t i = 0; i < len; i++) {
        canMsg2.data[i] = data[i];
    }
    MCP2515::ERROR sendStatus = mcp2515.sendMessage(MCP2515::TXB1, &canMsg2);
    if (sendStatus != MCP2515::ERROR_OK) {
        Serial.println("Error sending message...");
    }
}

void CubemarsAK::buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void CubemarsAK::buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}
// DUTY CYCLE MODE - #0
void CubemarsAK::comm_can_set_duty(uint8_t controller_id, float duty)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(duty * 100000.0), &send_index);
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_PWM), buffer, send_index);
}

// CURRENT LOOP MODE - #1
// The current value is of int32 type, and the value (-60000, 60000) represents -60-60A.
void CubemarsAK::comm_can_set_current(uint8_t controller_id, float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_CURRENT), buffer, send_index);
}
// CURRENT BRAKE MODE - #2
// The braking current value is of int32 type, and the value (0, 60000) represents 0-60A.
void CubemarsAK::comm_can_set_cb(uint8_t controller_id, float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_CURRENT_BRAKE), buffer, send_index);
}
// VELOCITY MODE - #3
// the speed value is int32 type, and the range (-100000, 100000) represents (-100000, 100000) electrical speed.
void CubemarsAK::comm_can_set_rpm(uint8_t controller_id, float rpm)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)rpm, &send_index);
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_VELOCITY), buffer, send_index);
}
// POSITION LOOP MODE - #4
// Position as int32 type，range (-360000000, 360000000) represents position (-36000°,36000°)
void CubemarsAK::comm_can_set_pos(uint8_t controller_id, float pos)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(pos * 10000.0), &send_index);
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_POSITION), buffer, send_index);
}
// SET ORIGIN MODE - #5
// The setting command is uint8_t type, 0 means setting the temporary origin (power failure elimination)
// 1 means setting the permanent zero point (automatic parameter saving)
// 2 means restoring the default zero point (automatic parameter saving)
void CubemarsAK::comm_can_set_origin(uint8_t controller_id, uint8_t set_origin_mode)
{
    int32_t send_index = 0;
    uint8_t buffer[1]; // Change buffer size to hold only one byte
    buffer[send_index++] = set_origin_mode; // Set the origin mode in the buffer
    
    // Transmit the CAN message with the origin setting
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_ORIGIN), buffer, send_index);
}

void CubemarsAK::comm_can_set_pos_spd(uint8_t controller_id, float pos,int16_t spd, int16_t RPA)
{
    int32_t send_index = 0;
    int16_t send_index1 = 4;
    uint8_t buffer[8];
    buffer_append_int32(buffer, (int32_t)(pos * 10000.0), &send_index);
    buffer_append_int16(buffer, spd, &send_index1);
    buffer_append_int16(buffer, RPA, &send_index1);
    comm_can_transmit_eid(canId(controller_id, AKMode::AK_POSITION_VELOCITY), buffer, send_index1);
}

void CubemarsAK::unpackServo(){
    if (mcp2515.readMessage(&canMsg2) == MCP2515::ERROR_OK) {
        position = (canMsg2.data[0] << 8) | canMsg2.data[1];
        position *= 0.1;
        speed = (canMsg2.data[2] << 8) | canMsg2.data[3];
        speed *= 10;
        current = (canMsg2.data[4] << 8) | canMsg2.data[5];
        current *= 0.01;
        motorTemp = canMsg2.data[6];
        errorCode = canMsg2.data[7];
    } 
}

float CubemarsAK::getPosition() const { return position; }

float CubemarsAK::getSpeed() const { return speed; }

float CubemarsAK::getCurrent() const { return current; }

int8_t CubemarsAK::getMotorTemp() const { return motorTemp; }

uint8_t CubemarsAK::getErrorCode() const { return errorCode; }


