#pragma once

namespace nxtftl
{
    class motor_dto
    {
    public:
        motor_dto() = default;
        unsigned int port;
        long int tacho_count;
    };
}

