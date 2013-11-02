
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

// filters/gaussian.cpp*
#include "stdafx.h"
#include "filters/gaussian.h"
#include "paramset.h"

namespace {

float gaussian(float alpha, float d, float expv) {
    return max(0.f, float(expf(-alpha * d * d) - expv));
}

}

std::function<float(float,float)> gaussianFilter(float xw, float yw, float alpha)
{
  const float expX(expf(-alpha * xw * xw));
  const float expY(expf(-alpha * yw * yw));
  return [expX,expY,alpha](float x, float y) {
    return gaussian(alpha, x, expX) * gaussian(alpha, y, expY);
  };
}

Filter CreateGaussianFilter(const ParamSet &ps) {
    // Find common filter parameters
    float xw = ps.FindOneFloat("xwidth", 2.f);
    float yw = ps.FindOneFloat("ywidth", 2.f);
    float alpha = ps.FindOneFloat("alpha", 2.f);
    return Filter(xw, yw, gaussianFilter(xw,yw,alpha));
}
