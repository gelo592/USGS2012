#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include <QVector>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>

struct Configuration
{

/** Structure of the .conf file:
  *
  *             Item                            (type)
  *---------------------------------------------------------
  *     Adjacent cells                          (bool)
  *     Consum stock selected                   (bool)
  *     Detritus stock selected                 (bool)
  *     DOC stock selected                      (bool)
  *     Herbivore stock selected                (bool)
  *     Macro stock selected                    (bool)
  *     POC stock selected                      (bool)
  *     Phyto stock selected                    (bool)
  *     Sedconsumer stock selected              (bool)
  *     Seddecomp stock selected                (bool)
  *     Waterdecomp stock selected              (bool)
  *     Temperature file name                   (char*)
  *     PAR file name                           (char*)
  *     Number of hydro maps                    (int)
  *     Hydro Map file names (one per line)     (char*)
  *     Days to Run (one per line)              (int)
  *     Output frequency                        (int)
  *     Timestep                                (int)
  *     TSS                                     (float)
  *     KPhyto                                  (float)
  *     KMacro                                  (float)
  *     Which Stock                             (char*)
  *     Macro                                   (float)
  *     Phyto                                   (float)
  *     Consumer                                (float)
  *     Decomp                                  (float)
  *     Sedconsumer                             (float)
  *     Seddecomp                               (float)
  *     Herbivore                               (float)
  *     Detritus                                (float)
  *     POC                                     (float)
  *     DOC                                     (float)
  *     Phyto Senescence                        (float)
  *     Phyto Respiration                       (float)
  *     Phyto Excretion                         (float)
  *     Phyto Aj                                (float)
  *     Phyto Gj                                (float)
  *     Herbivore Ai Phyto                      (float)
  *     Herbivore Gi Phyto                      (float)
  *     Herbivore Pref Phyto                    (float)
  *     Herbivore Ai Peri                       (float)
  *     Herbivore Gi Peri                       (float)
  *     Herbivore Pref Peri                     (float)
  *     Herbivore Ai Waterdecomp                (float)
  *     Herbivore Gi Waterdecomp                (float)
  *     Herbivore Pref Waterdecomp              (float)
  *     Herbivore Aj                            (float)
  *     Herbivore Gj                            (float)
  *     Herbivore Respiration                   (float)
  *     Herbivore Excretion                     (float)
  *     Herbivore Egestion                      (float)
  *     Herbivore Senescence                    (float)
  *     Herbivore Max                           (float)
  *     Waterdecomp Ai DOC                      (float)
  *     Waterdecomp Gi DOC                      (float)
  *     Waterdecomp Pref DOC                    (float)
  *     Waterdecomp Ai POC                      (float)
  *     Waterdecomp Gi POC                      (float)
  *     Waterdecomp Pref POC                    (float)
  *     Waterdecomp Aj                          (float)
  *     Waterdecomp Gj                          (float)
  *     Waterdecomp Respiration                 (float)
  *     Waterdecomp Excretion                   (float)
  *     Waterdecomp Senescence                  (float)
  *     Waterdecomp Max                         (float)
  *     Seddecomp Ai Detritus                   (float)
  *     Seddecomp Gi Detritus                   (float)
  *     Seddecomp Pref Detritus                 (float)
  *     Seddecomp Aj                            (float)
  *     Seddecomp Gj                            (float)
  *     Seddecomp Respiration                   (float)
  *     Seddecomp Excretion                     (float)
  *     Seddecomp Senescence                    (float)
  *     Seddecomp Max                           (float)
  *     Consumer Ai Herbivore                   (float)
  *     Consumer Gi Herbivore                   (float)
  *     Consumer Pref Herbivore                 (float)
  *     Consumer Ai Sedconsumer                 (float)
  *     Consumer Gi Sedconsumer                 (float)
  *     Consumer Pref Sedconsumer               (float)
  *     Consumer Aj                             (float)
  *     Consumer Gj                             (float)
  *     Consumer Respiration                    (float)
  *     Consumer Excretion                      (float)
  *     Consumer Senescence                     (float)
  *     Consumer Egestion                       (float)
  *     Consumer Max                            (float)
  *     Macro Senescence                        (float)
  *     Macro Respiration                       (float)
  *     Macro Excretion                         (float)
  *     Macro Temperature                       (float)
  *     Macro Gross                             (float)
  *     Macro Mass Max                          (float)
  *     Macro Velocity Max                      (float)
  *     Sedconsumer Ai Detritus                 (float)
  *     Sedconsumer Gi Detritus                 (float)
  *     Sedconsumer Pref Detritus               (float)
  *     Sedconsumer Ai Seddecomp                (float)
  *     Sedconsumer Gi Seddecomp                (float)
  *     Sedconsumer Pref Seddecomp              (float)
  *     Sedconsumer Aj                          (float)
  *     Sedconsumer Gj                          (float)
  *     Sedconsumer Respiration                 (float)
  *     Sedconsumer Excretion                   (float)
  *     Sedconsumer Senescence                  (float)
  *     Sedconsumer Max                         (float)
  */

public:
    Configuration();

