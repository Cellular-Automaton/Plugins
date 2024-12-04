///
/// PLC.hh creat a cellular plc object
///

#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <memory>
#include <vector>
#include <cmath>
#include <fstream>
// #include <nlohmann/json.hpp>

namespace PLC
{
    class PLC
    {
    public:
        PLC() = default;
        ~PLC() = default;
    };

    inline double gauss(double x, double mu, double sigma)
    {
        return std::exp(-0.5 * std::pow((x - mu) / sigma, 2));
    }
}; // namespace PLC
