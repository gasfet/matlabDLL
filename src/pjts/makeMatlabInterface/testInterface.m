clib.ftLib.Init_libMPSSE();

% % Prepare output variables as pointers
% libmpsse_ptr = libpointer('uint32Ptr', 0);
% libftd2xx_ptr = libpointer('uint32Ptr', 0);

a = clib.ftLib.Ver_libMPSSE(0,0);

% % Retrieve values from pointers
% libmpsse = libmpsse_ptr.Value;
% libftd2xx = libftd2xx_ptr.Value;

clib.ftLib.Cleanup_libMPSSE();
