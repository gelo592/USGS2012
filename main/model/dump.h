#ifndef DUMP_H
#define DUMP_H

#include <cstring>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#include <QMutex>
#include <QString>
#include <QDateTime>

#include "globals.h"
#include "patch.h"
#include "configuration.h"


/**
 * The file format is as follows:
 * First line: GUI_VARIABLES separated by a comma
 * Each line represents one coordinate (x,y) in the River and holds the values of the DOCs in a patch.
 * @return 1 on successful completion, 0 indicates failure file could not be created.
 */
int write_data(const Configuration & config);

/**
 * Creates a folder <data_path> and places all the results from running the model into this file.
 * @return 1 on successful completion, 0 indicates failure file could not be created.
 */
int dump_data(const Configuration & config);

/* Outputs an image */
void output_image();
QString make_file_name(int index);
void average_carbon(int day);

#endif
