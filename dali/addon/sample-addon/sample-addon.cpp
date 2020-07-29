/**
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 * All rights reserved.
 *
 * This software is a confidential and proprietary information of Samsung
 * Electronics, Inc. ("Confidential Information").  You shall not disclose such
 * Confidential Information and use it in accordance with the terms of your
 * employment with Samsung.
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
