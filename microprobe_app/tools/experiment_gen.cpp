#include "experiment_gen.h"

ExperimentPreset::ExperimentPreset() {};

void ExperimentPreset::add(float start, float stop, float f_ref_start, float f_ref_stop, PresetMode mode) {
    m_start.push_back(start);
    m_stop.push_back(stop);
    m_f_ref_start.push_back(f_ref_start);
    m_f_ref_stop.push_back(f_ref_stop);
    m_mode.push_back(mode);
    m_max_iter++;
}

void ExperimentPreset::reset() {
    m_start.clear();
    m_stop.clear();
    m_f_ref_start.clear();
    m_f_ref_stop.clear();
    m_mode.clear();

    m_iter = 0;
    m_max_iter = -1;
}

/*
At each time step, we check if we are in the time zone of the current preset.
Force reference is returned according to the mode.
If we move out of the time zone and if there is another preset following, we increment m_iter.
*/
float ExperimentPreset::getCurrentFRef(float time) {
    if (time >= m_start[m_iter]) {
        if (time < m_stop[m_iter]) {
            if (m_mode[m_iter] == step) {
                return m_f_ref_start[m_iter];
            } else {
                return m_f_ref_start[m_iter]+ (m_f_ref_stop[m_iter]-m_f_ref_start[m_iter])*(time-m_start[m_iter])/(m_stop[m_iter]-m_start[m_iter]);
            }
            
        } else {
            if (m_iter < m_max_iter) {
                m_iter++;
                if (m_mode[m_iter] == step) {
                    return m_f_ref_start[m_iter];
                } else {
                    return m_f_ref_start[m_iter]+ (m_f_ref_stop[m_iter]-m_f_ref_start[m_iter])*(time-m_start[m_iter])/(m_stop[m_iter]-m_start[m_iter]);
                }
            }
            return 0;
        }
    } else {
        return 0;
    }
}