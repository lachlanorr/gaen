//------------------------------------------------------------------------------
// Qbt.cpp - Qubicle binary tree reader
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include <array>

#include <zlib.h>

#include "gaen/assets/file_utils.h"
#include "gaen/chef/Qbt.h"

namespace gaen
{

struct WorkBuffers
{
    Vector<kMEM_Chef, u8> comp;
    Vector<kMEM_Chef, u8> uncomp;
    WorkBuffers(size_t compLen, size_t uncompLen)
      : comp(compLen)
      , uncomp(uncompLen)
    {}
};

static ChefString read_qbt_node_name(FileReader & rdr)
{
    static const u32 kMaxNameLen = 255;

    u32 nameLen;
    rdr.read(&nameLen);
    PANIC_IF(nameLen > kMaxNameLen, "Very long name in node, probably an error, length: %d", nameLen);

    std::array<char, kMaxNameLen> nameArr;
    rdr.ifs.read(nameArr.data(), nameLen);
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != nameLen, "Failed to read name of length: %d", nameLen);

    return ChefString(nameArr.data(), nameLen);
}

static void read_qbt_voxel_data(QbtNode & node, FileReader & rdr, WorkBuffers & bufs)
{
    rdr.read(&node.position);
    rdr.read(&node.localScale);
    rdr.read(&node.pivot);
    rdr.read(&node.size);

    u32 voxelDataSize;
    rdr.read(&voxelDataSize);

    if (voxelDataSize > bufs.comp.size())
    {
        bufs.comp.resize(voxelDataSize * 2);
    }

    rdr.ifs.read((char*)bufs.comp.data(), voxelDataSize);
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != voxelDataSize, "Failed to read voxel data of length: %d", voxelDataSize);

    uLongf uncompSize = bufs.uncomp.size();
    int ret = uncompress(bufs.uncomp.data(), &uncompSize, bufs.comp.data(), voxelDataSize);

    PANIC_IF(ret == Z_MEM_ERROR, "Not enough data to zlib uncompress");
    u32 resizeCount = 0;
    while (ret == Z_BUF_ERROR)
    {
        resizeCount++;
        PANIC_IF(resizeCount > 4, "Too many resizes, compressed data is too large");
        bufs.uncomp.resize(bufs.uncomp.size() * 2);
        uncompSize = bufs.uncomp.size();
        ret = uncompress(bufs.uncomp.data(), &uncompSize, bufs.comp.data(), voxelDataSize);
    }
    PANIC_IF(ret != Z_OK, "Failed to zlib uncompress: %d", ret);
    PANIC_IF(uncompSize % sizeof(Color) != 0, "zlib uncompress size not a multiple of Color size: %d", uncompSize);
    node.voxels.resize(uncompSize / sizeof(Color));
    memcpy(node.voxels.data(), bufs.uncomp.data(), uncompSize);
}

static QbtNodeUP read_qbt_node(const QbtNode * pParent, FileReader & rdr, WorkBuffers & bufs)
{
    QbtNodeUP pNode(GNEW(kMEM_Chef, QbtNode));
    pNode->pParent = pParent;
    rdr.read(&pNode->typeId);

    u32 dataSize;
    rdr.read(&dataSize);

    u32 childCount = 0;

    switch(pNode->typeId)
    {
    case kQBNT_Matrix:
        pNode->name = read_qbt_node_name(rdr);
        read_qbt_voxel_data(*pNode, rdr, bufs);
        break;
    case kQBNT_Model:
        rdr.read(&childCount);
        break;
    case kQBNT_Compound:
        pNode->name = read_qbt_node_name(rdr);
        read_qbt_voxel_data(*pNode, rdr, bufs);
        rdr.read(&childCount);
        break;
    default:
        PANIC("Invalid qbt node type: %d", pNode->typeId);
    }

    if (childCount > 0)
    {
        pNode->children.reserve(childCount);
        for (u32 i = 0; i < childCount; ++i)
        {
            pNode->children.push_back(read_qbt_node(pNode.get(), rdr, bufs));
        }
    }

    return pNode;
}

QbtUP Qbt::load_from_file(const char * path)
{
    FileReader rdr(path);
    PANIC_IF(!rdr.isOk(), "Unable to load file: %s", path);

    QbtUP pQbt(GNEW(kMEM_Chef, Qbt));

    u32 magic;
    rdr.read(&magic);
    PANIC_IF(magic != kQbtMagic, "Qbt magic mismatch: %0x8", magic);

    u8 majorVer, minorVer;
    rdr.read(&majorVer);
    rdr.read(&minorVer);
    PANIC_IF(majorVer != 1 || minorVer != 0, "Version mismatch: %d.%d", majorVer, minorVer);

    vec3 globalScale;
    rdr.read(&globalScale);

    char colorMapSectionCaption[8];
    rdr.read(&colorMapSectionCaption);
    PANIC_IF(0 != strncmp(colorMapSectionCaption, "COLORMAP", 8), "Invalid colormap section caption");

    u32 colorCount;
    rdr.read(&colorCount);
    PANIC_IF(colorCount > 255, "Invalid colorCount: %d", colorCount);

    if (colorCount > 0)
    {
        pQbt->colors.resize(colorCount);
        rdr.ifs.read((char*)pQbt->colors.data(), colorCount * sizeof(Color));
        PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != colorCount * sizeof(Color), "Failed to read color map, EOF");
    }

    char dataTreeSectionCaption[8];
    rdr.read(&dataTreeSectionCaption);
    PANIC_IF(0 != strncmp(dataTreeSectionCaption, "DATATREE", 8), "Invalid datatree section caption");

    WorkBuffers bufs(1024 * 1024, 5 * 1024 * 1024); // sizes will grow if necessary

    pQbt->pRoot = read_qbt_node(nullptr, rdr, bufs);

    return pQbt;
}

} // namespace gaen
