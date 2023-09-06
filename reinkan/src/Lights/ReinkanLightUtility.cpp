#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
    void ReinkanApp::AppendLight(const LightObject& lightObject)
    {
        appLightObjects.push_back(lightObject);
    }
}