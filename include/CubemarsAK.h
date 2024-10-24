#ifndef CUBEMARSAK_H
#define CUBEMARSAK_H

#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

#define P_MIN -32000
#define P_MAX 32000
#define V_MIN -32000
#define V_MAX 32000
#define CUR_MIN -1500
#define CUR_MAX 1500
#define T_MIN -30
#define T_MAX 30
#define Kt_TMotor 0.091
#define Kt_actual 0.105
#define Current_Factor 0.59
#define GEAR_RATIO 9.0
#define NUM_POLE_PAIRS 21


// Define the AKMode enum
enum AKMode {
    AK_PWM = 0,
    AK_CURRENT = 1,
    AK_CURRENT_BRAKE = 2,
    AK_VELOCITY = 3,
    AK_POSITION = 4,
    AK_ORIGIN = 5,
    AK_POSITION_VELOCITY = 6,
};


class CubemarsAK {
public:
    CubemarsAK(uint8_t csPin);
    ~CubemarsAK();
    
    void initializeCAN();

    uint32_t canId(int id, AKMode Mode_set);

    unsigned int float_to_uint(float x, float x_min, float x_max, float bits);
    float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits);
    void pack_cmd();

    void comm_can_set_duty(uint8_t controller_id, float duty);
    void comm_can_set_current(uint8_t controller_id, float current);
    void comm_can_set_cb(uint8_t controller_id, float current);
    void comm_can_set_rpm(uint8_t controller_id, float rpm);
    void comm_can_set_pos(uint8_t controller_id, float pos);
    void comm_can_set_origin(uint8_t controller_id, uint8_t set_origin_mode);
    void comm_can_set_pos_spd(uint8_t controller_id, float pos, int16_t spd, int16_t RPA);

    void comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len);
    void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);
    void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index);

    void unpackServo();
    float getPosition() const;
    float getSpeed() const;
    float getCurrent() const;
    int8_t getMotorTemp() const;
    uint8_t getErrorCode() const;


    struct can_frame canMsg2;
    MCP2515 mcp2515;

private:
    uint32_t controller_id;
    float position;
    float speed;
    float current;
    int8_t motorTemp;
    uint8_t errorCode;
};

#endif // CUBEMARSAK_H
