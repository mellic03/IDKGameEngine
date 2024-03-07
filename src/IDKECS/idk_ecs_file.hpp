#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <memory>

#include <libidk/idk_serialize.hpp>


namespace idk::ecs
{
    static constexpr uint32_t IDK_ECS_VERSION_MAJOR = 1;
    static constexpr uint32_t IDK_ECS_VERSION_MINOR = 1;

    class iComponentArray;

    struct ComponentFileHeader
    {
        uint32_t    major;
        uint32_t    minor;
        uint32_t    size;
        uint32_t    bytesize;
        uint32_t    id;
        std::string name;
    };

    struct EntityHierarchyFileHeader
    {
        uint32_t major;
        uint32_t minor;

        uint32_t num_entities;

    };

    struct ECSFileHeader
    {
        uint32_t major;
        uint32_t minor;
        uint32_t num_arrays;
    };



    void ComponentFile_write( std::ofstream &,
                              const ComponentFileHeader &,
                              iComponentArray * );

    void ComponentFile_readHeader( std::ifstream &,
                                   ComponentFileHeader & );

    void ComponentFile_readData( std::ifstream &,
                                 const ComponentFileHeader &,
                                 iComponentArray * );

    void ComponentFile_skipData( std::ifstream &,
                                 const ComponentFileHeader & );


    ComponentFileHeader ComponentFile_new( iComponentArray * );

    void EntityHierarchyFile_write( std::ofstream & );
    void EntityHierarchyFile_read( std::ifstream & );

    void ECSFile_write( std::ofstream &, const ECSFileHeader & );
    void ECSFile_read( std::ifstream &, ECSFileHeader & );

};


