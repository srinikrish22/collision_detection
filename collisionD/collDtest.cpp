#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>

#include"Coord.h"
#include "VehInfo.h"

#define PLANE_COL 50
#define PLANE_ROW 50

#define CYC 88
#define CAR 99

#define Y_AXIS_LOW 0
#define X_AXIS_LOW 0
#define Y_AXIS_HIGH 174
#define X_AXIS_HIGH 499

using namespace Veins;
using namespace boost;
using namespace std;

int plane[PLANE_ROW][PLANE_COL]={0};

Coord cleanup_coords(Coord in)
{
	Coord ret;
        if(in.y <= Y_AXIS_LOW)
	ret.y = 0.0;

	else if(in.y >= Y_AXIS_HIGH)
	ret.y = Y_AXIS_HIGH - 1.0 ;

	else
	ret.y = in.y - Y_AXIS_LOW;
///
        if(in.x <= X_AXIS_LOW)
	ret.x = 0.0;

	else if(in.x >= X_AXIS_HIGH)
	ret.x = X_AXIS_HIGH - 1.0;

	else
	ret.x = in.x - X_AXIS_LOW;
	
	ret.z = 0.0;

	return ret;
}

void print_plane()
{
	int col,row;
	for (row = 0; row < PLANE_ROW; row++)
    	{
        	for(col = 0 ; col < PLANE_COL ; col++ )
       		{
			if(plane[row][col] == 0)	
                	cout << plane[row][col] << "  ";
			else
			cout << plane[row][col] << " ";
        	}
        	cout << "\n";
	}

}

void insert_into_plane(Coord in, int veh)
{
	plane[int(in.y)][int(in.x)] = veh;
//	set_neighbours(in,);
}

void clear_coords(Coord in)
{ insert_into_plane(in,0); }

int main () {
  Coord cur_pos,old_pos,pass;
  std::vector <string> fields;
  std::string line;
  double conv = 0.0;
  std::ifstream myfile ("example12.txt");
  int row, col;

/*  for (row = 0; row < PLANE_ROW; row++)
  {	
        for(col = 0 ; col < PLANE_COL ; col++ )
        {
                plane[row][col] = 0;
        }
   }
*/
////////This part will test the array printing part////////
/*   cur_pos.x = 262.954;
   cur_pos.y = 108.25;
   old_pos.x = 0.0;
   old_pos.y = 0.0;

 for(int ver = 0 ; ver < 10 ; ver ++ )
 {
   clear_coords(old_pos);

   pass = cleanup_coords(cur_pos);
   old_pos.y = pass.y;
   old_pos.x = pass.x;

   insert_into_plane(pass,CYC);
   print_plane();

   cur_pos.x++;
   cur_pos.y++;
   usleep(500000);
 }
*/   
//////////END///////////
////////This part is the file reading and handling of the coordinates////////
std::unique_ptr<VehInfo> vehicle(new VehInfo());
//Coord old = {0.1,0.2,0.3}, newer = {1.1,2.2,3.3} ; // part of testing the = operator overloading
//old = newer;//this == other
//cout << "\nold after:" << old.x << "," << old.y << std::endl;

  int count = 0;
  if (myfile.is_open())
  {
    while ( getline (myfile,line) && count <= 100 )
    {
     	if(line != "EOF" && line!= "")
	{
//     		std::cout << line << '\n';
	     	split(fields, line, is_any_of(","));
		
/*		for(int i = 0; i < fields.size();i++ )
		{	
			cout << fields[i] << "\t";
		//	usleep(500000);
		}
		cout << endl;
*/		
		cur_pos.x = std::stod(fields[1]);
                cur_pos.y = std::stod(fields[2]);
		cur_pos.z = 0.0;
		double speed =  std::stod(fields[3]);
		double angle =  std::stod(fields[4]);

		vehicle->update_pos(fields[0],cleanup_coords(cur_pos), speed, angle );

     	}
     	else if( line == "EOF")
	break;
	
	else
    	continue;

	count++;
    }
    myfile.close();
  }
  else std::cout << "Unable to open file"; 
////////END////////
  return 0;
}


