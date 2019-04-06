#ifndef GEOPARSE_H
#define GEOPARSE_H

enum WorldSide
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

struct Point
{
    float x;
    float y;
    float z;
};

Point geoPointFromLatLong(float latitude, float latMin, WorldSide latSide, float longtitude, float longMin,  WorldSide longSide);
float arcDistance(Point a, Point b);

#endif // GEOPARSE_H
