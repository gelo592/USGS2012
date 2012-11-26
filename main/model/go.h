#ifndef GO_H
#define GO_H

#include "math.h"
#include <sys/types.h>
#include <time.h>

#include "globals.h"
#include "dump.h"
#include "pred.h"
#include "color.h"
#include "flowdata.h"
#include "grid.h"

/* Runs the model */
void go();

/* Updates the environmental variables (Primary macrophyte routine) */
void update_environmentals();

/**
 * Gives the value 1 if the two hydros are different to hydro_changed else 0
 * @param old_hydro: the previous value of the hydro_group
 * @param new_hydro: the new value of the hydro_group
 */
void update_hydro_changed(int old_hydro, int new_hydro);

/**
 * Iterates through all the patches and sets the different components
 * inside the patch, it also computes the maximum_vector in the river and sets it to COMPARE_MAX 
 */
void update_hydro_map();

/**
 * Updates the temperature variable
 */
void update_temp();

/**
 * Updates the photosynthetic radiation variable
 */
void update_par();

/**
 * @return the max_timestep based on the greatest x-y vector
 */
double get_timestep();

/**
 * @return 1 if DOC, POC, phyto, waterdecomp are NaN 0 otherwise
 */
int is_nan(int x, int y, double move_factor);

/**
 * Takes two sets of flow data and writes the changes to the destination set.
 * This way carbon can not flow more than once per iteration.
 */
void flow_carbon(Grid<FlowData> & source, Grid<FlowData> & dest);

/**
 * Checks if the x, y values for the patch is within boundaries of the map
 * @param x: the x-coordinate of the patch
 * @param y: the y-coordinate of the patch
 * @return 1 if valid, 0 otherwise
 */
int is_valid_patch(int x, int y);

/**
 * @return the number of days that have passed since the start of the code
 */
int get_day();

/**
 * Updates the global variables for the stocks
 * @param x: the x-coordinate of the patch
 * @param y: the y-coordinate of the patch
 */
void update_max(int x, int y);

/**
 * Copies the flow data from patches into a grid object.
 * @param flowData: The grid object to copy the patches data into.
 */
void copyFlowData(Grid<FlowData> & flowData);

/**
 * Writes the flow data back into patches array.
 * @param flowData: The grid object to copy the data from.
 */
void storeFlowData(Grid<FlowData> & flowData);
#endif
