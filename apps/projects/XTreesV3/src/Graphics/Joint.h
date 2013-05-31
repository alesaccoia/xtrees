/*
 *  Joint.h
 *  FluidExperiments
 *
 *
 *  https://sites.google.com/site/ofauckland/examples/17-cloth-physics
 *
 */


#ifndef __JOINT_H
#define __JOINT_H

#include "ofMain.h"

struct Joint {
  ofVec3f pos,vel,forces;
  float mass,inverse_mass,drag;

  Joint(ofVec3f pos, float mass, float drag = .96) : pos(pos), mass(mass), drag(drag) {
      if (mass==0.0f) inverse_mass = 0;
      else if (mass<0.001) mass=0.001;
      if (mass!=0.0f) inverse_mass = 1/mass;
  }

  void update() {
      forces *= inverse_mass;
      vel += forces;
      forces = 0;
      vel.limit(15);
      pos += vel;
      vel *= drag;
  }

  void addForce(ofVec3f oForce) {
      forces += oForce;
  }
};

#endif //__JOINT_H