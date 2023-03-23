#ifndef TOOLS_EXPERIMENT_GEN_H
#define TOOLS_EXPERIMENT_GEN_H

#include <cstdio>
#include <iostream>
#include <vector>

//uint is not defined in C++. Other files that contain 826api.h have this type definition.
typedef unsigned int uint;

using namespace std;

class ExperimentPreset {
public:
	enum PresetMode {step, ramp};

	ExperimentPreset();

	void add(float start, float stop, float f_ref_start, float f_ref_stop, PresetMode type);

    void reset();

    float getCurrentFRef(float time);

private:

    vector<float> m_start;
    vector<float> m_stop;
    vector<float> m_f_ref_start;
    vector<float> m_f_ref_stop;
    vector<PresetMode> m_mode;

    uint m_iter = 0;
    //Max iter is the highest array index allowed. 1 element = index 0.
    uint m_max_iter = -1;
};

#endif // TOOLS_EXPERIMENT_GEN_H