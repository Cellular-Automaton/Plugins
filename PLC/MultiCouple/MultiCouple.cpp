///
/// MultiCouple.cpp
///

#include "../include/MultiCouple.hh"

static void doubleToRGB(double value, int& R, int& G, int& B)
{
    value = std::max(0.0, std::min(1.0, value));

    if (value < 0.5) {
        R = 0;
        G = static_cast<int>(255 * (value * 2));
        B = 255;
    } else {
        R = static_cast<int>(255 * ((value - 0.5) * 2));
        G = static_cast<int>(255 * (1.0 - (value - 0.5) * 2));
        B = 0;
    }
}

static void printColoredDouble(double value, bool dec = false)
{
    int R, G, B;
    doubleToRGB(value, R, G, B);

    if (dec && value < 0) {
        std::cout << std::fixed << std::setprecision(3) << value;
        return;
    }
    if (value < 0) {
        std::cout << "\033[38;2;" << R << ";" << G << ";" << B << "m"
              << std::fixed << '-' << "\033[0m";
        return;
    }
    if (dec) {
        std::cout << std::fixed << std::setprecision(4) << value;
    } else {
         std::cout << "\033[38;2;" << R << ";" << G << ";" << B << "m"
              << std::fixed << std::setprecision(0) << value
              << "\033[0m";
    }
}

static void printMatrix(std::vector<std::vector<double>> matrix, bool mult = false, bool dec = false)
{
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            printColoredDouble(mult ? matrix[i][j] * 100 : matrix[i][j], dec);
        }
        std::cout << std::endl;
        // for (size_t j = 0; j < matrix[0].size(); ++j) {
        //     std::cout << " ";
        //     printColoredDouble(matrix[i][j] * (mult ? 100 : 1));
        // }
        // std::cout << std::endl;
    }
    std::cout << std::endl;
}

static void printMatrixComp(std::vector<std::vector<std::complex<double>>> matrix, bool mult = false, bool dec = false)
{
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            printColoredDouble(mult ? matrix[i][j].real() * 100 : matrix[i][j].real(), dec);
            std::cout << " ";
        }
        std::cout << std::endl;
        // for (size_t j = 0; j < matrix[0].size(); ++j) {
        //     std::cout << " ";
        //     printColoredDouble(matrix[i][j] * (mult ? 100 : 1));
        // }
        // std::cout << std::endl;
    }
    std::cout << std::endl;
}

static std::vector<std::vector<double>> shift(std::vector<std::vector<double>> data)
{
    int N = data.size();
    int M = data[0].size();
    double sum = 0.0;
    std::vector<std::vector<double>> new_data(N, std::vector<double>(M));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            sum += data[i][j];
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            data[i][j] /= sum;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int new_i = (i + N / 2) % N;
            int new_j = (j + M / 2) % M;
            new_data[new_i][new_j] = data[i][j];
        }
    }

    return new_data;
}

static std::vector<std::vector<std::complex<double>>> double_to_complex(std::vector<std::vector<double>> tab)
{
    std::vector<std::vector<std::complex<double>>> result(tab.size(), std::vector<std::complex<double>>(tab[0].size()));

    for (size_t i = 0; i < tab.size(); ++i) {
        for (size_t j = 0; j < tab[i].size(); ++j) {
            result[i][j] = std::complex<double>(tab[i][j], 0.0);
        }
    }
    return result;
}

static std::vector<std::vector<double>> complex_to_double(std::vector<std::vector<std::complex<double>>> tab)
{
    std::vector<std::vector<double>> result(tab.size(), std::vector<double>(tab[0].size()));

    for (size_t i = 0; i < tab.size(); ++i) {
        for (size_t j = 0; j < tab[i].size(); ++j) {
            result[i][j] = tab[i][j].real();
        }
    }
    return result;
}

