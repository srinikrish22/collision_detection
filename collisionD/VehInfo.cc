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
//    cout << "\n0.85 rec cords: " << t_coords.x << " , " << t_coords.y<< " for: " << objId << endl;

    t2 = cleanup_coords(t_coords); //Performing cleaning up of the coords to prevent out of bound reading
    pos.y = t2.y;
    pos.x = t2.x;

//    cout << "0.9  Corrected Coords: " << pos.y << " and " << pos.x << endl;
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
        cout << "3.4is_nearby_okay: " << is_nearby_okay << endl;
        if(is_nearby_okay)
        {
            //Clear the old entry first
            Coord old_pos = get_current_position(objId);
//            cout<<"4.XGot the current postion:X "<< old_pos.x << " ,Y " << old_pos.y<<endl;
            clear_position(objId, old_pos);
            clear_neighbourhood(objId, old_pos);

            //Set the new entry
            set_position(objId, pos, speed, angle);
            set_neighbourhood(objId, pos);

            std::string poss_coll = check_possible_collision(objId, pos, speed, angle);
            cout << "6.0Check_possible_collition return: "<<poss_coll <<endl;
            if(poss_coll != "")
                report_collision(objId, poss_coll, pos, "possible");

            std::string act_coll = check_collision(objId, pos, speed, angle);
            cout << "6.0Check_collision return: "<< act_coll << " ..."<< endl;
            if(act_coll != "")
                report_collision(objId, act_coll, pos, "actual");


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
                cout << "\n1entry_exists returning true for: "<< objId << "at:X "<<col << ",Y "<<row <<endl;
                return true;
            }
        }
    }
    cout << "1entry_exists returning false for: " << objId  <<endl;
    return false;
}

int VehInfo::create_entry(std::string objId, Coord pos, double speed, double angle )
{
    //Creating the entry into the plane array and returns 0. Else returns a exit code and sets the error. Check the existing entry and then check the neighbourhood.
    //Only then the insertion of the object can take place.

    if ((plane[int (pos.y)][int (pos.x)].ObjId == ""))
    {
        cout << "5Inside create_entry, place empty for: "<< objId <<" at : " << pos.x << " , " << pos.y << endl;
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
//    cout << "3.0check_neighbourhood: " << objId << ":X " << pos.x << " ,Y " << pos.y << endl;
    int row, col,rrow,ccol;
    Coord t_coords={pos.x,pos.y,0.0},t2 ;

    for(col=(int(pos.x)-((V_COLLISION_BOUND_COL-1)/2)); col <= ( X_AXIS_HIGH-1 <= (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))? X_AXIS_HIGH-1 :  (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))); col++)
    {
        //cout << "3.01check_neighbourhood:COL " << col << "row " << row << endl;
        for(row = (int(pos.y)-((V_COLLISION_BOUND_ROW-1)/2)); row <= (Y_AXIS_HIGH-1 <= (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)? Y_AXIS_HIGH-1 : (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)))) ; row++  )
        {
           // cout << "\n3.02check_neighbou call cleanupRxC "<<row << ", "<< col <<endl;
                            
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
    cout << "3.2check_neighbourghood, return true for: "<< objId <<endl;
    return true;
}

std::string VehInfo::check_collision(std::string objId, Coord pos, double speed, double angle)
{
    int row, col;
    Coord t_coords={pos.x,pos.y,0.0}, t2;

    int col_start = (int(pos.x)-((V_COLLISION_BOUND_COL-1)/2)) ;
    int row_start = (int(pos.y)-((V_COLLISION_BOUND_ROW-1)/2)) ;
    int col_end = (X_AXIS_HIGH-1 <= (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))? X_AXIS_HIGH-1 : (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2)));
    int row_end = (Y_AXIS_HIGH-1 <= (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2))? Y_AXIS_HIGH-1 : (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)));

    cout << "X.Xcheck_col:X " << pos.x << " :Y "<< pos.y << " : " << col_end << " : " << row_end << endl;

    for( col = col_start ;col <= col_end ; col++ )
    {
        for(row = row_start ;row <= row_end ; row++)
        {
            cout <<"X.Xcheck_col found: " << objId<< "atX " << col << " ,Y " << row << endl;
           /* t2 = cleanup_coords(Coord(col,row,0.0)); //Performing cleaning up of the coords to prevent out of bound reading
            row = t2.y;
            col = t2.x;*/

            if(col == int(pos.x) && row == int(pos.y))
                continue;
            else if (plane[row][col].ObjId != "" && plane[row][col].ObjId != objId ) {
                //print_adjecent(row_start, row_end, col_start, col_end);
                return (plane[row][col].ObjId); //return true as the vehicle which will crash
            }
        }
    }
    return ""; //return false
}

