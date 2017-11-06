#ifndef VEHINFO_H
#define VEHINFO_H

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
//#include <omnetpp.h>

//#include "veins/base/utils/Coord.h"
#include "Coord.h"
//#include "veins/base/utils/FindModule.h"
//#include "veins/modules/mobility/traci/TraCIConnection.h"

namespace Veins{

class VehInfo {

public:
    VehInfo();//: plane(new double[175][500]){};
    ~VehInfo();

protected:
    struct vehicle_bundle{
        std::string ObjId;
        Coord pos;
        double speed;
        double angle;
    };

    vehicle_bundle plane[175][500];

    void update_pos( std::string objId, Coord pos, double speed, double angle);
    bool entry_exists(std::string objId);
    int create_entry(std::string objId, Coord pos, double speed, double angle );
    bool validate_insert(std::string objId, Coord pos, double speed, double angle);
    bool check_collision();
    bool check_neighbourhood(std::string objId, Coord pos, double speed, double angle);
    void handle_collision(std::string objId);
    double get_distance(Coord first, Coord second);
    Coord get_current_position(std::string ObjId);

};

/*
struct <Name of struct> {
    members
} <instance name of struct>;
*/

}


#endif
