#include <cmath>
#include "geoparse.h"
#include <iostream>
using namespace std;

const int R = 1;

Point geoPointFromLatLong(float latitude, float latMin, WorldSide latSide, float longtitude, float longMin, WorldSide longSide)
{
    float radLat = (latitude + latMin/60 ) / 180 * static_cast<float>(M_PI);
    if(latSide == WorldSide::SOUTH) radLat = -radLat;
    cout << "rad lat = " << radLat << endl;
    float radLong = (longtitude - 180 + longMin/60) / 180 * static_cast<float>(M_PI);
    if(longSide == WorldSide::WEST) radLong = -radLong;
    cout << "rad long = " << radLong << endl;
    float x = -R * cosf(radLat) * cosf(radLong);
    float z = R * cosf(radLat) * sinf(radLong);
    float y = R * sinf(radLat);
//    float x = (R * sinf(radLat)*cosf(radLong));
//    float z = (R * sinf(radLat)*sinf(radLong));
//    float y = (R * cosf(radLat));
    cout << "Coordinates " << x << " " << y << " "<< z << endl;
    return Point{x,y,z};

//    float phi   = (90-latitude)*(M_PI/180);
//    float theta = (longtitude+180)*(M_PI/180);

//    float x = (R * sinf(phi)*cosf(theta));
//    float z = (R * sinf(phi)*sinf(theta));
//    float y = (R * cosf(phi));

//    cout << "Coordinates " << x << " " << y << " "<< z << endl;
//    return Point{x,y,z};

//    float cosLat = cosf(latitude * M_PI / 180.0);
//    float sinLat = sinf(latitude * M_PI / 180.0);
//    float cosLon = cosf(longtitude * M_PI / 180.0);
//    float sinLon = sinf(longtitude * M_PI / 180.0);
//    float x = R * cosLat * cosLon;
//    float y = R * cosLat * sinLon;
//    float z = R * sinLat;
//    cout << "Coordinates " << x << " " << y << " "<< z << endl;
//    return Point{x,y,z};
}

float arcDistance(Point a, Point b)
{
    auto dist = static_cast<float>(sqrt(pow(a.x-b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2))); //direct distance between points
    cout << "dist = "<<dist<<endl;
    float angle = 2*asin(dist/2/R);   //angle between 2 points
    cout << "angle = " << angle << endl;
    return R*angle;
}
