
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.
    Andreas-C. Bernstein 2013

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


// filters/mitchell.cpp*
#include "stdafx.h"
#include "filters/mitchell.h"
#include "paramset.h"

namespace {

float Mitchell1D(float B, float C, float x) {
  x = fabsf(2.f * x);
  if (x > 1.f)
    return ((-B - 6*C) * x*x*x + (6*B + 30*C) * x*x +
            (-12*B - 48*C) * x + (8*B + 24*C)) * (1.f/6.f);
  else
    return ((12 - 9*B - 6*C) * x*x*x +
            (-18 + 12*B + 6*C) * x*x +
            (6 - 2*B)) * (1.f/6.f);
}

}

std::function<float(float,float)>
mitchellFilter(float B, float C, float xw, float yw)
{
  float invXWidth(1.0f/xw);
  float invYWidth(1.0f/yw);

  return [B,C,invXWidth,invYWidth](float x, float y) {
    return Mitchell1D(B, C, x * invXWidth) * Mitchell1D(B, C, y * invYWidth);
  };
}

Filter CreateMitchellFilter(const ParamSet &ps) {
    // Find common filter parameters
    float xw = ps.FindOneFloat("xwidth", 2.f);
    float yw = ps.FindOneFloat("ywidth", 2.f);
    float B = ps.FindOneFloat("B", 1.f/3.f);
    float C = ps.FindOneFloat("C", 1.f/3.f);
    return Filter(xw, yw, mitchellFilter(B, C, xw, yw));
}
