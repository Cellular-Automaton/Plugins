///
/// MultiChannel.hh
///

#pragma once

#include <iomanip>
#include <complex>
#include "PLC.hh"

namespace PLC
{
    class MultiChannel
    {
        public:
            MultiChannel();
            MultiChannel(std::vector<std::vector<std::vector<double>>> tab_init);
            MultiChannel(std::vector<std::vector<std::vector<double>>> tab_init,
                        int R,
                        double kernels_mu,
                        double kernels_sigma,
                        std::vector<std::vector<double>> bs_init,
                        std::vector<double> ms_init,
                        std::vector<double> ss_init,
                        std::vector<double> hs_init,
                        std::vector<double> rs_init,
                        std::vector<std::size_t> sources_init,
                        std::vector<std::size_t> destinations_init,
                        double time);
            MultiChannel(std::vector<std::vector<std::vector<double>>> tab_init,
                        std::vector<std::vector<std::vector<double>>> kernels_init,
                        std::vector<double> ms_init,
                        std::vector<double> ss_init,
                        std::vector<double> hs_init,
                        std::vector<std::size_t> sources_init,
                        std::vector<std::size_t> destinations_init,
                        double time_init);
            ~MultiChannel() = default;

            std::vector<std::vector<std::vector<double>>> getTab();
            std::vector<std::vector<std::vector<double>>> getKernels();
            std::vector<double> getMus();
            std::vector<double> getSigmas();
            std::vector<double> getStrengths();
            std::vector<std::size_t> getSources();
            std::vector<std::size_t> getDestinations();
            double getTime();
            void setTime(double new_time);

            std::vector<MultiChannel> activate(size_t nb_loop);
            void run();
            void show();
            std::vector<std::vector<std::vector<double>>> calculate();

            std::vector<std::vector<std::vector<double>>> createKernels(int R, std::vector<std::vector<double>> bs, std::vector<double> rs, double mu, double sigma);
            std::vector<std::vector<std::vector<double>>> growth(std::vector<std::vector<std::vector<double>>> u);

        private:
            std::vector<std::vector<std::vector<double>>> tab;
            std::vector<std::vector<std::vector<double>>> kernels;
            std::vector<double> ms;
            std::vector<double> ss;
            std::vector<double> hs;
            std::vector<std::size_t> sources;
            std::vector<std::size_t> destinations;
            double time;
    };
}; // namespace PLC
