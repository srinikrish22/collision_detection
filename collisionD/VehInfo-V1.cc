#include "VehInfo.h"

//using Veins::VehInfo;i
using namespace std;
namespace Veins{
//Register_Class(VehInfo);


VehInfo::VehInfo()
    : plane({})
//: plane({{}, "", (0, 0) , 0.0, 0.0})
{
    Coord();
    for (int i = 0; i <= 500; i++)
    {
        for(int j = 0 ; j <=  175 ; j++ )
        {
            plane[i][j].ObjId = "";
            plane[i][j].pos.x = 0.0;
            plane[i][j].pos.y = 0.0;
            plane[i][j].pos.z = 0.0;
            plane[i][j].speed = -1;
            plane[i][j].angle = 0.0;
        }
    }
}

VehInfo::~VehInfo()
{
    for (int i = 0; i <= 500; i++)
        {
            for(int j = 0 ; j <=  175 ; j++ )
            {
                plane[i][j].ObjId = "";
                plane[i][j].pos.x = 0.0;
                plane[i][j].pos.y = 0.0;
                plane[i][j].pos.z = 0.0;
                plane[i][j].speed = -1;
                plane[i][j].angle = 0.0;
            }
        }
}

void VehInfo::update_pos(std::string objId, Coord pos, double speed, double angle)
{
    if(!entry_exists(objId)) //if already there is no entry of the object, then create one entry.
    {
        int ret = create_entry(objId, pos, speed,angle);
        if(ret != 0)
        {
            std::cout << "There was a problem inserting: "<< objId << std::endl ;
        }


    }
    else
    {   //make changes in the array. Then check for collision. else revert.

        //validate_insert(objId, pos, speed, angle);
        if(check_neighbourhood(objId, pos, speed, angle)){
            plane[int(pos.y)][int(pos.x)].ObjId = objId;
            plane[int(pos.y)][int(pos.x)].speed = speed;
            plane[int(pos.y)][int(pos.x)].angle = angle;
        }

        bool neighbourhood_check = check_neighbourhood(objId, pos, speed, angle);
        bool collision_check = check_collision();

        if(collision_check){
            handle_collision(objId);
        }
    }
}

bool VehInfo::entry_exists(std::string objId)
{
    int i,j;

        for (i = 0; i <= 500; i++)
        {
            for(j = 0 ; j <=  175 ; j++ )
            {
                if (plane[j][i].ObjId == objId)
                {
                    return true;
                }

            }
        }
        return false;
}
int VehInfo::create_entry(std::string objId, Coord pos, double speed, double angle )
{
    //Creating the entry into the plane array and returns 0. Else returns a exit code and sets the error.
    //checking the existing entry and then check the neighbourhood. Only then the insertion of the object can take place.

    if ((plane[int (pos.y)][int (pos.x)].ObjId == "")) //&& (check_neighbourhood(objId, pos, speed,angle)))
    {
        plane[int(pos.y)][int(pos.x)].ObjId = objId;
        plane[int(pos.y)][int(pos.x)].speed = speed;
        plane[int(pos.y)][int(pos.x)].angle = angle;

        return 0;
    }
    else//
    {
        std::perror("The neighbourhood is not open enough to insert object: ");//+ objId);
        return 1;
    }
}
bool VehInfo::check_collision(){}
bool VehInfo::check_neighbourhood(std::string objId, Coord pos, double speed, double angle){

    int car_colbound_rows = 3;
    int car_colbound_column = 3;

    int car_CDbound_rows = 5;
    int car_CDbound_column = 5;

    int r_iterator;// = int(pos.y) - ((car_boundary_rows-1)/2);
    int c_iterator;//= int(pos.x) - ((car_boundary_rows-1)/2);

    for(c_iterator = (int(pos.x) - ((car_colbound_rows-1)/2)) ; c_iterator++ ; c_iterator <= (int(pos.x) + ((car_colbound_rows-1)/2))){

        for(r_iterator = (int(pos.y) - ((car_colbound_rows-1)/2)) ; r_iterator++ ; r_iterator <= (int(pos.y) + ((car_colbound_rows-1)/2))){

            if(c_iterator == int(pos.x) && r_iterator == int(pos.y))
                continue;
            else if (plane[r_iterator][c_iterator].ObjId != "") {

                return false;
            }

        }
    }

    return true;
}
void VehInfo::handle_collision(std::string objId){}
bool VehInfo::validate_insert(std::string objId, Coord pos, double speed, double angle){return true;}

double VehInfo::get_distance(Coord first, Coord second){
   double ret = std::sqrt((std::pow((second.x-first.x), 2) + std::pow((second.y-first.y), 2)));
   return ret;
}

Coord VehInfo::get_current_position(std::string objId){

    int i,j;
    Coord ret;
    ret.x = -1;
    ret.y = -1;
    ret.z = -1;

    for (i = 0; i <= 500; i++)
    {
        for(j = 0 ; j <=  175 ; j++ )
        {
            if (plane[j][i].ObjId == objId)
            {
                ret.x = i;
                ret.y = j;
                ret.z = 0.0;

                return ret;
            }
        }
    }
    return ret;
}

}

int main(){return 0;}