static std::vector<std::vector<std::complex<double>>> fft2(std::vector<std::vector<std::complex<double>>> tab, bool inverse = false)
{
    int size_y = tab.size();
    int size_x = tab[0].size();
    double PI = 3.14159265358979323846;
    std::vector<std::vector<std::complex<double>>> result(size_y, std::vector<std::complex<double>>(size_x));
    double sign = inverse ? 1.0 : -1.0;
    double norm = inverse ? (1.0 / (size_x * size_y)) : 1.0;

    for (int u = 0; u < size_y; ++u) {
        for (int v = 0; v < size_x; ++v) {
            std::complex<double> sum(0, 0);

            for (int y = 0; y < size_y; ++y) {
                for (int x = 0; x < size_x; ++x) {
                    double angle = 2.0 * PI * ((u * y / double(size_y)) + (v * x / double(size_x)));
                    std::complex<double> expTerm = std::polar(1.0, sign * angle);
                    sum += tab[y][x] * expTerm;
                }
            }
            result[u][v] = sum * norm;
        }
    }
    return result;
}

std::vector<std::vector<std::vector<std::complex<double>>>> PLC::MultiCouple::createKernels(int R, std::vector<std::vector<double>> bs, double mu, double sigma)
{
    int size_y = this->tab.size();
    int size_x = this->tab[0].size();
    std::vector<std::vector<std::vector<std::complex<double>>>> result(bs.size(), std::vector<std::vector<std::complex<double>>>(size_y, std::vector<std::complex<double>>(size_x)));

    for (int idx = 0; idx < bs.size(); idx++) {
        std::vector<std::vector<double>> K(size_y, std::vector<double>(size_x));
        std::vector<std::vector<std::complex<double>>> nKernels(size_y, std::vector<std::complex<double>>(size_x));

        for (int i = 0; i < size_y; i++) {
            for (int j = 0; j < size_x; j++) {
                double distance = std::sqrt(std::pow(((size_x / 2) - size_x + j), 2) + std::pow(((size_y / 2) - size_y + i), 2)) / R * bs[idx].size();
                for (int r = 0; r < bs[idx].size(); r++) {
                    bool mask = int(distance) == r;
                    K[i][j] += \
                        gauss(fmod(distance, 1), mu, sigma) \
                        * mask \
                        * bs[idx][r];
                }
            }
        }
        K = shift(K);
        nKernels = fft2(double_to_complex(K));
        result[idx] = nKernels;
    }
    return result;
}

std::vector<std::vector<std::vector<double>>> PLC::MultiCouple::growthMultiCouple(const std::vector<std::vector<std::vector<double>>> us)
{
    std::vector<std::vector<std::vector<double>>> result;

    for (size_t idx = 0; idx < us.size(); ++idx) {
        size_t r_size = us[idx].size();
        size_t c_size = us[idx][0].size();
        std::vector<std::vector<double>> tmp(r_size, std::vector<double>(c_size, 0.0));

        for (size_t i = 0; i < r_size; ++i) {
            for (size_t j = 0; j < c_size; ++j) {
                tmp[i][j] = 2.0 * gauss(us[idx][i][j], this->ms[idx], this->ss[idx]) - 1.0;
            }
        }
        result.push_back(tmp);
    }
    return result;
}