std::string VehInfo::check_possible_collision(std::string objId, Coord pos, double speed, double angle){
    int row, col;
    Coord t_coords={pos.x,pos.y,0.0}, t2 ;

    int col_start = (int(pos.x) - ((V_COL_DETECT_BOUND_COL-1)/2));
    int row_start = (int(pos.y) - ((V_COL_DETECT_BOUND_ROW-1)/2));
    int col_end = (X_AXIS_HIGH-1 <= (int(pos.x) + ((V_COL_DETECT_BOUND_COL-1)/2))? X_AXIS_HIGH-1 : (int(pos.x) + ((V_COL_DETECT_BOUND_COL-1)/2))) ;
    int row_end = (Y_AXIS_HIGH-1 <= (int(pos.y) + ((V_COL_DETECT_BOUND_ROW-1)/2))? Y_AXIS_HIGH-1 : (int(pos.y) + ((V_COL_DETECT_BOUND_ROW-1)/2)));

    cout << "X.Xcheck_poss_col:X " << pos.x << " :Y "<< pos.y << " : " << col_end << " : " << row_end << endl;

    for( col = col_start ;col <= col_end ; col++ )
    {
        for(row = row_start ; row <= row_end ; row++)
        {
            /*t2 = cleanup_coords(Coord(col,row,0.0)); //Performing cleaning up of the coords to prevent out of bound reading
            row = t2.y;
            col = t2.x;*/
            cout <<"X.Xcheck_pos_col found: " << objId<< "atX " << col << " ,Y " << row << endl;
            if(col == int(pos.x) && row == int(pos.y))
                continue;
            else if (plane[row][col].ObjId != "" && plane[row][col].ObjId != objId ) {
               // print_adjecent(row_start, row_end, col_start, col_end);
                return (plane[row][col].ObjId); //return true as the vehicle which will crash
            }
        }
    }
    return ""; //return false
}

void VehInfo::set_neighbourhood(std::string objId, Coord pos){
    int row, col;
    Coord t_coords={pos.x,pos.y,0.0},t2 ;
//    cout << "7.1set_neighbourhood:X " << pos.x << " :Y "<< pos.y << " : " << ((V_COLLISION_BOUND_COL-1)/2) << " : " << ((V_COLLISION_BOUND_ROW-1)/2)<< endl;

    int col_start = (int(pos.x) - ((V_COLLISION_BOUND_COL-1)/2)) ;
    int row_start = (int(pos.y) - ((V_COLLISION_BOUND_ROW-1)/2)) ;
    int col_end = (X_AXIS_HIGH-1 <= (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))? X_AXIS_HIGH-1 : (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))) ;
    int row_end = (Y_AXIS_HIGH-1 <= (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2))? Y_AXIS_HIGH-1 : (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)));
//    cout <<"C: "<< col_start <<","<< col_end <<"\nR: "<< row_start<<"," << row_end << endl ;

    for(col = col_start ; col <= col_end ; col++)
    {
        for(row = row_start  ; row <= row_end ; row++)
        {
            /*t2= cleanup_coords(Coord(col,row,0.0)); //Performing cleaning up of the coords to prevent out of bound reading
            row = t2.y;
            col = t2.x;*/
//            cout <<"7.2set_neighbourhood setting for: " << objId<< "atX " << col << " ,Y " << row << endl;
            /*if(col == int(pos.x) && row == int(pos.y))
                continue;
            else*/
                plane[row][col].ObjId = objId;
         }
    }
}

