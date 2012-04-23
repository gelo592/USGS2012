#include "color.h"
#include <math.h>

void scale_color(double value, double maxVal, double minVal, int x, int y){
    float returnValue;
    if(maxVal == minVal){
        printf("Max and min value are equal in scale_color.\n");
        returnValue = 0.0;
        colorValues[x][y] = returnValue;
        return;
    }

    if(value <= minVal || isnan(value) ){
        returnValue = 0.0;
    }
    else if(value >= maxVal){
        returnValue = 1.0;
    }
    else{
        float rangeValues = fabs(maxVal - minVal);
        returnValue = (value / rangeValues);
    }
   // printf("ReturnValue: %f\n", returnValue);	
    colorValues[x][y] = returnValue;
}

void update_color(){
    int x = 0;
    int y = 0;


    if( strcmp(which_stock, "macro") == 0){
		hue = 120.0 / 360.0;
        for(y = 0; y < MAP_HEIGHT; y++){
            for(x = 0; x < MAP_WIDTH; x++){
                if (patches[x][y].depth == 0.0) {
                    colorValues[x][y] = -1;
                }
                else
                {
                  scale_color(patches[x][y].macro, MAX_MACRO, 0.0, x, y); 
                }
            }
        }
    }


    else if( strcmp(which_stock, "phyto") == 0){
		hue = 120.0 / 360.0;
        double testAvg = 0;
        int testIndex = 0;
        
        for(y=0; y < MAP_HEIGHT; y++){
          for(x = 0; x < MAP_WIDTH; x++){
            testAvg +=patches[x][y].phyto;
            testIndex++;
          }
        }
        double test = testAvg/testIndex;

        for(y = 0; y < MAP_HEIGHT; y++){
            for(x = 0; x < MAP_WIDTH; x++){
                if (patches[x][y].depth == 0.0) {
                    colorValues[x][y] = -1;
                }
                else
                {
                  //printf("Phyto value: %f\n", patches[x][y].phyto);
                  scale_color(patches[x][y].phyto, test, 0.0, x, y);
                }
            }
        }
    }


    else if( strcmp(which_stock, "waterdecomp") == 0){
		hue = 120.0 / 360.0;
        for(y = 0; y < MAP_HEIGHT; y++){
            for(x = 0; x < MAP_WIDTH; x++){
                if (patches[x][y].depth == 0.0) {
                    colorValues[x][y] = -1;
                }
                else
                    scale_color(patches[x][y].waterdecomp, MAX_WATERDECOMP, 0.0, x, y);
            }
        }
    }


    else if( strcmp(which_stock, "POC") == 0){
		hue = 240.0 / 360.0;
        for(y = 0; y < MAP_HEIGHT; y++){
            for(x = 0; x < MAP_WIDTH; x++){
                if (patches[x][y].depth == 0.0) {
                    colorValues[x][y] = -1;
                }
                else
                    scale_color(patches[x][y].POC, MAX_POC, 0.0, x, y);
            }
        }
    }
    /*TODO: Brown and Yellow are used as colors for the Netlogo code, but here
     * we use only green.
     */
    else if( strcmp(which_stock, "detritus") == 0){
		hue = 19.6 / 360.0;
        for(y = 0; y < MAP_HEIGHT; y++){
            for(x = 0; x < MAP_WIDTH; x++){
                if (patches[x][y].depth == 0.0) {
                    colorValues[x][y] = -1;
                }
                else
                    scale_color(patches[x][y].detritus, MAX_DETRITUS, 0.0, x, y);
            }
        }
    }


    else if( strcmp(which_stock, "sedconsumer") == 0){
		hue = 60.0 / 360.0;
        for(y = 0; y < MAP_HEIGHT; y++){
            for(x = 0; x < MAP_WIDTH; x++){
                if (patches[x][y].depth == 0.0) {
                    colorValues[x][y] = -1;
                }
                else
                    scale_color(patches[x][y].sedconsumer, MAX_SEDCONSUMER, 0.0, x, y);
            }
        }
    }

	else if( strcmp(which_stock, "seddecomp") == 0){
		hue = 240.0 / 360.0;
		for(y = 0; y < MAP_HEIGHT; y++){
			for(x = 0; x < MAP_WIDTH; x++){
				if(patches[x][y].depth == 0.0){
					colorValues[x][y] = -1;
				}
				else //Magic numbers taken from original Netlogo code
					scale_color(patches[x][y].seddecomp, MAX_SEDDECOMP, 0.0, x, y);
			}
		}
	}
	
	else if( strcmp(which_stock, "herbivore") == 0){
		hue = 300.0 / 360.0;
		for(y = 0; y < MAP_HEIGHT; y++){
			for(x = 0; x < MAP_WIDTH; x++){
				if(patches[x][y].depth == 0.0){
					colorValues[x][y] = -1;
				}
				else //Magic numbers taken from original Netlogo code
					scale_color(patches[x][y].herbivore, MAX_HERBIVORE, 0.0, x, y);
			}
		}
	}
	
	else if( strcmp(which_stock, "consum") == 0){
		hue = 300.0 / 360.0;
		for(y = 0; y < MAP_HEIGHT; y++){
			for(x = 0; x < MAP_WIDTH; x++){
				if(patches[x][y].depth == 0.0){
					colorValues[x][y] = -1;
				}
				else{ //Magic numbers taken from original Netlogo code
                    printf("Consum values: %f\n", patches[x][y].consum);
					scale_color(patches[x][y].consum, MAX_CONSUM, 0.0, x, y);
                }
			}
		}
	}
	
	else if( strcmp(which_stock, "DOC") == 0){
		hue = 60.0 / 360.0;
		for(y = 0; y < MAP_HEIGHT; y++){
			for(x = 0; x < MAP_WIDTH; x++){
				if(patches[x][y].depth == 0.0){
					colorValues[x][y] = -1;
				}
				else //Magic numbers taken from original Netlogo code
					scale_color(patches[x][y].herbivore, MAX_DOC, 0.0, x, y);
			}
		}
	}
}
