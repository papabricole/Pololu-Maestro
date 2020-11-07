#pragma once

namespace Maestro {
enum class Opcode {
    QUIT = 0,
    LITERAL,
    LITERAL8,
    LITERAL_N,
    LITERAL8_N,
    RETURN,
    JUMP,
    JUMP_Z,
    DELAY,
    GET_MS,
    DEPTH,
    DROP,
    DUP,
    OVER,
    PICK,
    SWAP,
    ROT,
    ROLL,
    BITWISE_NOT,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    SHIFT_RIGHT,
    SHIFT_LEFT,
    LOGICAL_NOT,
    LOGICAL_AND,
    LOGICAL_OR,
    NEGATE,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    MOD,
    POSITIVE,
    NEGATIVE,
    NONZERO,
    EQUALS,
    NOT_EQUALS,
    MIN,
    MAX,
    LESS_THAN,
    GREATER_THAN,
    SERVO,
    SERVO_8BIT,
    SPEED,
    ACCELERATION,
    GET_POSITION,
    GET_MOVING_STATE,
    LED_ON,
    LED_OFF,
    PWM,
    PEEK,
    POKE,
    SERIAL_SEND_BYTE,
    CALL
};
}  // namespace Maestro