%%

% hFile = ["ftd2xx.h", "ftdi_common.h", "ftdi_infra.h", "ftdi_mid.h", "libmpsse_i2c.h", "libmpsse_spi.h"];
% hFile = [ "libmpsse_i2c.h", "libmpsse_spi.h"];
hFile = ["all.h"];

clibgen.generateLibraryDefinition( hFile, ...
Libraries="libmpsse.lib", ...
IncludePath=pwd(), ...
InterfaceName="ftLib", ...
OutputFolder=pwd(), ...
OverwriteExistingDefinitionFiles=true);


%%

% unload(clibConfiguration('ftLib'))

build(defineftLib)
addpath("ftLib\");
clibConfiguration('ftLib','ExecutionMode','outofprocess')