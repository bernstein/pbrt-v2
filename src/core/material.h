
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef PBRT_CORE_MATERIAL_H
#define PBRT_CORE_MATERIAL_H

// core/material.h*
#include "pbrt.h"
#include "memory.h"
#include <functional>

// Material Declarations
struct Material {
    Material(const Material& m) : getBSDF(m.getBSDF), getBSSRDF(m.getBSSRDF) {}
    Material& operator=(const Material& rhs) {
      if (this != &rhs) {
        getBSDF = rhs.getBSDF;
        getBSSRDF = rhs.getBSSRDF;
      }
      return *this;
    }
    Material() {
      getBSSRDF = [](const DifferentialGeometry&,const DifferentialGeometry&,
                     MemoryArena&) -> BSSRDF* {
        return nullptr;
      };
    }
    Material(std::function<BSDF*(const DifferentialGeometry&,const DifferentialGeometry&,
                          MemoryArena&)> const& f) : getBSDF(f) {
      getBSSRDF = [] (const DifferentialGeometry&,const DifferentialGeometry&,
                          MemoryArena&) -> BSSRDF* {
        return nullptr;
      };
    }
    Material(std::function<BSDF*(const DifferentialGeometry&,const DifferentialGeometry&,
                          MemoryArena&)> const& f,
            std::function<BSSRDF*(const DifferentialGeometry&,const DifferentialGeometry&,
                          MemoryArena&)> const& f2)
            : getBSDF(f), getBSSRDF(f2) {}

    BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                          const DifferentialGeometry &dgShading,
                          MemoryArena &arena) const
    {
      return getBSDF(dgGeom, dgShading, arena);
    }
    BSSRDF *GetBSSRDF(const DifferentialGeometry &dgGeom,
                              const DifferentialGeometry &dgShading,
                              MemoryArena &arena) const {
        return getBSSRDF(dgGeom, dgShading, arena);
    }
    static void Bump(const Reference<Texture<float> > &d, const DifferentialGeometry &dgGeom,
        const DifferentialGeometry &dgShading, DifferentialGeometry *dgBump);

    std::function<BSDF*(const DifferentialGeometry&,const DifferentialGeometry&,
                          MemoryArena&)> getBSDF;
    std::function<BSSRDF*(const DifferentialGeometry&,const DifferentialGeometry&,
                          MemoryArena&)> getBSSRDF;
};

#endif // PBRT_CORE_MATERIAL_H
