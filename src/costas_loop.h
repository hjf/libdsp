#pragma once
#include <complex>
#include <vector>

/* Based on the Costas Loop from GNU Radio (https://github.com/gnuradio/gnuradio) */

namespace libdsp
{
    class CostasLoop
    {
    private:
        float d_error;
        float d_noise;
        bool d_use_snr;
        int d_order;
        float phase_detector_8(std::complex<float> sample) const // for 8PSK
        {
            const float K = (sqrtf(2.0) - 1);
            if (fabsf(sample.real()) >= fabsf(sample.imag()))
            {
                return ((sample.real() > 0.0f ? 1.0f : -1.0f) * sample.imag() -
                        (sample.imag() > 0.0f ? 1.0f : -1.0f) * sample.real() * K);
            }
            else
            {
                return ((sample.real() > 0.0f ? 1.0f : -1.0f) * sample.imag() * K -
                        (sample.imag() > 0.0f ? 1.0f : -1.0f) * sample.real());
            }
        };

        float phase_detector_4(std::complex<float> sample) const // for QPSK
        {
            return ((sample.real() > 0.0f ? 1.0f : -1.0f) * sample.imag() -
                    (sample.imag() > 0.0f ? 1.0f : -1.0f) * sample.real());
        };

        float phase_detector_2(std::complex<float> sample) const // for BPSK
        {
            return (sample.real() * sample.imag());
        }

        float d_phase, d_freq;
        float d_max_freq, d_min_freq;
        float d_damping, d_loop_bw;
        float d_alpha, d_beta;

        void advance_loop(float error)
        {
            d_freq = d_freq + d_beta * error;
            d_phase = d_phase + d_freq + d_alpha * error;
        }

        void phase_wrap()
        {
            while (d_phase > (2 * M_PI))
                d_phase -= 2 * M_PI;
            while (d_phase < (-2 * M_PI))
                d_phase += 2 * M_PI;
        }

        void frequency_limit()
        {
            if (d_freq > d_max_freq)
                d_freq = d_max_freq;
            else if (d_freq < d_min_freq)
                d_freq = d_min_freq;
        }

