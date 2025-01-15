%%

% hFile = ["ftd2xx.h", "ftdi_common.h", "ftdi_infra.h", "ftdi_mid.h", "libmpsse_i2c.h", "libmpsse_spi.h"];
hFile = [ "libmpsse_i2c.h", "libmpsse_spi.h"];

clibgen.generateLibraryDefinition( hFile, ...
Libraries="libmpsse.lib", ...
IncludePath=pwd(), ...
InterfaceName="ftLib", ...
OutputFolder=pwd(), ...
DefinedMacros=["FT_VER_MAJOR=1","FT_VER_MINOR=0", "FT_VER_BUILD=1", "WIN32", "_WINDOWS", "_USRDLL", "FTDIMPSSE_EXPORTS"], ...
OverwriteExistingDefinitionFiles=true);


%%
build(defineftLib)
