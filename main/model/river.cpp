#include "river.h"

River::River(Configuration & newConfig, HydroFileDict & hydroFileDict)
    : p(newConfig,hydroFileDict)
{
    config = newConfig;

    currHydroFile = NULL;
    currWaterTemp = -1.0;
    currPAR = -1;

    width = hydroFileDict.getMaxWidth();
    height = hydroFileDict.getMaxHeight();
}

//TODO This function is relatively slow because of many hashtable lookups
// in HydroFile class.  Consider pros and cons of using a grid instead.
void River::setCurrentHydroFile(HydroFile *newHydroFile) {
    // TODO Get rid of max_vector_component and g.COMPARE_MAX
    double max_vector_component = 0.0;

    for (int i = 0; i < p.getSize(); i++ ) {
        int x = p.pxcor[i];
        int y = p.pycor[i];

        if(newHydroFile->patchExists(x,y)){
            double depth = newHydroFile->getDepth(x,y);
            QVector2D flowVector = newHydroFile->getVector(x,y);
            double flowX = flowVector.x();
            double flowY = flowVector.y();

            //TODO Replace this line with flowVector.length() once we know if it is correct to do so.
            double flowMagnitude = newHydroFile->getFileVelocity(x,y);

            p.hasWater[i] = true;
            p.depth[i] = depth;
            p.flowX[i] = flowX;
            p.flowY[i] = flowY;
            p.flowMagnitude[i] = flowMagnitude;

            if (max_vector_component < fabs(flowX) ) {
                max_vector_component = fabs(flowX);
            }
            if( max_vector_component < fabs(flowY)) {
                max_vector_component = fabs(flowY);
            }

        } else {
            p.hasWater[i] = false;
            p.depth[i] = 0.0;
            p.flowX[i] = 0.0;
            p.flowY[i] = 0.0;
            p.flowMagnitude[i] = 0.0;
        }

        // update miscellanous variables inside the patch

        double current_depth = 0.0;
        if(currHydroFile != NULL && currHydroFile->patchExists(x,y)){
            current_depth = currHydroFile->getDepth(x,y);
        }

        // Water -> Land
        if (current_depth > 0.0 && p.depth[i] == 0.0) {
            p.detritus[i] += p.DOC[i] + p.POC[i] + p.phyto[i] +
                    p.macro[i] + p.waterdecomp[i] +
                    p.seddecomp[i] + p.herbivore[i] + p.sedconsumer[i] + p.consumer[i];

            p.DOC[i] = 0.0;
            p.POC[i] = 0.0;
            p.phyto[i] = 0.0;
            p.macro[i] = 0.0;
            p.waterdecomp[i] = 0.0;
            p.seddecomp[i] = 0.0;
            p.herbivore[i] = 0.0;
            p.sedconsumer[i] = 0.0;
            p.consumer[i] = 0.0;
        }

        // Land -> Water
        if (current_depth == 0.0 && p.depth[i] > 0.0) {
            p.detritus[i] *= 0.5;
        }
    }

    // update the maximum vector for the timestep
    g.COMPARE_MAX = max_vector_component;

    currHydroFile = newHydroFile;
}

void River::setCurrentWaterTemperature(double newTemp) {
    /*
     * TODO I don't know what this calculation is doing...
     * Is it really the "temperature" anymore after this?
     * If not, my member variable's name should change. -ECP
     *
     * Edit: temp_dif is only assigned a value of 0...  Removing for now. Something
     * to ask Kevin I suppose... -ECP
     *
     * Here is original 2011 team's function:
     *
     * g.temperature_index++;
     * g.temperature = g.temperature_data[g.temperature_index];
     * g.temperature = g.temperature - ((g.temperature - 17.0) * g.temp_dif);
     */

    currWaterTemp = newTemp;// - ((newTemp - 17.0) * g.temp_dif);
}

