% loadlibrary('libmpsse.dll', 'all.h', 'alias', 'mpsse', 'mfilename', 'libmpsse');
if ~libisloaded('libmpsse')
    loadlibrary('libmpsse.dll',@libmpsse);
end
% libfunctionsview libmpsse

calllib('libmpsse', 'Init_libMPSSE');

verMPSSE=libpointer('uint32Ptr',1);
verD2XX=libpointer('uint32Ptr',1);
status=calllib('libmpsse','Ver_libMPSSE',verMPSSE, verD2XX);
if ~status, disp(status); end
fprintf("libmpsse: %08x\n", verMPSSE.Value);
fprintf("libftd2xx: %08x\n", verD2XX.Value);

% fprintf("\nTest case 1 - I2C_GetNumChannels\n");
% status = I2C_GetNumChannels(&channels);
% fprintf("		I2C_GetNumChannels returned %d; channels=%d\n", status, channels);


calllib('libmpsse', 'Cleanup_libMPSSE');
unloadlibrary libmpsse