///
/// Lenia.hh
///

#pragma once

#include "PLC.hh"

namespace PLC
{
    class Lenia
    {
        public:
            Lenia();
            Lenia(std::vector<std::vector<double> > tab_init);
            Lenia(std::vector<std::vector<double> > tab_init, int R, double kernel_mu, double kernel_sigma, double growth_mu, double growth_sigma);
            Lenia(std::vector<std::vector<double> > tab_init, std::vector<std::vector<double> > kernel_init, double time, double growth_mu, double growth_sigma);
            ~Lenia() = default;

            std::vector<std::vector<double> > getTab();
            std::vector<std::vector<double> > getKernel();
            double getMu();
            double getSigma();
            double getTime();
            void setTime(double new_time);

            std::vector<Lenia> activate(size_t nb_loop);
            void run();
            void show();
            std::vector<std::vector<double> > calculate();

            std::vector<std::vector<double> > createKernel(int R, double mu, double sigma);
            std::vector<std::vector<double> > growthLenia(const std::vector<std::vector<double> > u);

        private:
            std::vector<std::vector<double> > tab;
            std::vector<std::vector<double> > kernel;
            double mu;
            double sigma;
            double time;
    };
}; // namespace PLC
