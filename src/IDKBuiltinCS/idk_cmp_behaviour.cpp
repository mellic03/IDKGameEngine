#include "idk_components.hpp"



// template <>
// idecs::Behaviour
// idk::getBehaviour<idk::TransformCmp>()
// {
//     idecs::Behaviour B;

//     B._onCopy = []( idk::EngineAPI &api, int src, int dst )
//     {
//         auto &src_data = api.getECS().getComponent<idk::TransformCmp>(src);
//         auto &dst_data = api.getECS().getComponent<idk::TransformCmp>(dst);
//         dst_data = src_data;
//     };

//     return B;
// }



// template <>
// idecs::Behaviour
// idk::getBehaviour<idk::ScriptCmp>()
// {
//     idecs::Behaviour B;

//     B._onCopy = []( idk::EngineAPI &api, int src, int dst )
//     {
//         auto &src_data = api.getECS().getComponent<idk::ScriptCmp>(src);
//         auto &dst_data = api.getECS().getComponent<idk::ScriptCmp>(dst);
//         dst_data = src_data;
//     };

//     return B;
// }




// template <>
// idecs::Behaviour
// idk::getBehaviour<idk::ModelCmp>()
// {
//     idecs::Behaviour B;

//     B._onCopy = []( idk::EngineAPI &api, int src, int dst )
//     {
//         auto &src_data = api.getECS().getComponent<idk::ModelCmp>(src);
//         auto &dst_data = api.getECS().getComponent<idk::ModelCmp>(dst);
//         dst_data = src_data;
//         dst_data.obj_id = dst;
//     };

//     return B;
// }