    /* Write all data to a file */
    void write(QString const & filename) const;

    /* Read in a file */
    void read(QString const & filename);

    bool adjacent;

    QString tempFile;
    QString parFile;
    QVector<QString> hydroMaps;

    uint8_t numStocks;
    uint16_t numHydroMaps;

    uint8_t outputFreq;
    uint8_t timestep;
    QVector<uint16_t> daysToRun;

    float tss;
    float kPhyto;
    float kMacro;

    // stock values
    QString whichStock;

    float macro;
    float phyto;
    float consumer;
    float decomp;
    float sedconsumer;
    float seddecomp;
    float herbivore;
    float detritus;
    float poc;
    float doc;

    float phytoSenescence;
    float phytoRespiration;
    float phytoExcretion;
    float phytoAj;
    float phytoGj;

    float herbivoreAiPhyto;
    float herbivoreGiPhyto;
    float herbivorePrefPhyto;
    float herbivoreAiPeri;
    float herbivoreGiPeri;
    float herbivorePrefPeri;
    float herbivoreAiWaterdecomp;
    float herbivoreGiWaterdecomp;
    float herbivorePrefWaterdecomp;
    float herbivoreAj;
    float herbivoreGj;
    float herbivoreRespiration;
    float herbivoreExcretion;
    float herbivoreEgestion;
    float herbivoreSenescence;
    float herbivoreMax;

    float waterdecompAiDoc;
    float waterdecompGiDoc;
    float waterdecompPrefDoc;
    float waterdecompAiPoc;
    float waterdecompGiPoc;
    float waterdecompPrefPoc;
    float waterdecompAj;
    float waterdecompGj;
    float waterdecompRespiration;
    float waterdecompExcretion;
    float waterdecompSenescence;
    float waterdecompMax;

    float seddecompAiDetritus;
    float seddecompGiDetritus;
    float seddecompPrefDetritus;
    float seddecompAj;
    float seddecompGj;
    float seddecompRespiration;
    float seddecompExcretion;
    float seddecompSenescence;
    float seddecompMax;

    float consumerAiHerbivore;
    float consumerGiHerbivore;
    float consumerPrefHerbivore;
    float consumerAiSedconsumer;
    float consumerGiSedconsumer;
    float consumerPrefSedconsumer;
    float consumerAj;
    float consumerGj;
    float consumerRespiration;
    float consumerExcretion;
    float consumerSenescence;
    float consumerEgestion;
    float consumerMax;

    float macroSenescence;
    float macroRespiration;
    float macroExcretion;
    float macroTemp;
    float macroGross;
    float macroMassMax;
    float macroVelocityMax;

    float sedconsumerAiDetritus;
    float sedconsumerGiDetritus;
    float sedconsumerPrefDetritus;
    float sedconsumerAiSeddecomp;
    float sedconsumerGiSeddecomp;
    float sedconsumerPrefSeddecomp;
    float sedconsumerAj;
    float sedconsumerGj;
    float sedconsumerRespiration;
    float sedconsumerExcretion;
    float sedconsumerSenescence;
    float sedconsumerMax;


private:
    bool nextBool(std::ifstream & file, std::string & str);
    uint16_t nextInt(std::ifstream & file, std::string & str);
    float nextFloat(std::ifstream & file, std::string & str);

    std::string & nextLine(std::ifstream & file, std::string & str) const;
};

#endif // CONFIGURATION_H
