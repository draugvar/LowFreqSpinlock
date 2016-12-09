//
// Created by Salvatore Rivieccio on 06/10/16.
//

#ifndef CLIONPROJECTS_UTIL_H
#define CLIONPROJECTS_UTIL_H

#endif //CLIONPROJECTS_UTIL_H

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

/*
 * Turn A into a string literal after macro-expanding it.
 */
#define STRINGIZE(A) STRINGIZE_NX(A)

#define VIRTUAL

#ifdef VIRTUAL
    #define CPUINFO_MIN "/home/cpu/cpu%i/cpufreq/cpuinfo_min_freq"
    #define CPUINFO_MAX "/home/cpu/cpu%i/cpufreq/cpuinfo_max_freq"
    #define SCALING_MIN "/home/cpu/cpu%i/cpufreq/scaling_min_freq"
    #define SCALING_MAX "/home/cpu/cpu%i/cpufreq/scaling_max_freq"
#endif

#ifndef VIRTUAL
    #define CPUINFO_MIN "/sys/devices/system/cpu/cpu%i/cpufreq/cpuinfo_min_freq"
    #define CPUINFO_MAX "/sys/devices/system/cpu/cpu%i/cpufreq/cpuinfo_max_freq"
    #define SCALING_MIN "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_min_freq"
    #define SCALING_MAX "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_max_freq"
#endif