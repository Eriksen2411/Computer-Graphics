//============================================================
// STUDENT NAME: Nguyen Duc Danh
// NUS User ID.: e0407670
// COMMENTS TO GRADER:
//
// ============================================================

#include <cmath>
#include "Sphere.h"

using namespace std;



bool Sphere::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    // Now the circle equation is (x - center)^2 + (y - center)^2 + (z - center)^2 - r^2 = 0
    double a = dot(r.direction(), r.direction());
    double b = 2 * dot(r.direction(), r.origin() - center);
    double c = dot(r.origin() - center, r.origin() - center) - (radius * radius);

    double d = (b * b) - (4 * a * c);
    if (d >= 0) {
        double temp = min((-b - sqrt(d)) / (2 * a), (-b + sqrt(d)) / (2 * a));
        // Set t to be the smaller value if the smaller value is > 0,
        // Else, set t to be the larger value, will check > 0 with condition (t >= tmin && t <= tmax)
        double t = (temp > 0) ? temp : max((-b - sqrt(d)) / (2 * a), (-b + sqrt(d)) / (2 * a));
        if (t >= tmin && t <= tmax) {
            rec.t = t;
            rec.p = r.pointAtParam(t);
            Vector3d N = r.pointAtParam(t) - center;
            rec.normal = N / N.length();
            rec.material = material;
            return true;
        }
    }
    return false;  // YOU CAN REMOVE/CHANGE THIS IF NEEDED.
}



bool Sphere::shadowHit( const Ray &r, double tmin, double tmax ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    // Now the circle equation is (x - center)^2 + (y - center)^2 + (z - center)^2 - r^2 = 0
    double a = dot(r.direction(), r.direction());
    double b = 2 * dot(r.direction(), r.origin() - center);
    double c = dot(r.origin() - center, r.origin() - center) - (radius * radius);

    double d = b * b - 4 * a * c;
    if (d >= 0) {
        double temp = min((-b - sqrt(d)) / (2 * a), (-b + sqrt(d)) / (2 * a));
        // Set t to be the smaller value if the smaller value is > 0,
        // Else, set t to be the larger value, will check > 0 with condition (t >= tmin && t <= tmax)
        double t = (temp > 0) ? temp : max((-b - sqrt(d)) / (2 * a), (-b + sqrt(d)) / (2 * a));
        return (t >= tmin && t <= tmax);
    }
    else {
        return false;
    }
}
