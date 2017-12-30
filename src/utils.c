//
// Created by Papoun on 27/12/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void exitOnFailure(void *object, const char *error) {
    if (object == NULL) {
        fprintf(stderr, "Erreur: %s", error);
        exit(EXIT_FAILURE);
    }
}