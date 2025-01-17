function testDLL_I2C()
%TESTDLLI2C Summary of this function goes here

    if ~libisloaded('libmpsse') 
        disp('Not loaded library');
        return; 
    end
    
    LN = 'libmpsse';
    [~,LN_str,LN_enum,~]=libmpsse();
    FT_DEVICE_LIST_INFO_NODE = LN_str.s_ft_device_list_info_node.members;

    calllib(LN, 'Init_libMPSSE');
    
    try
    	fprintf("\nVersion Check\n");
        verMPSSE=libpointer('uint32Ptr',1);
        verD2XX=libpointer('uint32Ptr',1);
        status=calllib(LN,'Ver_libMPSSE',verMPSSE, verD2XX);
        if status, disp(status); end
        fprintf("libmpsse: %08x\n", verMPSSE.Value);
        fprintf("libftd2xx: %08x\n", verD2XX.Value);
        
        fprintf("\nTest case 1 - I2C_GetNumChannels\n");
        channels=libpointer('uint32Ptr',1);
        status=calllib(LN,'I2C_GetNumChannels',channels);
        fprintf("		I2C_GetNumChannels returned %d; channels=%d\n", status, channels.Value);

	    fprintf("\nTest case 2 - I2C_GetChannelInfo\n");

        chanInfo = libstruct('s_ft_device_list_info_node', FT_DEVICE_LIST_INFO_NODE);
        for idx=1:channels.Value
            status=calllib(LN,'I2C_GetChannelInfo',idx, chanInfo);
            t = get(chanInfo);
        end



    catch e
        disp(e.message);
    end
    
    calllib('libmpsse', 'Cleanup_libMPSSE');

end

