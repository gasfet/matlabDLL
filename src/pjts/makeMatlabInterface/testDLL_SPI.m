function testDLL_SPI()

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

        fprintf("\nTest case 1 - SPI_GetNumChannels\n");
        channels=libpointer('uint32Ptr',1);
        status=calllib(LN,'SPI_GetNumChannels',channels);
        fprintf("        SPI_GetNumChannels returned %d; channels=%d\n", status, channels.Value);

        fprintf("\nTest case 2 - SPI_GetChannelInfo\n");
        chanInfo = libstruct('s_ft_device_list_info_node', FT_DEVICE_LIST_INFO_NODE);
        for idx=1:channels.Value
            status=calllib(LN,'SPI_GetChannelInfo',idx-1, chanInfo);
            t = get(chanInfo);
            fprintf("        _GetChannelInfo returned %d for channel =%d\n", status, idx-1);

            fprintf("        Flags=0x%x\n", t.Flags);
            fprintf("        Type=0x%x\n", t.Type);
            fprintf("        ID=0x%x\n", t.ID);
            fprintf("        LocId=0x%x\n", t.LocId);
            fprintf("        SerialNumber=%s\n", native2unicode(t.SerialNumber));
            fprintf("        Description=%s\n", native2unicode(t.Description));
            % fprintf("        ftHandle=%p (should be zero)\n", t.ftHandle);
        end

        % Define the I2C_ChannelConfig structure
        s_SPI_ChannelConfig = struct(...
            'ClockRate', uint32(100000), ... % Default clock rate (e.g., I2C_CLOCK_STANDARD_MODE)
            'LatencyTimer', uint8(16), ...  % Default latency timer (e.g., 16ms)
            'Options', uint32(0), ...       % Default options (e.g., 0 for no options enabled)
            'Pin', uint32(0), ...           % Default pin configuration
            'currentPinState', uint16(0)); % Default current pin state
        % s_SPI_ChannelConfig.Options = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
        % Convert the structure to a libstruct
        ChannelConfig = libstruct('ChannelConfig_spi', s_SPI_ChannelConfig);

        % Define the I2C_ChannelContext structure
        s_SPI_ChannelContext = struct(...
            'handle', libpointer('voidPtr', []), ... % Pointer to device handle
            'config', ChannelConfig, ...            % Nested ChannelConfig structure
            'next', libpointer('voidPtr', []));     % Pointer to the next context in the list

        % Convert the structure to a libstruct
        ChannelContext = libstruct('ChannelContext_spi', s_SPI_ChannelContext);

        for idx=1:channels.Value
            ftHandle = libpointer('voidPtr', 1);
            status=calllib(LN,'SPI_OpenChannel',idx-1, ftHandle);
            if status == 0
                try
                    status = calllib(LN, 'SPI_InitChannel', ftHandle, ChannelConfig);
                    if status == 0
                        disp('Channel initialized successfully.');
                        % START_ADDRESS_EEPROM = 0xFA;
                        % END_ADDRESS_EEPROM	 = 0xFC;
                        % RETRY_COUNT_EEPROM	 = 10;     
                        % I2C_DEVICE_ADDRESS_RTC	 = 		0x50;
                        % I2C_DEVICE_ADDRESS_EEPROM =		0x57;
                        % I2C_DEVICE_BUFFER_SIZE =			256;
                        % I2C_WRITE_COMPLETION_RETRY =		10;                        
                        % for address = START_ADDRESS_EEPROM:(END_ADDRESS_EEPROM-1)
			            %     fprintf("writing byte at address = %d\n", address);
			            %     write_byte(I2C_DEVICE_ADDRESS_EEPROM, address, address + 1);
                        % end

                    else
                        disp(['Failed to initialize channel. Status: ', num2str(status)]);
                    end
                catch e
                    disp(e.message);
                end
                % 위에서 open한것은 반드시 close해야 한다.
                status=calllib(LN,'SPI_CloseChannel',ftHandle);
            end



        end


    catch e
        disp(e.message);
    end

    calllib('libmpsse', 'Cleanup_libMPSSE');



end

% 
% // ########################### GPIO CONTROL ####################################
% 
% // --------------------- Chip Select line low ----------------------------------
% void MCU_CSlow(void)
% {
% 	SPI_ToggleCS(ftHandle, TRUE);
% }
% 
% // --------------------- Chip Select line high ---------------------------------
% void MCU_CShigh(void)
% {
% 	SPI_ToggleCS(ftHandle, FALSE);
% }
% 
% // -------------------------- PD line low --------------------------------------
% void MCU_PDlow(void)
% {
% 	// PD# set to 0, connect BLUE wire of MPSSE to PD# of FT8xx board
% 	SPI_ChangeCS(ftHandle, SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS7 | SPI_CONFIG_OPTION_CS_ACTIVELOW);
% 	SPI_ToggleCS(ftHandle, TRUE);
% }
% 
% // ------------------------- PD line high --------------------------------------
% void MCU_PDhigh(void)
% {
% 	// PD# set to 1, connect BLUE wire of MPSSE to PD# of FT8xx board
% 	SPI_ToggleCS(ftHandle, FALSE);
% 	SPI_ChangeCS(ftHandle, SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW);
% }
% 
% // --------------------- SPI Send and Receive ----------------------------------
% 
% uint8_t MCU_SPIRead8(void)
% {
% 	uint8_t DataRead = 0;
% 	DWORD transferred;
% 
% 	SPI_Read(ftHandle, &DataRead, 1, &transferred, 0);
% 
% 	return DataRead;
% }
% 
% void MCU_SPIWrite8(uint8_t DataToWrite)
% {
% 	DWORD transferred;
% 
% 	SPI_Write(ftHandle, &DataToWrite, 1, &transferred, 0);
% }
% 
% uint16_t MCU_SPIRead16(void)
% {
% 	uint16_t DataRead = 0;
% 	DWORD transferred;
% 
% 	SPI_Read(ftHandle, (UCHAR *)&DataRead, 2, &transferred, 0);
% 
% 	return DataRead;
% }
% 
% void MCU_SPIWrite16(uint16_t DataToWrite)
% {
% 	DWORD transferred;
% 
% 	SPI_Write(ftHandle, (UCHAR *)&DataToWrite, 2, &transferred, 0);
% }
% 
% uint32_t MCU_SPIRead24(void)
% {
% 	uint32_t DataRead = 0;
% 	DWORD transferred;
% 
% 	SPI_Read(ftHandle, (UCHAR *)&DataRead, 3, &transferred, 0);
% 
% 	return DataRead;
% }
% 
% void MCU_SPIWrite24(uint32_t DataToWrite)
% {
% 	DWORD transferred;
% 
% 	SPI_Write(ftHandle, (UCHAR *)&DataToWrite, 3, &transferred, 0);
% }
% 
% uint32_t MCU_SPIRead32(void)
% {
% 	uint32_t DataRead = 0;
% 	DWORD transferred;
% 
% 	SPI_Read(ftHandle, (UCHAR *)&DataRead, 4, &transferred, 0);
% 
% 	return DataRead;
% }
% 
% void MCU_SPIWrite32(uint32_t DataToWrite)
% {
% 	DWORD transferred;
% 
% 	SPI_Write(ftHandle, (UCHAR *)&DataToWrite, 4, &transferred, 0);
% }
% 
% void MCU_SPIWrite(const uint8_t *DataToWrite, uint32_t length)
% {
% 	DWORD transferred;
% 
% 	SPI_Write(ftHandle, (UCHAR *)DataToWrite, length, &transferred, 0);
% }
