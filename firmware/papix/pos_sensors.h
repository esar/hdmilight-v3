#include "vivepos.h"

void posSensorsInit();
sensor_angle_set_t* posSensorsPoll();
ootx_msg_t* posSensorsGetOotx();
lighthouse_solution_t* posSensorsGetLighthouseSolution();
