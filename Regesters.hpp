#pragma once

struct uart_device_t {
    __int16 accumulator;
    __int16 index;
};

static volatile uart_device_t* const uart_device =
    reinterpret_cast<volatile uart_device_t*>(0xfffffffe00000000);