#ifndef EARS_H
#define EARS_H

#include <complex.h>
#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLING_RATE 44100
#define FFT_SIZE 2048
#define WINDOW_SIZE FFT_SIZE
#define MIN_FREQ 20.0 // Hz, lower limit of human hearing
#define MAX_FREQ 120000.0 // Hz, upper limit of AI hearing range
#define OCTAVE_BASE 8.0
#define NOTES_PER_OCTAVE 7

// Struct for audio data processing
typedef struct {
    double *time_domain;
    double_complex *frequency_domain;
    fftw_plan plan;
} AudioData;

// Struct for cochlear properties
typedef struct {
    double cochlea_frequency; // Central frequency of the cochlear response
    double auditory_signal;
    double vestibular_adjustment;
} InnerEar;

// Struct to represent musical notes in an octave
typedef struct {
    const char *note;
    double frequency;
} MusicalNote;

// Function prototypes
void init_audio_data(AudioData **audio_data);
void free_audio_data(AudioData *audio_data);
void perform_fourier_transform(AudioData *audio_data);
void apply_cochlear_filter(AudioData *audio_data, InnerEar *inner_ear);
void generate_fibonacci_sequence(double *sequence, int *length);
void simulate_octave_range(InnerEar *inner_ear);
void populate_time_domain_signal(AudioData *audio_data, double frequency, double duration);

#endif // EARS_H
