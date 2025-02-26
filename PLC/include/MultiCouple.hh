///
/// MultiCouple.hh
///

#pragma once

#include <iomanip>
#include <complex>
#include "PLC.hh"

namespace PLC
{
    class MultiCouple
    {
        public:
            MultiCouple();
            MultiCouple(std::vector<std::vector<double>> tab_init);
            MultiCouple(std::vector<std::vector<double>> tab_init,
                        int R,
                        double kernels_mu,
                        double kernels_sigma,
                        std::vector<std::vector<double>> bs_init,
                        std::vector<double> ms_init,
                        std::vector<double> ss_init,
                        double time);
            MultiCouple(std::vector<std::vector<double>> tab_init,
                        std::vector<std::vector<std::vector<std::complex<double>>>> kernels_init,
                        std::vector<double> ms_init,
                        std::vector<double> ss_init,
                        double time_init);
            ~MultiCouple() = default;

            std::vector<std::vector<double>> getTab();
            std::vector<std::vector<std::vector<std::complex<double>>>> getKernels();
            std::vector<double> getMus();
            std::vector<double> getSigmas();
            double getTime();
            void setTime(double new_time);

            std::vector<MultiCouple> activate(size_t nb_loop);
            void run();
            void show();
            std::vector<std::vector<std::vector<double>>> calculate();

            std::vector<std::vector<std::vector<std::complex<double>>>> createKernels(int R, std::vector<std::vector<double>> bs, double mu, double sigma);
            std::vector<std::vector<std::vector<double>>> growth(std::vector<std::vector<std::vector<double>>> u);

        private:
            std::vector<std::vector<double>> tab;
            std::vector<std::vector<std::vector<std::complex<double>>>> kernels;
            std::vector<double> ms;
            std::vector<double> ss;
            double time;
    };
}; // namespace PLC
