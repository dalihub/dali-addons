/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <dali/devel-api/addons/addon-base.h>

using namespace Dali::AddOns;

namespace SampleAddOnNS
{
int TestFunction(int param)
{
  return param*2;
}

int TestFunction2(int param, int param2)
{
  return param+param2;
}
}

/**
 * Sample addon implementation
 */
class SampleAddOn : public Dali::AddOns::AddOnBase
{
public:

  void GetAddOnInfo( Dali::AddOnInfo& addonInfo ) override
  {
    addonInfo.name = "SampleAddOn";
    addonInfo.type = Dali::AddOnType::GENERIC;
    addonInfo.next = nullptr;
    addonInfo.version = 0u;
  }

  DispatchTable* GetGlobalDispatchTable() override
  {
    static DispatchTable dispatchTable;
    if( dispatchTable.Empty() )
    {
      dispatchTable["TestFunction"]       = SampleAddOnNS::TestFunction;
      dispatchTable["TestFunction2"]      = SampleAddOnNS::TestFunction2;
      dispatchTable["TestFunctionAlias2"] = SampleAddOnNS::TestFunction2;
    }
    return &dispatchTable;
  }

  DispatchTable* GetInstanceDispatchTable() override
  {
    return nullptr;
  }
};

REGISTER_ADDON_CLASS( SampleAddOn );
