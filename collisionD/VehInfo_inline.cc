#include "VehInfo.h"

using namespace std;
namespace Veins{
//Register_Class(VehInfo);

VehInfo::VehInfo()
    : plane()
{
    cout << "\n0.0Inside the constructor of VehInfo"<<endl;
   // Coord(); // Creates and the it gets discarded. So its useless.
    for (int j = 0; j < 500; j++)
    {
        for(int i = 0 ; i < 175 ; i++ )
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

VehInfo::~VehInfo() {}

void VehInfo::update_pos(std::string objId, Coord pos, double speed, double angle)
{       
    //cout << "\n0.8  Received Coords: " << pos.y << " and " << pos.x << endl;
    Coord t_coords = {pos.x,pos.y,0.0}, t2;
    cout << "0.85 rec cords: " << t_coords.x << " , " << t_coords.y<<endl;

    t2 = cleanup_coords(t_coords); //Performing cleaning up of the coords to prevent out of bound reading
    pos.y = t2.y;
    pos.x = t2.x;

    cout << "0.9  Corrected Coords: " << pos.y << " and " << pos.x << endl;
    if(!entry_exists(objId))
    {   
        cout << "2Inside updte_pos entry not exists,Obj: "<< objId << endl;
        //if already there is no entry of the object, then create one entry.
        bool is_nearby_okay = check_neighbourhood(objId, pos, speed, angle);
        cout << "4 is_nearby_okay: " << is_nearby_okay <<endl;
        if(is_nearby_okay)
        {
            int ret = create_entry(objId, pos, speed,angle);
            if(ret != 0)
            {
                std::cout << "There was a problem inserting: "<< objId << std::endl ;
            }
            set_neighbourhood(objId, pos);
        }
    }

    else
    {   
                cout << "3Inside updte_pos entry exists,Obj: "<< objId << endl;
        //First check nearby if okay, update. Else revert.
        bool is_nearby_okay = check_neighbourhood(objId, pos, speed, angle);
        if(is_nearby_okay)
        {
            //Clear the old entry first
            Coord old_pos = get_current_position(objId);
            clear_position(objId, old_pos);
            clear_neighbourhood(objId, old_pos);

            //Set the new entry
            set_position(objId, pos, speed, angle);
            set_neighbourhood(objId, pos);
            bool is_collision_poss = check_collision(objId, pos, speed, angle);
            if(is_collision_poss)
                report_collision(objId, "Something", pos);
        }
    }
}

bool VehInfo::entry_exists(std::string objId)
{
    for(int row = 0 ; row <  175 ; row++ )
    {
        for (int col = 0; col < 500; col++)
        {
            if (plane[row][col].ObjId == objId)
            {
                cout << "1Inside entry_exists returning true for: "<< plane[row][col].ObjId.size() <<".."<< objId <<endl;
                return true;
            }
        }
    }
//    cout << "2Inside entry_exists returning false for: " << plane[row][col].ObjId.size() <<".."<<plane[row][col].speed<<".."<<plane[row][col].angle <<".."<< objId <<endl;
    cout << "1Inside entry_exists returning false for: " << objId <<endl;
    return false;
}

int VehInfo::create_entry(std::string objId, Coord pos, double speed, double angle )
{
    //Creating the entry into the plane array and returns 0. Else returns a exit code and sets the error. Check the existing entry and then check the neighbourhood.
    //Only then the insertion of the object can take place.

    if ((plane[int (pos.y)][int (pos.x)].ObjId == ""))
    {
        cout << "5Inside create_entry, place empty for: "<< objId <<endl;
        plane[int(pos.y)][int(pos.x)].ObjId = objId;
        plane[int(pos.y)][int(pos.x)].speed = speed;
        plane[int(pos.y)][int(pos.x)].angle = angle;
        return 0;
    }
    else
    {
        std::perror("The neighbourhood is not open enough to insert object: ");//+ objId);
        return 1;
    }
}

bool VehInfo::check_neighbourhood(std::string objId, Coord pos, double speed, double angle){
//(a>b)?b : a
    int row, col,rrow,ccol; Coord t_coords={pos.x,pos.y,0.0},t2 ;
    for(col = (int(pos.x) - ((V_COLLISION_BOUND_COL-1)/2)) ; col++ ; col <=( X_AXIS_HIGH < (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))? X_AXIS_HIGH :  (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2)) ))
    {
        for(row = (int(pos.y) - ((V_COLLISION_BOUND_ROW-1)/2)) ; row++ ; row <=(Y_AXIS_HIGH < (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)? Y_AXIS_HIGH : (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)) )))
        {
            cout << "\n3.01check_neighbou call cleanupRxC "<<row << ", "<< col <<endl;
                            
            t2 = cleanup_coords(Coord(col,row,0.0)); //Performing cleaning up of the coords to prevent out of bound reading
            rrow = t2.y;
            ccol = t2.x;
            cout << "3.0check_neighbou Coorected Coords RxC : " << row << " and " << col << endl;
            if(ccol == int(pos.x) && rrow == int(pos.y))
                continue;
            else if (plane[rrow][ccol].ObjId != "" && plane[rrow][ccol].ObjId != objId ) 
            {
                cout << "3.1check_neighbou ELSE Objid found: " << plane[rrow][ccol].ObjId.size() << endl;
                cout << "3.2check_neighbou ELSE return false for: "<< objId <<endl;
                return false;
            }

        }
    }
    cout << "3.2Inside check_neighbourghood, return true for: "<< objId <<endl;
    return true;
}