void River::setCurrentPAR(int newPAR) {
    /*
     * TODO I don't know what this calculation is doing...
     * Is it really the "PAR" anymore after this?
     * If not, my member variable's name should change.
     * Also, is integer truncation the right thing to do or should it round? -ECP
     *
     * Edit: par_dif is only assigned a value of 0...  Removing for now. Something
     * to ask Kevin I suppose... -ECP
     *
     * Here is original 2011 team's function:
     *
     * g.photo_radiation_index++;
     * g.photo_radiation = g.photo_radiation_data[g.photo_radiation_index];
     * g.photo_radiation = g.photo_radiation - (int)(g.photo_radiation * g.par_dif);
     */

    currPAR = newPAR;// - (int)(newPAR * g.par_dif);
}

void River::setCurrentGrowthRate(double newRate) {
    currGrowthRate = newRate;
}

void River::flow(Grid<FlowData> * source, Grid<FlowData> * dest) {
    //TODO Replace with CarbonFlowMap stuff

    copyFlowData(*dest);
    copyFlowData(*source);

    int max_time = 60/config.timestep;
    g.nan_trigger = false;
    for (int t = 0; t < max_time; t++)
    {
        std::swap(source, dest);
        flowSingleTimestep(*source, *dest, config);
        if (g.nan_trigger) {
            break;
        }
    }

    storeFlowData(*dest);
}

void River::copyFlowData(Grid<FlowData> & flowData) {
    for(int i = 0; i < p.getSize(); i++) {
        int x = p.pxcor[i];
        int y = p.pycor[i];

        flowData(x,y).hasWater    = p.hasWater[i];
        flowData(x,y).depth       = p.depth[i];
        flowData(x,y).velocity    = p.flowMagnitude[i];
        flowData(x,y).px_vector   = p.flowX[i];
        flowData(x,y).py_vector   = p.flowY[i];
        flowData(x,y).DOC         = p.DOC[i];
        flowData(x,y).POC         = p.POC[i];
        flowData(x,y).phyto       = p.phyto[i];
        flowData(x,y).waterdecomp = p.waterdecomp[i];

    }
}

void River::storeFlowData(Grid<FlowData> & flowData) {
    for(int i = 0; i < p.getSize(); i++) {
        int x = p.pxcor[i];
        int y = p.pycor[i];

        p.DOC[i]         = flowData(x,y).DOC;
        p.POC[i]         = flowData(x,y).POC;
        p.phyto[i]       = flowData(x,y).phyto;
        p.waterdecomp[i] = flowData(x,y).waterdecomp;
    }
}

bool River::is_calc_nan(int x, int y, double move_factor, Grid<FlowData> & dst) {
    if ( /*isnan( dst(x,y).DOC + dst(x,y).DOC*move_factor )*/
        dst(x,y).DOC + dst(x,y).DOC*move_factor
            != dst(x,y).DOC + dst(x,y).DOC*move_factor)
    {
        return true;
    }
    if ( /*isnan( dst(x,y).POC + dst(x,y).POC*move_factor )*/
            dst(x,y).POC + dst(x,y).POC*move_factor
                != dst(x,y).POC + dst(x,y).POC*move_factor)
    {
        return true;
    }
    if ( /*isnan( dst(x,y).phyto + dst(x,y).phyto*move_factor )*/
            dst(x,y).phyto + dst(x,y).phyto*move_factor
                != dst(x,y).phyto + dst(x,y).phyto*move_factor)
    {
        return true;
    }
    if ( /*isnan( dst(x,y).waterdecomp + dst(x,y).waterdecomp*move_factor )*/
            dst(x,y).waterdecomp + dst(x,y).waterdecomp*move_factor
                != dst(x,y).waterdecomp + dst(x,y).waterdecomp*move_factor)
    {
        return true;
    }
    return false;
}

double River::getMaxTimestep() {
    if (g.COMPARE_MAX == 0.0)
    {
        return 1.0;
    }
    return (((double)PATCH_LENGTH)/((double)g.COMPARE_MAX));
}

