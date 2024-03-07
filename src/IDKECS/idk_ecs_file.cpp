#include "idk_ecs_file.hpp"
#include "idk_ecs_component.hpp"

#include <libidk/idk_assert.hpp>



void
idk::ecs::ComponentFile_write( std::ofstream &stream,
                               const ComponentFileHeader &header,
                               iComponentArray *comp_array )
{
    stream.write(reinterpret_cast<const char *>(&header), 5*sizeof(uint32_t));
    idk::streamwrite(stream, header.name);

    size_t written = comp_array->serialize(stream);

    idk_printvalue(header.name);
    idk_printvalue(header.bytesize);
    idk_printvalue(written);
    std::cout << "\n";
}



void
idk::ecs::ComponentFile_readHeader( std::ifstream &stream, ComponentFileHeader &header )
{
    stream.read(reinterpret_cast<char *>(&header), 5*sizeof(uint32_t));
    idk::streamread<std::string>(stream, header.name);
}


void
idk::ecs::ComponentFile_readData( std::ifstream &stream,
                                  const ComponentFileHeader &header,
                                  iComponentArray *comp_array )
{
    comp_array->deserialize(stream);
}


void
idk::ecs::ComponentFile_skipData( std::ifstream &stream,
                                  const ComponentFileHeader &header )
{
    std::cout << "Skipping " << header.bytesize << " bytes from " << header.name << " component\n";
    auto data = std::malloc(header.bytesize);
    stream.read(reinterpret_cast<char *>(data), header.bytesize);
    std::free(data);
}



idk::ecs::ComponentFileHeader
idk::ecs::ComponentFile_new( iComponentArray *comp_array )
{
    ComponentFileHeader header = {
        .major    = IDK_ECS_VERSION_MAJOR,
        .minor    = IDK_ECS_VERSION_MINOR,
        .size     = comp_array->numObjects(),
        .bytesize = comp_array->nbytes(),
        .id       = uint32_t(comp_array->getID()),
        .name     = comp_array->name(),
    };

    return header;
}



void
idk::ecs::ECSFile_write( std::ofstream &stream, const ECSFileHeader &header )
{
    stream.write(reinterpret_cast<const char *>(&header), sizeof(ECSFileHeader));
}


void
idk::ecs::ECSFile_read( std::ifstream &stream, ECSFileHeader &header )
{
    stream.read(reinterpret_cast<char *>(&header), sizeof(ECSFileHeader));
}