PLC::MultiCouple::MultiCouple()
{
    this->tab = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.02,0.03,0.04,0.04,0.04,0.03,0.02,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0.1,0.16,0.2,0.23,0.25,0.24,0.21,0.18,0.14,0.1,0.07,0.03,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.09,0.2,0.33,0.44,0.52,0.56,0.58,0.55,0.51,0.44,0.37,0.3,0.23,0.16,0.08,0.01,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.13,0.29,0.45,0.6,0.75,0.85,0.9,0.91,0.88,0.82,0.74,0.64,0.55,0.46,0.36,0.25,0.12,0.03,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.14,0.38,0.6,0.78,0.93,1.0,1.0,1.0,1.0,1.0,1.0,0.99,0.89,0.78,0.67,0.56,0.44,0.3,0.15,0.04,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.08,0.39,0.74,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.98,0.85,0.74,0.62,0.47,0.3,0.14,0.03,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.32,0.76,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.88,0.75,0.61,0.45,0.27,0.11,0.01,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.35,0.83,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.88,0.73,0.57,0.38,0.19,0.05,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.99,1.0,1.0,1.0,1.0,0.99,1.0,1.0,1.0,1.0,1.0,1.0,0.85,0.67,0.47,0.27,0.11,0.01,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.55,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.93,0.83,0.79,0.84,0.88,0.89,0.9,0.93,0.98,1.0,1.0,1.0,1.0,0.98,0.79,0.57,0.34,0.15,0.03,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.47,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.9,0.72,0.54,0.44,0.48,0.6,0.7,0.76,0.82,0.91,0.99,1.0,1.0,1.0,1.0,0.91,0.67,0.41,0.19,0.05,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.27,0.99,1.0,1.0,1.0,1.0,0.9,0.71,0.65,0.55,0.38,0.2,0.14,0.21,0.36,0.52,0.64,0.73,0.84,0.95,1.0,1.0,1.0,1.0,1.0,0.78,0.49,0.24,0.07,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.14,0.63,0.96,1.0,1.0,1.0,0.84,0.17,0,0,0,0,0,0,0,0.13,0.35,0.51,0.64,0.77,0.91,0.99,1.0,1.0,1.0,1.0,0.88,0.58,0.29,0.09,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.07,0.38,0.72,0.95,1.0,1.0,1.0,0.22,0,0,0,0,0,0,0,0,0,0.11,0.33,0.5,0.67,0.86,0.99,1.0,1.0,1.0,1.0,0.95,0.64,0.33,0.1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.32,0.49,0.71,0.93,1.0,1.0,1.0,0.56,0,0,0,0,0,0,0,0,0,0,0,0.1,0.31,0.52,0.79,0.98,1.0,1.0,1.0,1.0,0.98,0.67,0.35,0.11,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.6,0.83,0.98,1.0,1.0,0.68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.15,0.38,0.71,0.97,1.0,1.0,1.0,1.0,0.97,0.67,0.35,0.11,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.51,0.96,1.0,1.0,0.18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.09,0.34,0.68,0.95,1.0,1.0,1.0,1.0,0.91,0.61,0.32,0.1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.13,0.56,0.99,1.0,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.17,0.45,0.76,0.96,1.0,1.0,1.0,1.0,0.82,0.52,0.26,0.07,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.33,0.7,0.94,1.0,1.0,0.44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.33,0.68,0.91,0.99,1.0,1.0,1.0,1.0,0.71,0.42,0.19,0.03,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.53,0.89,1.0,1.0,1.0,0.8,0.43,0.04,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.47,0.86,1.0,1.0,1.0,1.0,1.0,0.95,0.58,0.32,0.12,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.77,0.99,1.0,0.97,0.58,0.41,0.33,0.18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.54,0.95,1.0,1.0,1.0,1.0,1.0,0.8,0.44,0.21,0.06,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.39,0.83,1.0,1.0,0.55,0.11,0.05,0.15,0.22,0.06,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.58,0.99,1.0,1.0,1.0,1.0,1.0,0.59,0.29,0.11,0.01,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0.55,0.81,0.86,0.97,1.0,1.0,0.5,0,0,0.01,0.09,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0,0.26,0.78,1.0,1.0,1.0,1.0,1.0,0.66,0.35,0.13,0.03,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.33,1.0,1.0,1.0,1.0,1.0,1.0,0.93,0.11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.23,0.73,0.95,1.0,1.0,1.0,1.0,1.0,0.62,0.35,0.12,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.51,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.56,0.25,0.09,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.12,0.38,1.0,1.0,1.0,0.66,0.08,0.55,1.0,1.0,1.0,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0,0.35,1.0,1.0,1.0,1.0,1.0,1.0,0.67,0.12,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0.6,1.0,1.0,1.0,1.0,1.0,1.0,0.49,0,0,0.87,1.0,0.88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1.0,1.0,1.0,1.0,1.0,1.0,0.7,0.07,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0.04,0.21,0.48,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0,0,0.04,0.42,0.26,0,0,0,0,0,0,0,0,0,0.12,0.21,0.34,0.58,1.0,1.0,1.0,0.99,0.97,0.99,0.46,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0.5,1.0,1.0,1.0,1.0,0.96,0,0.31,1.0,1.0,1.0,0.53,0,0,0,0,0,0,0,0,0.2,0.21,0,0,0,0.27,1.0,1.0,1.0,1.0,1.0,1.0,0.87,0.52,0.01,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0.84,1.0,1.0,1.0,1.0,1.0,0,0,0,0.83,1.0,1.0,0.52,0,0,0,0,0,0,0,0.26,0.82,0.59,0.02,0,0,0.46,1.0,1.0,1.0,1.0,1.0,0.9,0.55,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0.39,0.99,1.0,1.0,1.0,1.0,0.78,0.04,0,0,0,0.93,0.92,0,0,0,0,0,0,0,0,0.69,1.0,1.0,0.36,0,0,1.0,1.0,0.65,0.66,0.97,0.87,0.54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0.55,0.75,0.59,0.74,1.0,1.0,0,0,0.75,0.71,0.18,0,0,0,0,0,0,0,0,0,0,0.29,0,0,0.45,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.47,0.39,0.71,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0.69,0.81,0.8,0.92,1.0,0.13,0,0,0.13,0.94,0.58,0,0,0,0,0,0,0,0,0,1.0,1.0,0.34,0,0.04,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0.63,0.85,0.9,0.98,1.0,0.09,0,0,0.02,1.0,0.64,0,0,0,0,0,0,0,0,0.59,1.0,1.0,0.84,0,0,1.0,1.0,1.0,1.0,1.0,1.0,0.64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0.64,0.65,0.67,1.0,1.0,0.21,0.01,0,0.04,0.02,0,0,0,0,0,0,0,0,0,0.69,1.0,1.0,1.0,0.29,0.37,1.0,1.0,0.6,0.63,1.0,0.84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0.44,0.73,0.73,0.85,1.0,0.97,0.23,0.05,0,0,0,0,0,0,0,0,0.06,0,0,0,0.97,1.0,1.0,1.0,1.0,1.0,1.0,0.33,0.24,0.67,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0.12,0.55,0.9,0.9,1.0,1.0,1.0,0.43,0.04,0,0,0,0,0,0,0,0.31,0.54,0,0,0,0.88,1.0,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0.29,0.71,1.0,1.0,1.0,1.0,0.79,0.28,0,0,0,0,0,0,0,0,0.4,0.77,0.54,0,0,0.87,1.0,1.0,1.0,1.0,1.0,0.31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0.16,0.27,0.41,0.72,0.99,1.0,1.0,0.82,0.42,0.09,0,0,0,0,0,0,0,0,0.1,0.55,0.58,0.58,0.77,0.99,1.0,1.0,1.0,1.0,0.63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0.31,0.48,0.45,0.46,0.63,0.88,1.0,0.83,0.59,0.28,0.06,0,0,0,0,0,0,0,0,0,0.32,0.7,0.95,1.0,1.0,1.0,1.0,0.7,0.58,0.12,0.04,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0.23,0.54,0.53,0.48,0.57,0.59,0.65,0.63,0.55,0.35,0.13,0.03,0.02,0.09,0.74,1.0,0.09,0,0,0,0.32,0.86,1.0,1.0,1.0,1.0,0.57,0.44,0.31,0.16,0.01,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0.31,0.45,0.31,0.18,0.28,0.39,0.47,0.54,0.5,0.35,0.2,0.16,0.28,0.75,1.0,0.42,0.01,0,0,0.6,1.0,1.0,1.0,1.0,0.51,0.29,0.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0.14,0.3,0.4,0.54,0.71,0.74,0.65,0.49,0.35,0.27,0.47,0.6,0.6,0.72,0.98,1.0,1.0,1.0,1.0,0.65,0.33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0.06,0.33,0.53,0.69,0.94,0.99,1.0,0.84,0.41,0.16,0.15,0.96,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.73,0.13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0.42,0.86,0.98,0.98,0.99,1.0,0.94,0.63,0.32,0.62,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.65,0.23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0.07,0.62,0.95,1.0,1.0,0.99,0.98,0.99,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.98,0.14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0.03,0.46,0.89,1.0,1.0,0.97,0.83,0.75,0.81,0.94,1.0,1.0,1.0,1.0,0.99,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0.14,0.57,0.88,0.93,0.81,0.58,0.45,0.48,0.64,0.86,0.97,0.99,0.99,0.42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0.23,0.45,0.47,0.39,0.29,0.19,0.2,0.46,0.28,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0.08,0.22,0.24,0.15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0.07,0.22,0.14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    std::vector<std::vector<double>> bs = {{0.5, 1, 0.667}};
    this->kernels = this->createKernels(18, bs, 0.5, 0.15);
    this->ms = {0.26};
    this->ss = {0.036};
    this->time = 0.1;
}

