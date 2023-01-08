#include "controller.h"

using namespace std;

const float PID::kp = 0.5;
const float PID::ki = 0.1;
const float PID::kd = 0.;

PID::PID() {}

auto now() { return std::chrono::steady_clock::now(); }

// auto awake_time() {
//     using std::chrono::operator""ms;
//     return now() + 2000ms;
// }

void test() {
    using namespace std::chrono_literals;

    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); // REALTIME_PRIORITY_CLASS but care for CPU draining

    for (int i = 0 ; i<5 ; i++) {
        const auto start {now()};
        auto delay = now() + 2000ms;
        std::cout << "Hello, waiter...\n" << std::flush;
        // std::this_thread::sleep_for(300ms);
        
        std::this_thread::sleep_until(delay);
        std::chrono::duration<double, std::milli> elapsed {now() - start};
        std::cout << "Waited " << elapsed.count() << " ms\n";
    }
}

void PID::control_loop() {
    using namespace std::chrono_literals;

    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); // REALTIME_PRIORITY_CLASS but care for CPU draining
    
    float out;

    for (int i = 0 ; i<5 ; i++) {
        const auto start {now()};
        auto delay = now() + 2000ms;

        m_error = m_reference - m_val;
        m_d_error = m_error - m_prev_error;

        out = kp * m_error + ki * (m_error + m_prev_error) + kd * m_d_error;
        m_val += out;

        cout << m_val << endl;

        m_prev_error = m_error;
        // std::this_thread::sleep_for(300ms);
        
        std::this_thread::sleep_until(delay);
        std::chrono::duration<double, std::milli> elapsed {now() - start};
        std::cout << "Waited " << elapsed.count() << " ms\n";
    }
}

void PID::idle() {
    // thread pid_thread(test);
    // pid_thread.join();
    thread pid_thread(&PID::control_loop, this); //my hypothesis: member functions hide their first argument which is "this"
    pid_thread.join();
}

void PID::setReference(float reference) {
    m_reference = reference;
}