bool VehInfo::check_collision(std::string objId, Coord pos, double speed, double angle)
{
    int row, col; 
    Coord t_coords={pos.x,pos.y,0.0}, t2 ;
        t2 = cleanup_coords(t_coords); //Performing cleaning up of the coords to prevent out of bound reading
        row = t2.y;
        col = t2.x;
        for(col = (int(pos.x) - ((V_COL_DETECT_BOUND_COL-1)/2)) ; col++ ; col <= (int(pos.x) + ((V_COL_DETECT_BOUND_COL-1)/2)))
        {
            for(row = (int(pos.y) - ((V_COL_DETECT_BOUND_ROW-1)/2)) ; row++ ; row <= (int(pos.y) + ((V_COL_DETECT_BOUND_ROW-1)/2)))
            {
                if(col == int(pos.x) && row == int(pos.y))
                    continue;
                else if (plane[row][col].ObjId != "" && plane[row][col].ObjId != objId ) {
                    return true;
                }

            }
        }
        return false;
}

double VehInfo::get_distance(Coord first, Coord second){
   double ret = std::sqrt((std::pow((second.x-first.x), 2) + std::pow((second.y-first.y), 2)));
   return ret;
}

Coord VehInfo::get_current_position(std::string objId){

    int row,col;
    Coord ret;
    ret.x = -1;
    ret.y = -1;
    ret.z = -1;

    for (col = 0; col <= 500; col++)
    {
        for(row = 0 ; row <=  175 ; row++ )
        {
            if (plane[row][col].ObjId == objId)
            {
                ret.x = col;
                ret.y = row;
                ret.z = 0.0;

                return ret;
            }
        }
    }
    return ret;
}

void VehInfo::set_neighbourhood(std::string objId, Coord pos){
    int row, col; Coord t_coords={pos.x,pos.y,0.0},t2 ;
        t_coords= cleanup_coords(t_coords); //Performing cleaning up of the coords to prevent out of bound reading
        row = t2.y;
        col = t2.x;
    for(col = (int(pos.x) - ((V_COLLISION_BOUND_COL-1)/2)) ; col++ ; col <= (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2)))
    {
        for(row = (int(pos.y) - ((V_COLLISION_BOUND_ROW-1)/2)) ; row++ ; row <= (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)))
        {
            if(col == int(pos.x) && row == int(pos.y))
                continue;
            else
                plane[row][col].ObjId = objId;
         }
    }
}

void VehInfo::clear_neighbourhood(std::string objId, Coord pos){
    int row, col; Coord t_coords={pos.x,pos.y,0.0} ,t2;
        t_coords = cleanup_coords(t_coords); //Performing cleaning up of the coords to prevent out of bound reading
        row = t2.y;
        col = t2.x;
        for(col = (int(pos.x) - ((V_COLLISION_BOUND_COL-1)/2)) ; col++ ; col <= (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2)))
        {
            for(row = (int(pos.y) - ((V_COLLISION_BOUND_ROW-1)/2)) ; row++ ; row <= (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)))
            {
                if(col == int(pos.x) && row == int(pos.y))
                    continue;
                else
                    plane[row][col].ObjId = "";
             }
        }
}

void VehInfo::set_position(std::string objId, Coord pos, double speed, double angle){
    plane[int(pos.y)][int(pos.x)].ObjId = objId;
    plane[int(pos.y)][int(pos.x)].speed = speed;
    plane[int(pos.y)][int(pos.x)].angle = angle;
}

void VehInfo::clear_position(std::string objId, Coord pos){
    plane[int(pos.y)][int(pos.x)].ObjId = "";
    plane[int(pos.y)][int(pos.x)].speed = 0.0;
    plane[int(pos.y)][int(pos.x)].angle = 0.0;
}

void VehInfo::report_collision(std::string objId1, std::string objId2, Coord pos )
{
    std::cout<<"\nThere will be a collision for vehicle: "<<objId1<<" at position: ("<<pos.x<<" , "<< pos.y<<" )";
}

Coord VehInfo::cleanup_coords(Coord in) //return is an array as (Ry(0),Cx(1))
 {                         //174    241
         Coord ret = {0.0,0.0,0.0}; 
         int col = int(in.x);
         int row = int(in.y);
         if(col <= X_AXIS_LOW)
         ret.x= 0.0;
 
         else if(col >= X_AXIS_HIGH)
         ret.x = X_AXIS_HIGH;
 
         else
         ret.x = col - X_AXIS_LOW;
///
         if(row <= Y_AXIS_LOW)
         ret.y = 0;
 
         else if(row >= Y_AXIS_HIGH)
         ret.y = Y_AXIS_HIGH;
  
          else
          ret.y = row - Y_AXIS_LOW;
        cout << "X.X Coord cleanup recv: "<<row<<" and "<<col<<" Ret: " << ret.y << " and " << ret.x << endl; 
//        cout << "X.X HighLowsX,Y: " << X_AXIS_LOW <<"\t"<< X_AXIS_HIGH <<"\t"<< Y_AXIS_LOW<<"\t" << Y_AXIS_HIGH << endl;
         return ret;
 }

bool VehInfo::validate_insert(std::string objId, Coord pos, double speed, double angle ){return true;}



}//Namespace


//int main(){return 0;}<
