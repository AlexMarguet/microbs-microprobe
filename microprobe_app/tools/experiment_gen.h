#ifndef TOOLS_EXPERIMENT_GEN_H
#define TOOLS_EXPERIMENT_GEN_H

using namespace std;

class ExperimentPreset {
public:
	enum PresetMode {step, ramp};

	ExperimentPreset();

	void add(uint start, uint stop, float f_ref, PresetMode type);

    void reset();

    float getCurrentFRef(uint time);

private:

    vector<uint> m_start;
    vector<uint> m_stop;
    vector<float> m_f_ref;
    vector<PresetMode> m_mode;

    uint m_iter = 0;
    uint m_max_iter = 0;
};

#endif // TOOLS_EXPERIMENT_GEN_H