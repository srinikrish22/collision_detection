#ifndef _COORD_H
#define _COORD_H

#include <vector>

namespace Veins{

class Coord
{
public:
    /** @brief Constant with all values set to 0. */
    static const Coord ZERO;

public:
    /** @name x, y and z coordinate of the position. */
    /*@{*/
    double x;
    double y;
    double z;
    /*@}*/

private:
  void copy(const Coord& other) { x = other.x; y = other.y; z = other.z; }

public:
    /** @brief Default constructor. */
    Coord()
        : x(0.0), y(0.0), z(0.0) {}

    /** @brief Initializes a coordinate. */
    Coord(double x, double y, double z = 0.0)
        : x(x), y(y), z(z) {}

    /**
         * @brief Assigns coordinate vector 'other' to this.
         *
         * This operator can change the dimension of the coordinate.
         */
   Coord& operator=(const Coord& other) {
        if (this == &other) return *this;
        else{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;

//    std::cout << "\nIn equal: "<< this->x << " , "<<this->y<<" and " << other.x<<" ,"<<other.y<<std::endl; 
	return *this;
	//cObject::operator=(other);
        //copy(other);
        //return *this;
	}
   }
};
}
#endif
