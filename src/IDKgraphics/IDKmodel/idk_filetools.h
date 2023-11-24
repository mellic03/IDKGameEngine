// #pragma once

// #include "idk_filetypes.h"
// #include "idk_model.h"

// #include <fstream>


// namespace idk::filetools
// {
//     void    tex_save(std::string filepath, idk::__tex_file_t &);
//     void    tex_load(std::string filepath, idk::__tex_file_t &);
//     void    texpak_save(std::string filepath, idk::__texpak_file_t &);
//     void    texpak_load(std::string filepath, idk::__texpak_file_t &);

//     void    mat_save(std::string filepath, idk::__mat_file_t &);
//     void    mat_load(std::string filepath, idk::__mat_file_t &);
//     idk::__mat_file_t   mat_cast( idk::Material & );
//     idk::Material       mat_cast( idk::__mat_file_t & );

//     void    vts_save(std::string filepath, idk::__vts_file_t &);
//     void    vts_load(std::string filepath, idk::__vts_file_t &);

//     void    mdl_save(std::string filepath, idk::__mdl_file_t &);
//     void    mdl_load(std::string filepath, idk::__mdl_file_t &);

//     template <typename T>
//     void    vector_to_bin( std::string filepath, std::vector<T> &v );

//     template <typename T>
//     void    vector_from_bin( std::string filepath, std::vector<T> &v );

//     __tex_file_t    texFromIMG( std::string filepath );
// };




// template <typename T>
// void
// idk::filetools::vector_to_bin( std::string filepath, std::vector<T> &v )
// {
//     std::ofstream stream(filepath, std::ios_base::binary);

//     int vecsize = v.size();
//     stream.write((const char *)(&vecsize), sizeof(int));
//     stream.write((const char *)(&v[0]), vecsize*sizeof(T));
    
//     stream.close();
// }


// template <typename T>
// void
// idk::filetools::vector_from_bin( std::string filepath, std::vector<T> &v )
// {
//     std::ifstream stream(filepath, std::ios_base::binary);

//     int vecsize;
//     stream.read((char *)(&vecsize), sizeof(int));
//     v.resize(vecsize);
//     stream.read((char *)(&v[0]), vecsize*sizeof(T));
    
//     stream.close();
// }