void River::flowSingleTimestep(Grid<FlowData> &source, Grid<FlowData> &dest, Configuration &config) {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            if( !source(x,y).hasWater || source(x,y).velocity <= 0.0 ) {
                continue;
            }

            double px_vector = source(x,y).px_vector;
            double py_vector = source(x,y).py_vector;

            double corner_patch = fabs( py_vector * px_vector ) / PATCH_AREA;
            double tb_patch = fabs( py_vector*( PATCH_LENGTH - fabs(px_vector) ) ) / PATCH_AREA;
            double rl_patch = fabs( px_vector*( PATCH_LENGTH - fabs(py_vector) ) ) / PATCH_AREA;

            // if a neighbor patch is dry, the carbon does not move in that direction
            double max_timestep = getMaxTimestep();
            int tb_moved = 0;
            int corner_moved = 0;
            int rl_moved = 0;

            int px = (int)(max_timestep * px_vector);
            int py = (int)(max_timestep * py_vector);

            // is this the opposite of what is waned?
            if (config.adjacent) {
                if (px >= 1) {
                    px = 1;
                } else if (px <= -1) {
                    px = -1;
                } else {
                    px = 0;
                }

                if (py >= 1) {
                    py = 1;
                } else if (py <= -1) {
                    py = -1;
                } else {
                    py = 0;
                }
            }

            // flow carbon to the top/bottom patches
            if ( is_valid_patch(x, y+py) && (py!=0) ) {
                if (is_calc_nan(x,y+py,tb_patch, dest)) {
                    g.nan_trigger = true;
                } else {
                    dest(x, y+py).DOC += source(x,y).DOC*tb_patch;
                    dest(x, y+py).POC += source(x,y).POC*tb_patch;
                    dest(x, y+py).phyto += source(x,y).phyto*tb_patch;
                    dest(x, y+py).waterdecomp += source(x,y).waterdecomp*tb_patch;
                    tb_moved = 1;
                }
            }

            // flow carbon to the corner patch
            if ( is_valid_patch(x+px, y+py) && (px!=0) && (py!=0)) {
                if (is_calc_nan(x+px,y+py,corner_patch, dest)) {
                    g.nan_trigger = true;
                } else {
                    dest(x+px, y+py).DOC += source(x,y).DOC*corner_patch;
                    dest(x+px, y+py).POC += source(x,y).POC*corner_patch;
                    dest(x+px, y+py).phyto += source(x,y).phyto*corner_patch;
                    dest(x+px, y+py).waterdecomp += source(x,y).waterdecomp*corner_patch;
                    corner_moved = 1;
                }
            }

            // flow carbon to the left/right patches
            //TODO code pushes carbon onto land patches...
            if ( is_valid_patch(x+px, y) && (px!=0) ) {
                if ( is_calc_nan(x+px,y,rl_patch, dest) ) {
                    g.nan_trigger = true;
                } else {
                    dest(x+px, y).DOC += source(x,y).DOC*rl_patch;
                    dest(x+px, y).POC += source(x,y).POC*rl_patch;
                    dest(x+px, y).phyto += source(x,y).phyto*rl_patch;
                    dest(x+px, y).waterdecomp += source(x,y).waterdecomp*rl_patch;
                    rl_moved = 1;
                }
            }

            // how much components did we loose
            double patch_loss = tb_patch*tb_moved + corner_patch*corner_moved + rl_patch*rl_moved;
            dest(x,y).DOC = source(x,y).DOC - source(x,y).DOC*patch_loss;
            dest(x,y).POC = source(x,y).POC - source(x,y).POC*patch_loss;
            dest(x,y).phyto = source(x,y).phyto - source(x,y).phyto*patch_loss;
            dest(x,y).waterdecomp = source(x,y).waterdecomp - source(x,y).waterdecomp*patch_loss;

        }
    }
}

bool River::is_valid_patch(int x, int y) {
    if (x <0 || y < 0) return false;
    if (x >= width || y >= height) return false;
    return true;
}