PLC::MultiCouple::MultiCouple(std::vector<std::vector<double>> tab_init)
{
    this->tab = std::move(tab_init);
    std::vector<std::vector<double>> bs = {{1, 0.4166666666666667, 0.6666666666666666},{0.08333333333333333, 1},{1}};
    this->kernels = this->createKernels(10, bs, 0.5, 0.15);
    this->ms = {0.156,0.193,0.342};
    this->ss = {0.0118,0.049,0.0891};
    this->time = 0.2;
}

PLC::MultiCouple::MultiCouple(std::vector<std::vector<double>> tab_init,
                            int R,
                            double kernels_mu,
                            double kernels_sigma,
                            std::vector<std::vector<double>> bs_init,
                            std::vector<double> ms_init,
                            std::vector<double> ss_init,
                            double time_init)
{
    if (bs_init.size() == 0 || bs_init.size() != ms_init.size() || bs_init.size() != ss_init.size()) {
        bs_init = {{0.5, 1, 0.667}};
        ms_init = {0.26};
        ss_init = {0.036};
        time = 0.1;
    }
    this->tab = std::move(tab_init);
    this->kernels = this->createKernels(R, bs_init, kernels_mu, kernels_sigma);
    this->ms = ms_init;
    this->ss = ss_init;
    this->time = time_init;
}

