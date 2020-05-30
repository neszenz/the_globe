#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <string>
#include <utility>
#include <vector>

#define PROFILER Profiler::getInstance()
#define PROFILER_RESET PROFILER.reset()
#define PROFILE(X) PROFILER.add_profile(#X);\
                   X;\
                   PROFILER.stop_latest_profile()
#define PROFILER_STR PROFILER.print_to_string()
#define PROFILER_C_STR PROFILER.print_to_string().c_str()

class Profiler {
    public:
        static Profiler& getInstance() {
            static Profiler instance;
            return instance;
        }
        Profiler (const Profiler&)       = delete;
        void operator=(const Profiler&)  = delete;

        void reset();
        void add_profile(const std::string& name);
        void stop_latest_profile();
        std::string print_to_string();

    private:
        typedef std::pair<double, double> start_stop_t;
        typedef std::pair<std::string, start_stop_t> profile_t;

        Profiler() {}

        std::vector<profile_t> m_profiles;
};

#endif//PROFILER_HPP