Statistics River::generateStatistics() {
    Statistics stats;

    for(int i = 0; i < p.getSize(); i++) {
        if(!p.hasWater[i]) {
            continue;
        }

        stats.waterPatches++;

        double carbon = 0.0;

        //Macro
        carbon += p.macro[i];
        stats.totalMacro += p.macro[i];
        stats.maxMacro = max(stats.maxMacro, p.macro[i]);

        //Phyto
        carbon += p.phyto[i];
        stats.totalPhyto += p.phyto[i];
        stats.maxPhyto = max(stats.maxPhyto, p.phyto[i]);

        //Herbivore
        carbon += p.herbivore[i];
        stats.totalHerbivore += p.herbivore[i];
        stats.maxHerbivore = max(stats.maxHerbivore, p.herbivore[i]);

        //WaterDecomp
        carbon += p.waterdecomp[i];
        stats.totalWaterDecomp += p.waterdecomp[i];
        stats.maxWaterDecomp = max(stats.maxWaterDecomp, p.waterdecomp[i]);

        //SedDecomp
        carbon += p.seddecomp[i];
        stats.totalSedDecomp += p.seddecomp[i];
        stats.maxSedDecomp = max(stats.maxSedDecomp, p.seddecomp[i]);

        //SedConsum
        carbon += p.sedconsumer[i];
        stats.totalSedConsumer += p.sedconsumer[i];
        stats.maxSedConsumer = max(stats.maxSedConsumer, p.sedconsumer[i]);

        //Consumer
        carbon += p.consumer[i];
        stats.totalConsum += p.consumer[i];
        stats.maxConsum = max(stats.maxConsum, p.consumer[i]);

        //DOC
        carbon += p.DOC[i];
        stats.totalDOC += p.DOC[i];
        stats.maxDOC = max(stats.maxDOC, p.DOC[i]);

        //POC
        carbon += p.POC[i];
        stats.totalPOC += p.POC[i];
        stats.maxPOC = max(stats.maxPOC, p.POC[i]);

        //Detritus
        carbon += p.detritus[i];
        stats.totalDetritus += p.detritus[i];
        stats.maxDetritus = max(stats.maxDetritus, p.detritus[i]);

        //Carbon
        stats.totalCarbon += carbon;
        stats.maxCarbon = max(stats.maxCarbon, carbon);
    }

    stats.avgMacro       = stats.totalMacro / stats.waterPatches;
    stats.avgPhyto       = stats.totalPhyto / stats.waterPatches;
    stats.avgHerbivore   = stats.totalHerbivore / stats.waterPatches;
    stats.avgWaterDecomp = stats.totalWaterDecomp / stats.waterPatches;
    stats.avgSedDecomp   = stats.totalSedDecomp / stats.waterPatches;
    stats.avgSedConsumer   = stats.totalSedConsumer / stats.waterPatches;
    stats.avgConsum      = stats.totalConsum / stats.waterPatches;
    stats.avgDOC         = stats.totalDOC / stats.waterPatches;
    stats.avgPOC         = stats.totalPOC / stats.waterPatches;
    stats.avgDetritus    = stats.totalDetritus / stats.waterPatches;
    stats.avgCarbon      = stats.totalCarbon / stats.waterPatches;

    return stats;
}

