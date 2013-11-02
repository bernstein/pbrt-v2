
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


// filters/sinc.cpp*
#include "stdafx.h"
#include "filters/sinc.h"
#include "paramset.h"

namespace {
  float Sinc1D(float tau, float x) {
      x = fabsf(x);
      if (x < 1e-5) return 1.f;
      if (x > 1.)   return 0.f;
      x *= M_PI;
      const float sinc = sinf(x) / x;
      const float lanczos = sinf(x * tau) / (x * tau);
      return sinc * lanczos;
  }
}

std::function<float(float,float)> lanczosSincFilter(float xw, float yw, float tau)
{
  float invXWidth(1.0f/xw);
  float invYWidth(1.0f/yw);
  return [invXWidth,invYWidth,tau](float x, float y) {
    return Sinc1D(tau, x * invXWidth) * Sinc1D(tau, y * invYWidth);
  };
}

Filter CreateSincFilter(const ParamSet &ps) {
    float xw = ps.FindOneFloat("xwidth", 4.);
    float yw = ps.FindOneFloat("ywidth", 4.);
    float tau = ps.FindOneFloat("tau", 3.f);
    return Filter(xw, yw, lanczosSincFilter(xw,yw,tau));
}
