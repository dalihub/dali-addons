# AddOn Example

This repository is a framework that should be used in order to build DALi addons.

## Creating new AddOn

In order to create a new AddOn create a directory witin **./dali/addons** and add the cmake file with build setup. The file must have a *cmake* extension.

### AddOn build cmake file

Sample build.cmake file:

    SET(ADDON_NAME "name-of-addon")  
    SET(ADDON_VERSION "1.0.0")  
    SET(SOURCES ${CURRENT_DIR}/source_0.cpp
                ${CURRENT_DIR}/source_1.cpp)  
    SET(ADDON_ENABLED 1)

**ADDON_NAME** - name of the final addon library

**ADDON_VERSION** - version of addon

**SOURCES** - list of the source files to build the addon

**ADDON_ENABLED** - (optional, 1 by default), if 0 the addon is excluded from the build

**CURRENT_DIR** - the path relative to the addon source directory (**must not be changed!**)

### Implementing the new AddOn

To implement the new AddOn the class **AddOnBase** should be used. **AddOnBase** takes care of registering the AddOn with the AddOnManager and provides interface to create dispatch tables for AddOn exported functions.

### Sample implementation

    #include <dali/devel-api/addons/addon-base.h>
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
        static DispatchTable dispatchTable;  
        if( dispatchTable.Empty() )  
        {  
          dispatchTable["TestInstanceFunction"]  = SampleAddOnNS::TestInstanceFunction;  
          dispatchTable["TestInstanceFunction2"] = SampleAddOnNS::TestInstance1;  
          dispatchTable["TestInstanceFunction3"] = SampleAddOnNS::TestInstance3;  
        }  
        return &dispatchTable; 
      }  
    };  
      
    REGISTER_ADDON_CLASS( SampleAddOn );
### AddOnBase interface
**void AddOnBase::GetAddOnInfo( Dali::AddOnInfo& addonInfo )**
Function fills the **Dali::AddOnInfo** structure. Must be overriden by derived class.

**DispatchTable\* AddOnBase::GetGlobalDispatchTable()**
Returns valid global dispatch table or nullptr when dispatch table is unspecified.

**DispatchTable\* AddOnBase::GetInstanceDispatchTable()**
Returns valid instance dispatch table or nullptr when dispatch table is unspecified.

**REGISTER_ADDON_CLASS( CLASS_NAME )**
Macro registers the derived AddOn class and creates library entry point.

### Setting up the dispatch table
The **DispatchTable** is a map-like type. It allows to create aliases for AddOn implemented functions. To add a new entry or modify existing use index operator with a string as parameter, for example:

    dispatchTable["FunctionName"] = FunctionPointer;

The DispatchTable may be modified in the runtime in order to change the AddOn behaviour. For example, it is possible to map different functions in case of debugging the addon.

## Building the AddOn

AddOns may use DALi internal source code.

### Ubuntu

All the DALi repositories must be present next to dali-addons-example. The DESKTOP_PREFIX variable must be set (as for whole DALi build in general). 

    cd dali-addons-example/build/tizen
    cmake .
    make install

The AddOns will install at ${DESKTOP_PREFIX}/opt/lib.

### Tizen

First all DALi repositories must be built using GBS and be deployed to RPMS directory of the GBS root. The Tizen AddOns build system will use the **\*-debugsource-\*.rpm** packages.

Use GBS to build Tizen addons (as for DALi libs):

    gbs -c gbs.conf build --keep-packs --define "enable_trace 1" --define "enable-debug 1" --include-all --ccache --define "jobs 4"
