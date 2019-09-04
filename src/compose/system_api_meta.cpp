//------------------------------------------------------------------------------
// system_api_meta.cpp - Autogenerated with codegen.py from system_api.h
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#include "core/HashMap.h"

#include "compose/compiler_structs.h"

namespace gaen
{

void register_system_apis(ParseData * pParseData)
{
    {
        // create_shape_box
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("size", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("create_shape_box", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "create_shape_box", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "handle", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // create_shape_cone
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("size", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("slices", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("create_shape_cone", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "create_shape_cone", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "handle", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // create_shape_cylinder
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("size", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("slices", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("create_shape_cylinder", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "create_shape_cylinder", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "handle", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // create_shape_sphere
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("size", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("slices", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("sections", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("create_shape_sphere", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "create_shape_sphere", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "handle", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // create_shape_quad_sphere
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("size", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("sections", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("create_shape_quad_sphere", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "create_shape_quad_sphere", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "handle", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // print
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("str", parsedata_find_type_symbol(pParseData, "string", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("print", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "print", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // hashstr
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("hash", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("hashstr", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "hashstr", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "string", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // print_asset_info
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("asset", parsedata_find_type_symbol(pParseData, "asset_handle", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("print_asset_info", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "print_asset_info", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // radians
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("degrees", parsedata_find_type_symbol(pParseData, "float", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("radians", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "radians", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "float", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // degrees
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("radians", parsedata_find_type_symbol(pParseData, "float", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("degrees", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "degrees", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "float", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // watch_input_state
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("state", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("deviceId", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("message", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("watch_input_state", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "watch_input_state", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // watch_mouse
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("moveMessage", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("wheelMessage", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("watch_mouse", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "watch_mouse", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // mat43_rotation
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("angles", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        size_t mangledLen = mangle_function_len("mat43_rotation", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "mat43_rotation", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "mat43", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // mat3_rotation
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("angles", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        size_t mangledLen = mangle_function_len("mat3_rotation", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "mat3_rotation", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "mat3", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // quat_from_axis_angle
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("dir", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("angle", parsedata_find_type_symbol(pParseData, "float", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("quat_from_axis_angle", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "quat_from_axis_angle", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "quat", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // quat_normalize
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("quat", parsedata_find_type_symbol(pParseData, "quat", 1, 1), pParseData));
        size_t mangledLen = mangle_function_len("quat_normalize", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "quat_normalize", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "quat", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_gen_uid
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        size_t mangledLen = mangle_function_len("renderer_gen_uid", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_gen_uid", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "uint", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_move_camera
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("position", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("direction", parsedata_find_type_symbol(pParseData, "quat", 1, 1), pParseData));
        size_t mangledLen = mangle_function_len("renderer_move_camera", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_move_camera", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_move_fps_camera
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("position", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("pitch", parsedata_find_type_symbol(pParseData, "float", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("yaw", parsedata_find_type_symbol(pParseData, "float", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("renderer_move_fps_camera", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_move_fps_camera", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_insert_model_instance
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("uid", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("modelHandle", parsedata_find_type_symbol(pParseData, "handle", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("renderer_insert_model_instance", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_insert_model_instance", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_transform_model_instance
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("uid", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("transform", parsedata_find_type_symbol(pParseData, "mat43", 1, 1), pParseData));
        size_t mangledLen = mangle_function_len("renderer_transform_model_instance", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_transform_model_instance", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_remove_model_instance
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("uid", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("renderer_remove_model_instance", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_remove_model_instance", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_insert_light_directional
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("uid", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("direction", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("renderer_insert_light_directional", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_insert_light_directional", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_update_light_directional
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("uid", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("direction", parsedata_find_type_symbol(pParseData, "vec3", 1, 1), pParseData));
        ast_add_child(pSystemApiDef, ast_create_function_arg("color", parsedata_find_type_symbol(pParseData, "color", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("renderer_update_light_directional", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_update_light_directional", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }
    {
        // renderer_remove_light_directional
        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);
        ast_add_child(pSystemApiDef, ast_create_function_arg("uid", parsedata_find_type_symbol(pParseData, "uint", 0, 0), pParseData));
        size_t mangledLen = mangle_function_len("renderer_remove_light_directional", pSystemApiDef->pChildren);
        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);
        mangle_function(mangledName, kMaxCmpId, "renderer_remove_light_directional", pSystemApiDef->pChildren);
        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "void", 0, 0)->pSymDataType, mangledName, pSystemApiDef, pParseData));
    }

}


} // namespace gaen
