#pragma once

#include <cstdint>
#include <random>
#include <string>

#define SCRIPT_CIDX 0
#define TRANSFORM_CIDX 1
#define MESH_INSTANCE_CIDX 2
#define CAMERA_CIDX 3

//Random / UUID
static std::random_device s_RandomDevice;
static std::mt19937_64 s_Editor(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;
uint64_t randUUID();

int createProject(const std::string &name, const std::string &path);
