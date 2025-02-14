#ifndef AUDITORY_PERCEPTION_EARS_H
#define AUDITORY_PERCEPTION_EARS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <fftw3.h>
#include <math.h>

// Constants from ears.h
#define SAMPLING_RATE 44100
#define FFT_SIZE 2048
#define WINDOW_SIZE FFT_SIZE
#define MIN_FREQ 20.0
#define MAX_FREQ 120000.0
#define NOTES_PER_OCTAVE 7

// Constants from auditory_perception.h
#define MAX_PERCEPTIONS 100
#define MAX_LABEL_LENGTH 50

// Structures from ears.h
typedef struct {
    double *time_domain;
    fftw_complex *frequency_domain;
    fftw_plan plan;
} AudioData;

typedef struct {
    double cochlea_frequency;
    double auditory_signal;
    double vestibular_adjustment;
} InnerEar;

typedef struct {
    const char *note;
    double frequency;
} MusicalNote;

// Structures from auditory_perception.h
typedef struct {
    double confidence;
    char label[MAX_LABEL_LENGTH];
} Perception;

typedef struct {
    Perception perceptions[MAX_PERCEPTIONS];
    int num_perceptions;
} AuditoryScene;

// Function prototypes from ears.c
int init_audio_data(AudioData **audio_data);
void free_audio_data(AudioData *audio_data);
int perform_fourier_transform(AudioData *audio_data);
void perform_inverse_fft(AudioData *audio_data);
void apply_cochlear_filter(AudioData *audio_data, InnerEar *inner_ear);
void apply_window_function(AudioData *audio_data);
void apply_convolution(AudioData *audio_data, const double *kernel, int kernel_size);
void apply_frequency_domain_convolution(AudioData *audio_data, const double *kernel, int kernel_size);
void process_with_overlap_add(AudioData *audio_data, const double *input, int input_length, int hop_size);
void generate_fibonacci_sequence(double *sequence, int *length);
void simulate_octave_range(InnerEar *inner_ear);
void populate_time_domain_signal(AudioData *audio_data, double frequency, double duration);
void simulate_cochlear_response(InnerEar *inner_ear, double frequency);

// Function prototypes from auditory_perception.c
int init_auditory_scene(AuditoryScene **scene);
void free_auditory_scene(AuditoryScene *scene);
int process_audio_for_perception(AudioData *audio_data, AuditoryScene *scene, InnerEar *inner_ear);
void classify_sound(AuditoryScene *scene);
void identify_speaker(AuditoryScene *scene);
void contextualize_sound(AuditoryScene *scene, const char *context);
double find_dominant_frequency(AudioData *audio_data);

// Error codes
#define EARS_SUCCESS 0
#define EARS_MEMORY_ERROR -1

#endif // AUDITORY_PERCEPTION_EARS_H
