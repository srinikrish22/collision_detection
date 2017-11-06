#ifndef VEHINFO_H
#define VEHINFO_H

#define V_COLLISION_BOUND_ROW 5
#define V_COLLISION_BOUND_COL 5

#define V_COL_DETECT_BOUND_ROW 7
#define V_COL_DETECT_BOUND_COL 7

#define Y_AXIS_LOW 0
#define X_AXIS_LOW 0
#define Y_AXIS_HIGH 174
#define X_AXIS_HIGH 499


#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <array>
//#include <omnetpp.h>

//#include "veins/base/utils/Coord.h"
#include "Coord.h"
//#include "veins/base/utils/FindModule.h"
//#include "veins/modules/mobility/traci/TraCIConnection.h"
//using namespace std;
namespace Veins{

class VehInfo {

public:
    VehInfo();//: plane(new double[175][500]){};
    ~VehInfo();
     void update_pos( std::string objId, Coord pos, double speed, double angle);

protected:
    struct vehicle_bundle{
        std::string ObjId;
        Coord pos;
        double speed;
        double angle;
    };

//    vehicle_bundle plane[175][500];
    std::array< std::array<vehicle_bundle, 500>, 175> plane;

    
    bool entry_exists(std::string objId);
    int create_entry(std::string objId, Coord pos, double speed, double angle );
    bool validate_insert(std::string objId, Coord pos, double speed, double angle);
    bool check_neighbourhood(std::string objId, Coord pos, double speed, double angle);
    void report_collision(std::string objId1, std::string objId2, Coord pos , std::string type);
    double get_distance(Coord first, Coord second);
    Coord get_current_position(std::string ObjId);

    void set_neighbourhood(std::string objId, Coord pos);
    void clear_neighbourhood(std::string objId, Coord pos);
    void set_position(std::string objId, Coord pos, double speed, double angle);
    void clear_position(std::string objId, Coord pos);

    std::string check_collision(std::string objId, Coord pos, double speed, double angle);
    Coord cleanup_coords(Coord in);
    std::string check_possible_collision(std::string objId, Coord pos, double speed, double angle);
    void print_adjecent(int row_start, int row_end, int col_start, int col_end);

};

/*
struct <Name of struct> {
    members
} <instance name of struct>;
*/

}


#endif
