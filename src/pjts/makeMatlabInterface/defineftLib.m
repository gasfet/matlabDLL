%% About defineftLib.m
% This file defines the MATLAB interface to the library |ftLib|.
%
% Commented sections represent C++ functionality that MATLAB cannot automatically define. To include
% functionality, uncomment a section and provide values for <SHAPE>, <DIRECTION>, etc. For more
% information, see helpview(fullfile(docroot,'matlab','helptargets.map'),'cpp_define_interface') to "Define MATLAB Interface for C++ Library".



%% Setup
% Do not edit this setup section.
function libDef = defineftLib()
libDef = clibgen.LibraryDefinition("ftLibData.xml");

%% OutputFolder and Libraries 
libDef.OutputFolder = "D:\myGits\FTDI\src\pjts\makeMatlabInterface";
libDef.Libraries = "libmpsse.lib";

%% C++ function |Init_libMPSSE| with MATLAB name |clib.ftLib.Init_libMPSSE|
% C++ Signature: void Init_libMPSSE()

Init_libMPSSEDefinition = addFunction(libDef, ...
    "void Init_libMPSSE()", ...
    "MATLABName", "clib.ftLib.Init_libMPSSE", ...
    "Description", "clib.ftLib.Init_libMPSSE Representation of C++ function Init_libMPSSE."); % Modify help description values as needed.
validate(Init_libMPSSEDefinition);

%% C++ function |Cleanup_libMPSSE| with MATLAB name |clib.ftLib.Cleanup_libMPSSE|
% C++ Signature: void Cleanup_libMPSSE()

Cleanup_libMPSSEDefinition = addFunction(libDef, ...
    "void Cleanup_libMPSSE()", ...
    "MATLABName", "clib.ftLib.Cleanup_libMPSSE", ...
    "Description", "clib.ftLib.Cleanup_libMPSSE Representation of C++ function Cleanup_libMPSSE."); % Modify help description values as needed.
validate(Cleanup_libMPSSEDefinition);

%% C++ function |Ver_libMPSSE| with MATLAB name |clib.ftLib.Ver_libMPSSE|
% C++ Signature: FT_STATUS Ver_libMPSSE(LPDWORD libmpsse,LPDWORD libftd2xx)

%Ver_libMPSSEDefinition = addFunction(libDef, ...
%    "FT_STATUS Ver_libMPSSE(LPDWORD libmpsse,LPDWORD libftd2xx)", ...
%    "MATLABName", "clib.ftLib.Ver_libMPSSE", ...
%    "Description", "clib.ftLib.Ver_libMPSSE Representation of C++ function Ver_libMPSSE."); % Modify help description values as needed.
%defineArgument(Ver_libMPSSEDefinition, "libmpsse", "clib.array.ftLib.UnsignedLong", "input", <SHAPE>); % <MLTYPE> can be "clib.array.ftLib.UnsignedLong", or "uint32"
%defineArgument(Ver_libMPSSEDefinition, "libftd2xx", "clib.array.ftLib.UnsignedLong", "input", <SHAPE>); % <MLTYPE> can be "clib.array.ftLib.UnsignedLong", or "uint32"
%defineOutput(Ver_libMPSSEDefinition, "RetVal", "uint32");
%validate(Ver_libMPSSEDefinition);

%% Validate the library definition
validate(libDef);

end