void VehInfo::clear_neighbourhood(std::string objId, Coord pos){
    int row, col;
    Coord t_coords={pos.x,pos.y,0.0} ,t2;
    //cout << "6clear_neighbourhood:X " << pos.x << " :Y "<< pos.y<<" Obj "<<objId << endl;

    int col_start = (int(pos.x) - ((V_COLLISION_BOUND_COL-1)/2)) ;
    int row_start = (int(pos.y) - ((V_COLLISION_BOUND_ROW-1)/2)) ;
    int col_end = (X_AXIS_HIGH-1 <= (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2))? X_AXIS_HIGH-1 : (int(pos.x) + ((V_COLLISION_BOUND_COL-1)/2)));
    int row_end = (Y_AXIS_HIGH-1 <= (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2))? Y_AXIS_HIGH-1 : (int(pos.y) + ((V_COLLISION_BOUND_ROW-1)/2)));
    //cout <<"6.05clear_neighC: "<< col_start <<","<< col_end <<"\nR: "<< row_start<<"," << row_end << endl ;

    for(col = col_start ; col <= col_end ; col++)
    {
        for(row = row_start ; row <= row_end ; row++)
        {
            /*t_coords = cleanup_coords(Coord(col,row,0.0)); //Performing cleaning up of the coords to prevent out of bound reading
            row = t2.y;
            col = t2.x;*/
            if(col == int(pos.x) && row == int(pos.y))
                continue;
            else
            {
               // cout<< "6.1clear_neighborhood clearing at:X "<<col << ",Y " << row << endl;
                plane[row][col].ObjId = "";
            }
         }
    }
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
                cout << "4get_curr_pos : "<<objId <<" at:X "<<ret.x << " ,Y "<<ret.y << endl;
                return ret;
            }
        }
    }
    return ret;
}


void VehInfo::set_position(std::string objId, Coord pos, double speed, double angle){\
//    cout << "9set_pos:"<<objId<<" atX " << pos.x<<" ,Y "<<pos.y << endl;
    plane[int(pos.y)][int(pos.x)].ObjId = objId;
    plane[int(pos.y)][int(pos.x)].speed = speed;
    plane[int(pos.y)][int(pos.x)].angle = angle;
}

void VehInfo::clear_position(std::string objId, Coord pos){
//    cout << "5clear_pos:"<<objId<<" atX " << pos.x<<" ,Y "<<pos.y << endl;
    plane[int(pos.y)][int(pos.x)].ObjId = "";
    plane[int(pos.y)][int(pos.x)].speed = 0.0;
    plane[int(pos.y)][int(pos.x)].angle = 0.0;
}

void VehInfo::report_collision(std::string objId1, std::string objId2, Coord pos, std::string type )
{
    if(type == "actual")
        std::cout<<"\nTHERE IS A COLLISION!! for vehicle: "<<objId1<<" with "<<objId2 <<" at position: ("<<pos.x<<" , "<< pos.y<<" )"<<endl<<endl;

    else if(type == "possible")
        std::cout<<"\nTHERE MIGHT BE A COLLISION!! for vehicle: "<<objId1<<" with "<<objId2 <<" at position: ("<<pos.x<<" , "<< pos.y<<" )"<<endl<<endl;
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
//        cout << "X.X Coord cleanup recv: "<<row<<" and "<<col<<" Ret: " << ret.y << " and " << ret.x << endl; 
//        cout << "X.X HighLowsX,Y: " << X_AXIS_LOW <<"\t"<< X_AXIS_HIGH <<"\t"<< Y_AXIS_LOW<<"\t" << Y_AXIS_HIGH << endl;
         return ret;
 }

bool VehInfo::validate_insert(std::string objId, Coord pos, double speed, double angle ){return true;}

void VehInfo::print_adjecent(int row_start, int row_end, int col_start, int col_end){

    for(int row = row_start ; row<= row_end; row++ )
    {
        for(int col = col_start ; col <= col_end ; col++)
        {
            cout << plane[col][row].ObjId <<"\t\t";
        }
        cout<<endl;
    }
}



}//Namespace


//int main(){return 0;}<