PLC::MultiCouple::MultiCouple(std::vector<std::vector<double>> tab_init,
                            std::vector<std::vector<std::vector<std::complex<double>>>> kernels_init,
                            std::vector<double> ms_init,
                            std::vector<double> ss_init,
                            double time_init)
{
    this->tab = std::move(tab_init);
    this->kernels = std::move(kernels_init);
    this->ms = ms_init;
    this->ss = ss_init;
    this->time = time_init;
}

std::vector<PLC::MultiCouple> PLC::MultiCouple::activate(size_t nb_loop)
{
    std::vector<MultiCouple> result;

    for (size_t i = 0; i < nb_loop; ++i) {
        MultiCouple tmp(this->tab, this->kernels, this->ms, this->ss, this->time);
        result.push_back(tmp);
        this->run();
    }
    return result;
}

void PLC::MultiCouple::run()
{
    std::vector<std::vector<double>> new_tab = this->tab;
    std::vector<std::vector<std::vector<double>>> us = this->calculate();
    std::vector<std::vector<std::vector<double>>> gs = this->growthMultiCouple(us);

    for (size_t idx = 0; idx < gs.size(); ++idx) {
        for (size_t i = 0; i < new_tab.size(); ++i) {
            for (size_t j = 0; j < new_tab[j].size(); ++j) {
                new_tab[i][j] += this->time * (gs[idx][i][j] / double(gs.size()));
                if (new_tab[i][j] < 0) {
                    new_tab[i][j] = 0;
                }
                if(new_tab[i][j] > 1) {
                    new_tab[i][j] = 1;
                }
            }
        }
    }
    this->tab = new_tab;
}

void PLC::MultiCouple::show()
{
    printMatrix(this->tab);
}

std::vector<std::vector<std::vector<double>>> PLC::MultiCouple::calculate()
{
    int rows = this->tab.size();
    int cols = this->tab[0].size();
    std::vector<std::vector<double>> U(rows, std::vector<double>(cols, 0.0));
    std::vector<std::vector<std::complex<double>>> X = fft2(double_to_complex(this->tab));
    std::vector<std::vector<std::complex<double>>> mul(rows, std::vector<std::complex<double>>(cols));
    std::vector<std::vector<std::vector<double>>> result;


    for (size_t idx = 0; idx < this->kernels.size(); ++idx) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                mul[i][j] = X[i][j] * this->kernels[idx][i][j];
            }
        }

        std::vector<std::vector<std::complex<double>>> tmp = fft2(mul, true);

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                U[i][j] = tmp[i][j].real();
            }
        }
        result.push_back(U);
    }
    return result;
}

void PLC::MultiCouple::setTime(double new_time)
{
    this->time = new_time;
}

std::vector<std::vector<double>> PLC::MultiCouple::getTab()
{
    return this->tab;
}

std::vector<std::vector<std::vector<std::complex<double>>>> PLC::MultiCouple::getKernels()
{
    return this->kernels;
}

std::vector<double> PLC::MultiCouple::getMus()
{
    return this->ms;
}

std::vector<double> PLC::MultiCouple::getSigmas()
{
    return this->ss;
}

double PLC::MultiCouple::getTime()
{
    return this->time;
}
