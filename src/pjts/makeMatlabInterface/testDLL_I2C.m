function testDLL_I2C()
%TESTDLLI2C Summary of this function goes here

    if ~libisloaded('libmpsse')
        disp('Not loaded library');
        return;
    end

    LN = 'libmpsse';
    [~,LN_str,LN_enum,~]=libmpsse();
    % FT_DEVICE_LIST_INFO_NODE = LN_str.s_ft_device_list_info_node.members;

    FT_DEVICE_LIST_INFO_NODE = struct(...
        'Flags', uint32(0), ...
        'Type', uint32(0), ...
        'ID', uint32(0), ...
        'LocId', uint32(0), ...
        'SerialNumber', zeros(1, 16, 'int8'), ...
        'Description', zeros(1, 64, 'int8'), ...
        'ftHandle', libpointer('voidPtr', 1));

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
        fprintf("        I2C_GetNumChannels returned %d; channels=%d\n", status, channels.Value);

        fprintf("\nTest case 2 - I2C_GetChannelInfo\n");
        chanInfo = libstruct('s_ft_device_list_info_node', FT_DEVICE_LIST_INFO_NODE);
        for idx=1:channels.Value
            status=calllib(LN,'I2C_GetChannelInfo',idx-1, chanInfo);
            t = get(chanInfo);
            fprintf("        I2C_GetChannelInfo returned %d for channel =%d\n", status, idx-1);

            fprintf("        Flags=0x%x\n", t.Flags);
            fprintf("        Type=0x%x\n", t.Type);
            fprintf("        ID=0x%x\n", t.ID);
            fprintf("        LocId=0x%x\n", t.LocId);
            fprintf("        SerialNumber=%s\n", native2unicode(t.SerialNumber));
            fprintf("        Description=%s\n", native2unicode(t.Description));
            % fprintf("        ftHandle=%p (should be zero)\n", t.ftHandle);
        end

        % Define the I2C_ChannelConfig structure
        s_I2C_ChannelConfig = struct(...
            'ClockRate', uint32(100000), ... % Default clock rate (e.g., I2C_CLOCK_STANDARD_MODE)
            'LatencyTimer', uint8(16), ...  % Default latency timer (e.g., 16ms)
            'Options', uint32(0), ...       % Default options (e.g., 0 for no options enabled)
            'Pin', uint32(0), ...           % Default pin configuration
            'currentPinState', uint16(0)); % Default current pin state

        % Convert the structure to a libstruct
        ChannelConfig = libstruct('ChannelConfig_i2c', s_I2C_ChannelConfig);

        % Define the I2C_ChannelContext structure
        s_I2C_ChannelContext = struct(...
            'handle', libpointer('voidPtr', []), ... % Pointer to device handle
            'config', ChannelConfig, ...            % Nested ChannelConfig structure
            'next', libpointer('voidPtr', []));     % Pointer to the next context in the list

        % Convert the structure to a libstruct
        ChannelContext = libstruct('ChannelContext_i2c', s_I2C_ChannelContext);

        for idx=1:channels.Value
            ftHandle = libpointer('voidPtr', 1);
            status=calllib(LN,'I2C_OpenChannel',idx-1, ftHandle);
            if status == 0
                try
                    status = calllib(LN, 'I2C_InitChannel', ftHandle, ChannelConfig);
                    if status == 0
                        disp('Channel initialized successfully.');
                        START_ADDRESS_EEPROM = 0xFA;
                        END_ADDRESS_EEPROM	 = 0xFC;
                        RETRY_COUNT_EEPROM	 = 10;     
                        I2C_DEVICE_ADDRESS_RTC	 = 		0x50;
                        I2C_DEVICE_ADDRESS_EEPROM =		0x57;
                        I2C_DEVICE_BUFFER_SIZE =			256;
                        I2C_WRITE_COMPLETION_RETRY =		10;                        
                        for address = START_ADDRESS_EEPROM:(END_ADDRESS_EEPROM-1)
			                fprintf("writing byte at address = %d\n", address);
			                write_byte(I2C_DEVICE_ADDRESS_EEPROM, address, address + 1);
                        end

                    else
                        disp(['Failed to initialize channel. Status: ', num2str(status)]);
                    end
                catch e
                    disp(e.message);
                end
                % 위에서 open한것은 반드시 close해야 한다.
                status=calllib(LN,'I2C_CloseChannel',ftHandle);
            end



        end


    catch e
        disp(e.message);
    end

    calllib('libmpsse', 'Cleanup_libMPSSE');


    function write_byte(slaveAddress, registerAddress, data)
        % MATLAB wrapper for the write_byte function in C.
        % Inputs:
        %   - libname: Name of the loaded library (e.g., 'YourLibraryName')
        %   - ftHandle: FTDI device handle (voidPtr)
        %   - slaveAddress: Address of the I2C slave device (uint8)
        %   - registerAddress: Register address to write to (uint8)
        %   - data: Data to write to the register (uint8)
        % Output:
        %   - status: FTDI status code returned by the library (uint32)
    
        % Prepare buffer for writing
        % buffer = libpointer('uint8Ptr', 256);
        buffer = zeros(1,256, 'uint8');
        buffer(1) = registerAddress;
        buffer(2) = data;
        bytesToTransfer =2;
        bytesTransfered = libpointer('uint32Ptr', 0); % Pointer for transferred bytes
    
        % Define options
        I2C_TRANSFER_OPTIONS_START_BIT = 0x00000001;
        I2C_TRANSFER_OPTIONS_STOP_BIT = 0x00000002;
    
        % Call the I2C_DeviceWrite function
        status = calllib(LN, 'I2C_DeviceWrite', ...
            ftHandle, ...
            uint8(slaveAddress), ...
            bytesToTransfer, ...
            libpointer('uint8Ptr', buffer), ...
            bytesTransfered, ...
            bitor(I2C_TRANSFER_OPTIONS_START_BIT, I2C_TRANSFER_OPTIONS_STOP_BIT));
    
        % if status ~= 0
        %     error('I2C_DeviceWrite failed with status: %d', status);
        % end
    end
    
    function [status, data] = read_byte(slaveAddress, registerAddress)
        % MATLAB wrapper for the read_byte function in C.
        % Inputs:
        %   - libname: Name of the loaded library (e.g., 'YourLibraryName')
        %   - ftHandle: FTDI device handle (voidPtr)
        %   - slaveAddress: Address of the I2C slave device (uint8)
        %   - registerAddress: Register address to read from (uint8)
        % Outputs:
        %   - status: FTDI status code returned by the library (uint32)
        %   - data: Data read from the I2C slave device (uint8)
    
        % Prepare buffer for the write operation
        writeBuffer = uint8(registerAddress); % Register address
        bytesToTransfer = uint32(1);
        bytesTransfered = libpointer('uint32Ptr', 0); % Pointer for transferred bytes
    
        % Define options
        I2C_TRANSFER_OPTIONS_START_BIT = uint32(hex2dec('02'));
    
        % Call the I2C_DeviceWrite function to set the register address
        status = calllib(LN, 'I2C_DeviceWrite', ...
            ftHandle, ...
            uint8(slaveAddress), ...
            bytesToTransfer, ...
            writeBuffer, ...
            bytesTransfered, ...
            I2C_TRANSFER_OPTIONS_START_BIT);
    
        if status ~= 0
            error('I2C_DeviceWrite failed with status: %d', status);
        end
    
        % Prepare buffer for the read operation
        readBuffer = libpointer('uint8Ptr', 0); % Allocate buffer for reading
    
        % Call the I2C_DeviceRead function
        bytesToTransfer = uint32(1);
        bytesTransfered = libpointer('uint32Ptr', 0); % Reset pointer for transferred bytes
        status = calllib(LN, 'I2C_DeviceRead', ...
            ftHandle, ...
            uint8(slaveAddress), ...
            bytesToTransfer, ...
            readBuffer, ...
            bytesTransfered, ...
            I2C_TRANSFER_OPTIONS_START_BIT);
    
        if status ~= 0
            error('I2C_DeviceRead failed with status: %d', status);
        end
    
        % Extract the read data
        data = readBuffer.Value;
    end



end