        float tanh_lut_table[256] = {
            -0.96402758, -0.96290241, -0.96174273, -0.96054753, -0.95931576, -0.95804636,
            -0.95673822, -0.95539023, -0.95400122, -0.95257001, -0.95109539, -0.9495761,
            -0.94801087, -0.94639839, -0.94473732, -0.94302627, -0.94126385, -0.93944862,
            -0.93757908, -0.93565374, -0.93367104, -0.93162941, -0.92952723, -0.92736284,
            -0.92513456, -0.92284066, -0.92047938, -0.91804891, -0.91554743, -0.91297305,
            -0.91032388, -0.90759795, -0.9047933, -0.90190789, -0.89893968, -0.89588656,
            -0.89274642, -0.88951709, -0.88619637, -0.88278203, -0.87927182, -0.87566342,
            -0.87195453, -0.86814278, -0.86422579, -0.86020115, -0.85606642, -0.85181914,
            -0.84745683, -0.84297699, -0.83837709, -0.83365461, -0.82880699, -0.82383167,
            -0.81872609, -0.81348767, -0.80811385, -0.80260204, -0.7969497, -0.79115425,
            -0.78521317, -0.77912392, -0.772884, -0.76649093, -0.75994227, -0.75323562,
            -0.74636859, -0.73933889, -0.73214422, -0.7247824, -0.71725127, -0.70954876,
            -0.70167287, -0.6936217, -0.68539341, -0.67698629, -0.66839871, -0.65962916,
            -0.65067625, -0.64153871, -0.6322154, -0.62270534, -0.61300768, -0.60312171,
            -0.59304692, -0.58278295, -0.57232959, -0.56168685, -0.55085493, -0.53983419,
            -0.52862523, -0.51722883, -0.50564601, -0.49387799, -0.48192623, -0.46979241,
            -0.45747844, -0.44498647, -0.4323189, -0.41947836, -0.40646773, -0.39329014,
            -0.37994896, -0.36644782, -0.35279057, -0.33898135, -0.32502449, -0.31092459,
            -0.2966865, -0.28231527, -0.26781621, -0.25319481, -0.23845682, -0.22360817,
            -0.208655, -0.19360362, -0.17846056, -0.16323249, -0.14792623, -0.13254879,
            -0.11710727, -0.10160892, -0.08606109, -0.07047123, -0.05484686, -0.0391956,
            -0.02352507, -0.00784298, 0.00784298, 0.02352507, 0.0391956, 0.05484686,
            0.07047123, 0.08606109, 0.10160892, 0.11710727, 0.13254879, 0.14792623,
            0.16323249, 0.17846056, 0.19360362, 0.208655, 0.22360817, 0.23845682,
            0.25319481, 0.26781621, 0.28231527, 0.2966865, 0.31092459, 0.32502449,
            0.33898135, 0.35279057, 0.36644782, 0.37994896, 0.39329014, 0.40646773,
            0.41947836, 0.4323189, 0.44498647, 0.45747844, 0.46979241, 0.48192623,
            0.49387799, 0.50564601, 0.51722883, 0.52862523, 0.53983419, 0.55085493,
            0.56168685, 0.57232959, 0.58278295, 0.59304692, 0.60312171, 0.61300768,
            0.62270534, 0.6322154, 0.64153871, 0.65067625, 0.65962916, 0.66839871,
            0.67698629, 0.68539341, 0.6936217, 0.70167287, 0.70954876, 0.71725127,
            0.7247824, 0.73214422, 0.73933889, 0.74636859, 0.75323562, 0.75994227,
            0.76649093, 0.772884, 0.77912392, 0.78521317, 0.79115425, 0.7969497,
            0.80260204, 0.80811385, 0.81348767, 0.81872609, 0.82383167, 0.82880699,
            0.83365461, 0.83837709, 0.84297699, 0.84745683, 0.85181914, 0.85606642,
            0.86020115, 0.86422579, 0.86814278, 0.87195453, 0.87566342, 0.87927182,
            0.88278203, 0.88619637, 0.88951709, 0.89274642, 0.89588656, 0.89893968,
            0.90190789, 0.9047933, 0.90759795, 0.91032388, 0.91297305, 0.91554743,
            0.91804891, 0.92047938, 0.92284066, 0.92513456, 0.92736284, 0.92952723,
            0.93162941, 0.93367104, 0.93565374, 0.93757908, 0.93944862, 0.94126385,
            0.94302627, 0.94473732, 0.94639839, 0.94801087, 0.9495761, 0.95109539,
            0.95257001, 0.95400122, 0.95539023, 0.95673822, 0.95804636, 0.95931576,
            0.96054753, 0.96174273, 0.96290241, 0.96402758};

        float tanhf_lut(float x)
        {
            if (x > 2)
                return 1;
            else if (x <= -2)
                return -1;
            else
            {
                int index = 128 + 64 * x;
                return tanh_lut_table[index];
            }
        }

        void update_gains()
        {
            float denom = (1.0 + 2.0 * d_damping * d_loop_bw + d_loop_bw * d_loop_bw);
            d_alpha = (4 * d_damping * d_loop_bw) / denom;
            d_beta = (4 * d_loop_bw * d_loop_bw) / denom;
        }

        void set_loop_bandwidth(float bw)
        {
            if (bw < 0)
            {
                throw std::out_of_range("control_loop: invalid bandwidth. Must be >= 0.");
            }

            d_loop_bw = bw;
            update_gains();
        }

    public:
        CostasLoop(float loop_bw, unsigned int order);
        size_t work(std::complex<float> *in, size_t length, std::complex<float> *out);
    };
} // namespace libdsp