//TODO Use QFile
int River::saveCSV(QString displayedStock, int daysElapsed) const {
    QString file_name = "./results/data/map_data_";
    QDateTime date_time = QDateTime::currentDateTime();
    QString date_time_str = date_time.toString("MMM_d_H_mm_ss");
    file_name.append(date_time_str);
    file_name.append(".csv");

    const char* cfile_name = file_name.toStdString().c_str();
    FILE* f = fopen(cfile_name, "w");
    if (f == NULL) {
        printf("file name: %s could not be opened\n", cfile_name);
        return 0;
    }

    // GUI variables used
    fprintf(f,"%s\n","# timestep_factor,hydro_group,days_to_run,tss,k_phyto,k_macro,sen_macro_coef,resp_macro_coef,macro_base_temp,macro_mass_max,macro_vel_max,gross_macro_coef,which_stock");

    fprintf(f,"%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%s\n",
            config.timestep, daysElapsed, config.tss,
            config.kPhyto, config.kMacro, (config.macroSenescence/24),
            (config.macroRespiration/24), config.macroTemp, config.macroMassMax,
            config.macroVelocityMax, config.macroGross, displayedStock.toStdString().c_str());

    //TODO Print out the hydrofile used for this simulated day.

    fprintf(f,"%s\n","# pxcor,pycor,pcolor,px_vector,py_vector,depth,velocity,assimilation,detritus,DOC,POC,waterdecomp,seddecomp,macro,phyto,herbivore,sedconsumer,peri,consumer");

    int x,y;
    int pxcor, pycor, pcolor;
    double px_vector, py_vector;
    double depth;
    double velocity;
    double assimilation;
    double detritus, DOC, POC, waterdecomp, seddecomp, macro, phyto, herbivore, sedconsumer, peri, consumer;

    for(x = 0; x < width; x++) {
        for(y=0;y < height; y++) {
            //Skip if cell doesn't exist or is land
            if( !currHydroFile->patchExists(x,y) || currHydroFile->getDepth(x,y) <= 0.0 ) {
                continue;
            }

            depth = currHydroFile->getDepth(x,y);

            QVector2D flowVector = currHydroFile->getVector(x,y);
            velocity = currHydroFile->getFileVelocity(x,y);

            int i = p.getIndex(x,y);
            pxcor = p.pxcor[i];
            pycor = p.pycor[i];
            pcolor = p.pcolor[i];
            px_vector = flowVector.x();
            py_vector = flowVector.y();
            assimilation = p.assimilation[i];
            detritus = p.detritus[i];
            DOC = p.DOC[i];
            POC = p.POC[i];
            waterdecomp = p.waterdecomp[i];
            seddecomp = p.seddecomp[i];
            macro = p.macro[i];
            phyto = p.phyto[i];
            herbivore = p.herbivore[i];
            sedconsumer = p.sedconsumer[i];
            peri = p.peri[i];
            consumer = p.consumer[i];


            fprintf(f,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",pxcor,pycor,pcolor,px_vector,py_vector,depth,
                      velocity,assimilation,detritus,DOC,POC,
                      waterdecomp,seddecomp,macro,phyto,herbivore,sedconsumer,peri,consumer);
        }
    }
    fclose(f);
    return 1;
}

void River::generateImages(QVector<QImage> &images, QVector<QString> & stockNames,
                           QMutex &imageMutex, Statistics & stats)
{
    imageMutex.lock();
    for(int imageIndex = 0; imageIndex < NUM_IMAGES; imageIndex++){
        QColor color("black");
        images[imageIndex].fill(color.rgb());
    }

    for(int i = 0; i < p.getSize(); i++){
        if(!p.hasWater[i]) {
            continue;
        }

        int x = p.pxcor[i];
        int y = p.pycor[i];

        QColor macroColor = getHeatMapColor(p.macro[i], stats.avgMacro, stats.maxMacro);
        QColor phytoColor = getHeatMapColor(p.phyto[i], stats.avgPhyto, stats.maxPhyto);
        QColor herbivoreColor = getHeatMapColor(p.herbivore[i], stats.avgHerbivore, stats.maxHerbivore);
        QColor waterDecompColor = getHeatMapColor(p.waterdecomp[i], stats.avgWaterDecomp, stats.maxWaterDecomp);
        QColor sedDecompColor = getHeatMapColor(p.seddecomp[i], stats.avgSedDecomp, stats.maxSedDecomp);
        QColor sedConsumerColor = getHeatMapColor(p.sedconsumer[i], stats.avgSedConsumer, stats.maxSedConsumer);
        QColor consumColor = getHeatMapColor(p.consumer[i], stats.avgConsum, stats.maxConsum);
        QColor DOCColor = getHeatMapColor(p.DOC[i], stats.avgDOC, stats.maxDOC);
        QColor POCColor = getHeatMapColor(p.POC[i], stats.avgPOC, stats.maxPOC);
        QColor detritusColor = getHeatMapColor(p.detritus[i], stats.avgDetritus, stats.maxDetritus);

        images[STOCK_MACRO].setPixel( x, y, macroColor.rgb());
        images[STOCK_PHYTO].setPixel(x, y, phytoColor.rgb());
        images[STOCK_HERBIVORE].setPixel(x, y, herbivoreColor.rgb());
        images[STOCK_WATERDECOMP].setPixel( x, y, waterDecompColor.rgb());
        images[STOCK_SEDDECOMP].setPixel(x, y, sedDecompColor.rgb());
        images[STOCK_SEDCONSUMER].setPixel(x, y, sedConsumerColor.rgb());
        images[STOCK_CONSUMER].setPixel(x, y, consumColor.rgb());
        images[STOCK_DOC].setPixel(x, y, DOCColor.rgb());
        images[STOCK_POC].setPixel(x, y, POCColor.rgb());
        images[STOCK_DETRITUS].setPixel(x, y, detritusColor.rgb());

        int patchCarbon = p.macro[i] + p.phyto[i] + p.herbivore[i] + p.waterdecomp[i] + p.seddecomp[i]
                + p.sedconsumer[i] + p.consumer[i] + p.DOC[i] + p.POC[i] + p.detritus[i];
        QColor allCarbonColor = getHeatMapColor(patchCarbon, stats.avgCarbon, stats.maxCarbon);
        images[STOCK_ALL_CARBON].setPixel(x, y, allCarbonColor.rgb());
    }

    //Due to the layout of the hydrofiles, the river will appear upside down if we don't flip it.
    for(int imageIndex = 0; imageIndex < NUM_IMAGES; imageIndex++){
        images[imageIndex] = images[imageIndex].mirrored(false,true);
    }
    imageMutex.unlock();

    QImageWriter writer;
    writer.setFormat("png");

    for(int i = 0; i < NUM_IMAGES; i++){
        QString date_time_str = QDateTime::currentDateTime().toString("_MMM_d_H_mm_ss");

        QString fileName = "./results/images/" + stockNames[i] + date_time_str + ".png";
        writer.setFileName(fileName);
        writer.write(images[i]);
    }
}

