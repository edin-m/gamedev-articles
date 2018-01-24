#ifndef BVH_H
#define BVH_H

#include <vector>

#include "glm/glm.hpp"

namespace k {

typedef struct {
    float x, y, z;
} OFFSET;

typedef struct JOINT JOINT;

struct JOINT {
    // joint name
    const char* name = NULL;

    // joint parent
    JOINT* parent = NULL;

    // offset data
    OFFSET offset;

    // num of channels joint has
    unsigned int num_channels = 0;

    // ordered list of channels
    short* channels_order = NULL;

    // joint's children
    std::vector<JOINT*> children;

    // local transofrmation matrix (premultiplied with parents'
    glm::mat4 matrix;

    // index of joint's channel data in motion array
    unsigned int channel_start = 0;
};

typedef struct {
    JOINT* rootJoint;
    int num_channels;
} HIERARCHY;

typedef struct {
    unsigned int num_frames;
    unsigned int num_motion_channels = 0;
    float* data = NULL;
    unsigned* joint_channel_offsets;
} MOTION;

typedef struct {
    unsigned int num_hierarchies;
    unsigned int num_total_joints;
    unsigned int num_motion_channels = 0;
    unsigned int num_frames;

} bvh_statistics;

/**
 * @brief The Bvh class
 */
class Bvh {
public:
    Bvh();
    ~Bvh();

    void printJoint(const JOINT* const joint);

    void moveTo(unsigned frame);

    const JOINT* getRootJoint() const { return rootJoint; }
    unsigned getNumFrames() const { return motionData.num_frames; }
private:
    JOINT* rootJoint;

    MOTION motionData;

    friend class BvhLoader;
};

}

#endif
