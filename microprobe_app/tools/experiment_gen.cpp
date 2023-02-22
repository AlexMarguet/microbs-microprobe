#include "experiment_gen.h"

ExperimentPreset::ExperimentPreset() {};

void ExperimentPreset::add(uint start, uint stop, float f_ref, PresetMode mode) {
    m_start.push_back(start);
    m_stop.push_back(stop);
    m_f_ref.push_back(f_ref);
    m_mode.push_back(mode);
}

void ExperimentPreset::reset() {
    m_start.clear();
    m_stop.clear();
    m_f_ref.clear();
    m_mode.clear();

    m_iter = 0;
}

float ExperimentPreset::getCurrentFRef(uint time) {
    if (time >= m_start[m_iter]) {
        if (time < m_stop[m_iter]) {
            return m_f_ref[m_iter];
        } else {
            if (m_iter < m_max_iter) {
                m_iter++;
                return m_f_ref[m_iter];
            }
            return 0;
        }
    }
}