/**
 * Scales the color of the patch from green to red.
 * @param value The value of the patch
 * @param maxVal the max value for a patch
 */
QColor River::getHeatMapColor(double carbonValue, double avgVal, double maxVal) {
    if( carbonValue <= 0.0 || maxVal <= 0.0 ) {
        return QColor("green");
    }

    if( carbonValue == avgVal) {
        return QColor("yellow");
    }

    if( carbonValue >= maxVal) {
        return QColor("red");
    }

    double distFromAverage = fabs(carbonValue - avgVal);

    if( carbonValue < avgVal) {
        double relativeValue = distFromAverage / avgVal;
        return QColor::fromHsv( 60 + (int)(60*relativeValue),255,255);
    } else {
        double relativeValue = distFromAverage / (maxVal - avgVal);
        return QColor::fromHsv( 60 - (int)(60*relativeValue),255,255);
    }
}

void River::processPatches() {
    #pragma omp parallel
    {
        PatchComputation::updatePatches(p, config, currPAR);
        PatchComputation::macro(p, config, currPAR, currWaterTemp, currGrowthRate);
        PatchComputation::phyto(p, config, currPAR, currWaterTemp, currGrowthRate);
        PatchComputation::herbivore(p, config);
        PatchComputation::waterDecomp(p, config);
        PatchComputation::sedDecomp(p, config);
        PatchComputation::sedConsumer(p, config);
        PatchComputation::consumer(p, config);
        PatchComputation::DOC(p, config, currGrowthRate);
        PatchComputation::POC(p, currGrowthRate);
        PatchComputation::detritus(p, config, currGrowthRate);

        #pragma omp for
        for(int i = 0; i < p.getSize(); i++) {
            //Only process patches if they currently contain water
            if(!p.hasWater[i]) {
                continue;
            }

            PatchComputation::predPhyto(p, i);
            PatchComputation::predHerbivore(p, i);
            PatchComputation::predSedDecomp(p, i);
            PatchComputation::predWaterDecomp(p, i);
            PatchComputation::predSedConsumer(p, i);
            PatchComputation::predDetritus(p, i);
            PatchComputation::predDOC(p, i);
            PatchComputation::predPOC(p, i);
            PatchComputation::predConsum(p, i);
        }
    }
}
