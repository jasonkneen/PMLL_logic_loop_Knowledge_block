#ifndef AUDITORY_PERCEPTION_H
#define AUDITORY_PERCEPTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ears.h" // Assuming we're building on top of the existing Ears system

#define MAX_PERCEPTIONS 100
#define MAX_LABEL_LENGTH 50

// Struct for auditory perception
typedef struct {
    double confidence;
    char label[MAX_LABEL_LENGTH];
} Perception;

// Struct for auditory scene
typedef struct {
    Perception perceptions[MAX_PERCEPTIONS];
    int num_perceptions;
} AuditoryScene;

// Function prototypes
int init_auditory_scene(AuditoryScene **scene);
void free_auditory_scene(AuditoryScene *scene);
int process_audio_for_perception(AudioData *audio_data, AuditoryScene *scene, InnerEar *inner_ear);
void classify_sound(AuditoryScene *scene);
void identify_speaker(AuditoryScene *scene);
void contextualize_sound(AuditoryScene *scene, const char *context);

#endif // AUDITORY_PERCEPTION_H
