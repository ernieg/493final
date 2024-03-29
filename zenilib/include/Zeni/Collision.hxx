/* This file is part of the Zenipex Library.
* Copyleft (C) 2011 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#ifndef ZENI_COLLISION_HXX
#define ZENI_COLLISION_HXX

#include <Zeni/Collision.h>

#include <Zeni/Global.h>

namespace Zeni {
  
  namespace Collision {

    float Sphere::shortest_distance(const Plane &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Sphere::shortest_distance(const Line &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Sphere::shortest_distance(const Ray &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Sphere::shortest_distance(const Line_Segment &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Sphere::shortest_distance(const Infinite_Cylinder &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Sphere::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Sphere::shortest_distance(const Parallelepiped &rhs) const {
      return rhs.shortest_distance(*this);
    }

    template <typename TYPE>
    bool Sphere::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Plane::shortest_distance(const Line &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Plane::shortest_distance(const Ray &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Plane::shortest_distance(const Line_Segment &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Plane::shortest_distance(const Infinite_Cylinder &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Plane::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Plane::shortest_distance(const Parallelepiped &rhs) const {
      return rhs.shortest_distance(*this);
    }

    template <typename TYPE>
    bool Plane::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Line_Segment::shortest_distance(const Sphere &rhs) const {
      return nearest_point(rhs).first;
    }
    float Line_Segment::shortest_distance(const Zeni::Point3f &rhs) const {
      return nearest_point(rhs).first;
    }
    float Line_Segment::shortest_distance(const Plane &rhs) const {
      return nearest_point(rhs).first;
    }
    float Line_Segment::shortest_distance(const Line &rhs) const {
      return nearest_point(rhs).first;
    }
    float Line_Segment::shortest_distance(const Ray &rhs) const {
      return nearest_point(rhs).first;
    }
    float Line_Segment::shortest_distance(const Line_Segment &rhs) const {
      return nearest_point(rhs).first;
    }
    float Line_Segment::shortest_distance(const Infinite_Cylinder &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Line_Segment::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Line_Segment::shortest_distance(const Parallelepiped &rhs) const {
      return nearest_point(rhs).first;
    }

    template <typename TYPE>
    bool Line_Segment::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Ray::shortest_distance(const Sphere &rhs) const {
      return nearest_point(rhs).first;
    }
    float Ray::shortest_distance(const Zeni::Point3f &rhs) const {
      return nearest_point(rhs).first;
    }
    float Ray::shortest_distance(const Plane &rhs) const {
      return nearest_point(rhs).first;
    }
    float Ray::shortest_distance(const Line &rhs) const {
      return nearest_point(rhs).first;
    }
    float Ray::shortest_distance(const Ray &rhs) const {
      return nearest_point(rhs).first;
    }
    float Ray::shortest_distance(const Line_Segment &rhs) const {
      return nearest_point(rhs).first;
    }
    float Ray::shortest_distance(const Infinite_Cylinder &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Ray::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Ray::shortest_distance(const Parallelepiped &rhs) const {
      return nearest_point(rhs).first;
    }

    template <typename TYPE>
    bool Ray::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Line::shortest_distance(const Ray &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Line::shortest_distance(const Infinite_Cylinder &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Line::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }

    template <typename TYPE>
    bool Line::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Infinite_Cylinder::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Infinite_Cylinder::shortest_distance(const Parallelepiped &rhs) const {
      return rhs.shortest_distance(*this);
    }

    template <typename TYPE>
    bool Infinite_Cylinder::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Capsule::shortest_distance(const Sphere &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Zeni::Point3f &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Plane &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Line &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Ray &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Line_Segment &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Infinite_Cylinder &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Capsule &rhs) const {
      return nearest_point(rhs).first;
    }
    float Capsule::shortest_distance(const Parallelepiped &rhs) const {
      return nearest_point(rhs).first;
    }

    template <typename TYPE>
    bool Capsule::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

    float Parallelepiped::shortest_distance(const Line &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Parallelepiped::shortest_distance(const Ray &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Parallelepiped::shortest_distance(const Line_Segment &rhs) const {
      return rhs.shortest_distance(*this);
    }
    float Parallelepiped::shortest_distance(const Capsule &rhs) const {
      return rhs.shortest_distance(*this);
    }

    template <typename TYPE>
    bool Parallelepiped::intersects(const TYPE &rhs) const {
      return shortest_distance(rhs) < ZENI_COLLISION_EPSILON;
    }

  }

}

#include <Zeni/Global_Undef.h>

#endif
