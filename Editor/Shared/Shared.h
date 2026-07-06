#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

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


//Component stuff
struct Component
{};

struct transform_component : Component
{};

struct script_component : Component
{};

struct meshinstance_component : Component
{};

struct camera_component : Component
{};

Component getComponentById(int id);


struct Object
{
    std::string Name;
    uint64_t UUID;
    std::vector<Component> Components;
};


struct Scene
{
    std::string Name;
    uint64_t UUID;
    std::unordered_map<uint64_t, Object> Objects;
};


class Editor
{
public:
    Editor(const std::string &path);
    ~Editor();

    std::string GetProjectName() const;
    std::string GetDefaultScene() const;
    std::string GetDefaultShader() const;

private:
    Scene LoadScene(const std::string &path);

    std::string m_Path;
    std::string m_ProjectName;
    std::string m_DefaultScene;
    std::string m_DefaultShader;

    std::unordered_map<uint64_t, Scene> m_Scenes;
};
