#include "trajectory.h"

#include <stdint.h>

uint32_t* trajectories[NTRAJECTORIES] = {
    NULL,
    traj1, traj2, traj3, traj4, traj5,
    traj6, traj7, traj8, traj9, traj10,
    traj11, traj12, traj13, traj14, traj15,
    traj16, traj17, traj18, traj19, traj20,
    traj21, traj22, traj23, traj24, traj25,
    traj26, traj27, traj28, traj29, traj30,
    traj31, traj32, traj33, traj34, traj35,
    traj36, traj37, traj38, traj39, traj40,
    traj41, traj42, traj43, traj44, traj45,
    traj46, traj47, traj48, traj49, traj50,
    traj51
};

for (unsigned i = 0; i < 17; ++i) {
    for( unsigned j = 0; j < 10; ++j ) {
        Coords coords;
        if(32 * i - 32 * j + 30 < 0){
            coords.x = 65536
        } else {
            coords.x = 32 * i - 32 * j + 25;
        }
        double temp = 235 + 32 * 1.73 * j;
        if( temp > 480 ){
            temp = 65536
        }
        coords.y = int32_t(temp);
        trajectories[i+1][j] = coords;
    }
}

for (unsigned i = 0; i < 17; ++i) {
    for( unsigned j = 0; j < 10; ++j) {
        Coords coords;
        if(32 * i + 64 * j + 90 > 640) {
            coords.x = 65536;
        } else {
            coords.x = 32 * i + 64 * j + 90;
        }
        coords.y = 315;
        trajectories[i+18][j] = coords;
    }
}

for (unsigned i = 0; i < 17; ++i) {
    for( unsigned j = 0; j < 10; ++j) {
        Coords coords;
        if (32 * i - 32 * j - 15 < 0) {
            coords.x = 65536;
        } else {
            coords.x = 32 * i - 32 * j - 15;
        }
        double temp = 235 + 32 * 1.73 * j;
        if (temp > 480) {
            temp = 65536;
        }
        coords.y = int32_t(temp);
        trajectories[i+35][j] = coords;
    }
}