#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;
using namespace AmiBroker::PlugIn;
//
// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
[assembly:AssemblyTitleAttribute("AmiBroker.Samples.SamplePlugInVC")];
[assembly:AssemblyDescriptionAttribute("Visual C++/CLI Sample AFL plug-in")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("J. T.")];
[assembly:AssemblyProductAttribute(".NET for AmiBroker")];
[assembly:AssemblyCopyrightAttribute("Copyright (c) 2010")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
// You can specify all the value or you can default the Revision and Build Numbers
// by using the '*' as shown below:
[assembly:AssemblyVersionAttribute("6.20.3.0")];
[assembly:AssemblyFileVersionAttribute("6.20.3.0")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

//[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];

// indicate that the dll is an AmiBroker plug-in
[assembly:ABPlugIn()];