function testPCA9698(LN, fH)

    PCA9698_I2C_ADDRESS = 0x20;
    PCA9698_REG_INPUT_PORT0 = 0x00;
    PCA9698_REG_OUTPUT_PORT0 = 0x08;
    PCA9698_REG_CONFIG_PORT0 = 0x18;

    % for addr =PCA9698_I2C_ADDRESS
    %     PCA9698_WriteRegister(LN, fH, addr, PCA9698_REG_CONFIG_PORT0, 0x00);
    %     PCA9698_WriteRegister(LN, fH, addr, PCA9698_REG_OUTPUT_PORT0, 0xFF);
    %     PCA9698_WriteRegister(LN, fH, addr, PCA9698_REG_OUTPUT_PORT0, 0x00);
    % end
    % [~, r] = PCA9698_ReadRegister(LN, fH,  PCA9698_REG_INPUT_PORT0);
    % disp(r)
    test112(LN, fH);
    test111(LN, fH);
end

function status = PCA9698_WriteRegister(LN, ftHandle, addr, reg, value)
    I2C_TRANSFER_OPTIONS_START_BIT = 0x00000001;
    I2C_TRANSFER_OPTIONS_STOP_BIT = 0x00000002;
    I2C_TRANSFER_OPTIONS_BREAK_ON_NACK = 0x00000004;
    I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE = 0x00000008;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER = 0x00000030;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES = 0x00000010;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BITS = 0x00000020;
    I2C_TRANSFER_OPTIONS_NO_ADDRESS = 0x00000040;

    bytesTransfered = libpointer('uint32Ptr', 0); % Pointer for transferred bytes
    bufferPtr = libpointer('uint8Ptr', zeros(1,256, 'uint8'));
    bufferPtr.Value(1) = reg;
    bufferPtr.Value(2) = value;
    bytesToTransfer = 2;

    % Call the I2C_DeviceWrite function to set the register address
    optW = bitor(I2C_TRANSFER_OPTIONS_START_BIT, I2C_TRANSFER_OPTIONS_STOP_BIT, 'uint32');
    % optW = bitor(optW, I2C_DEVICE_ADDRESS_EEPROM, 'uint32');
    status = calllib(LN, 'I2C_DeviceWrite', ...
        ftHandle, ...
        uint8(addr), ...
        bytesToTransfer, ...
        bufferPtr, ...
        bytesTransfered, ...
        optW);

     if status ~= 0
         disp("레지스터 쓰기 실패");
     end

end

function [status, r] = PCA9698_ReadRegister(LN, ftHandle, addr, reg)
    PCA9698_I2C_ADDRESS = 0x20;
    I2C_TRANSFER_OPTIONS_START_BIT = 0x00000001;
    I2C_TRANSFER_OPTIONS_STOP_BIT = 0x00000002;
    I2C_TRANSFER_OPTIONS_BREAK_ON_NACK = 0x00000004;
    I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE = 0x00000008;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER = 0x00000030;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES = 0x00000010;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BITS = 0x00000020;
    I2C_TRANSFER_OPTIONS_NO_ADDRESS = 0x00000040;
    bytesTransfered = libpointer('uint32Ptr', 0); % Pointer for transferred bytes
    bufferPtr = libpointer('uint8Ptr', zeros(1,256, 'uint8'));
    bufferPtr.Value(1) = reg;
    bytesToTransfer = 1;

    % Call the I2C_DeviceWrite function to set the register address
    optW = bitor(I2C_TRANSFER_OPTIONS_START_BIT, 0, 'uint32');
    % optW = bitor(optW, I2C_DEVICE_ADDRESS_EEPROM, 'uint32');
    status = calllib(LN, 'I2C_DeviceWrite', ...
        ftHandle, ...
        uint8(addr), ...
        bytesToTransfer, ...
        bufferPtr, ...
        bytesTransfered, ...
        optW);

     if status ~= 0
         disp("레지스터 쓰기 실패");
     end

    % Call the I2C_DeviceRead function
    bytesToTransfer =1;
    bytesTransfered.Value = 0;
    optR = bitor(I2C_TRANSFER_OPTIONS_STOP_BIT, 0, 'uint32');
    % optR = bitor(optR, I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE, 'uint32');
    status = calllib(LN, 'I2C_DeviceRead', ...
        ftHandle, ...
        uint8(PCA9698_I2C_ADDRESS), ...
        bytesToTransfer, ...
        bufferPtr, ...
        bytesTransfered, ...
        optR);

     if status ~= 0
         disp("레지스터 읽기 실패");
     end
    r = bufferPtr.Value(1);
end

function test112(LN, ftHandle)
    PCA9698_I2C_ADDRESS = 0x20;
    I2C_TRANSFER_OPTIONS_START_BIT = 0x00000001;
    I2C_TRANSFER_OPTIONS_STOP_BIT = 0x00000002;
    I2C_TRANSFER_OPTIONS_BREAK_ON_NACK = 0x00000004;
    I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE = 0x00000008;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER = 0x00000030;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES = 0x00000010;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BITS = 0x00000020;
    I2C_TRANSFER_OPTIONS_NO_ADDRESS = 0x00000040;


    bytesTransfered = libpointer('uint32Ptr', 0); % Pointer for transferred bytes
    bufferPtr = libpointer('uint8Ptr', zeros(1,256, 'uint8'));
    bufferPtr.Value(1) = 0x88;
    rData = randi([0, 0xff], 5,1);
    bufferPtr.Value(2) = rData(1);
    bufferPtr.Value(3) = rData(2);
    bufferPtr.Value(4) = rData(3);
    bufferPtr.Value(5) = rData(4);
    bufferPtr.Value(6) = rData(5);
    % bufferPtr.Value(7) = 0x55;
    % bufferPtr.Value(8) = 0xaa;
    % bufferPtr.Value(9) = 0x55;
    bytesToTransfer = 6;

    string(dec2hex(bufferPtr.Value(2:bytesToTransfer)))'

    % Call the I2C_DeviceWrite function to set the register address
    optW = bitor(I2C_TRANSFER_OPTIONS_START_BIT, I2C_TRANSFER_OPTIONS_STOP_BIT, 'uint32');
    % optW = bitor(optW, I2C_TRANSFER_OPTIONS_FAST_TRANSFER, 'uint32');
    optW = bitor(optW, I2C_TRANSFER_OPTIONS_BREAK_ON_NACK, 'uint32');
    status = calllib(LN, 'I2C_DeviceWrite', ...
        ftHandle, ...
        uint8(0x20), ...
        bytesToTransfer, ...
        bufferPtr, ...
        bytesTransfered, ...
        optW);

     if status ~= 0
         fprintf("test112():레지스터 쓰기 실패 status = %d\n", status);
     end
     % if bytesToTransfer ~= bytesTransfered.Value
     %     disp("데이터 전송 실패");
     % end     

end


function test111(LN, ftHandle)
    PCA9698_I2C_ADDRESS = 0x20;
    I2C_TRANSFER_OPTIONS_START_BIT = 0x00000001;
    I2C_TRANSFER_OPTIONS_STOP_BIT = 0x00000002;
    I2C_TRANSFER_OPTIONS_BREAK_ON_NACK = 0x00000004;
    I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE = 0x00000008;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER = 0x00000030;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES = 0x00000010;
    I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BITS = 0x00000020;
    I2C_TRANSFER_OPTIONS_NO_ADDRESS = 0x00000040;


    bytesTransfered = libpointer('uint32Ptr', 0); % Pointer for transferred bytes
    bufferPtr = libpointer('uint8Ptr', zeros(1,256, 'uint8'));
    bufferPtr.Value(1) = 0x88;
    % bufferPtr.Value(2) = 0x88;
    bytesToTransfer = 1;

    % Call the I2C_DeviceWrite function to set the register address
    optW = bitor(I2C_TRANSFER_OPTIONS_START_BIT, I2C_TRANSFER_OPTIONS_STOP_BIT, 'uint32');
    % optW = bitor(optW, I2C_TRANSFER_OPTIONS_FAST_TRANSFER, 'uint32');
    optW = bitor(optW, I2C_TRANSFER_OPTIONS_BREAK_ON_NACK, 'uint32');
    status = calllib(LN, 'I2C_DeviceWrite', ...
        ftHandle, ...
        uint8(0x20), ...
        bytesToTransfer, ...
        bufferPtr, ...
        bytesTransfered, ...
        optW);

     if status ~= 0
         fprintf("test111():레지스터 쓰기 실패 status = %d\n", status);
     end
     % if bytesToTransfer ~= bytesTransfered.Value
     %     disp("데이터 전송 실패");
     % end     

    % Call the I2C_DeviceRead function
    bytesToTransfer = 5;
    bytesTransfered.Value = 0;
    optR = bitor(I2C_TRANSFER_OPTIONS_START_BIT,I2C_TRANSFER_OPTIONS_STOP_BIT, 'uint32');
    % optR = bitor(optR, I2C_TRANSFER_OPTIONS_FAST_TRANSFER, 'uint32');
    optR = bitor(optR, I2C_TRANSFER_OPTIONS_BREAK_ON_NACK, 'uint32');
    status = calllib(LN, 'I2C_DeviceRead', ...
        ftHandle, ...
        uint8(0x20), ...
        bytesToTransfer, ...
        bufferPtr, ...
        bytesTransfered, ...
        optR);

     if status ~= 0
         fprintf("test111():레지스터 읽기 실패 status = %d\n", status);
     end
     % if bytesToTransfer ~= bytesTransfered.Value
     %     disp("데이터 전송 실패");
     % end
     string(dec2hex(bufferPtr.Value(1:bytesToTransfer)))'
end

% function [r, rLen ]= composeBufferPointer(data)
%     arguments (Input)
%         data (1,:)
%     end
%     arguments(Output)
%         r
%         rLen (1,1) 
%     end
%     r = libpointer('uint8Ptr', zeros(1,256, 'uint8'));
%     rLen = min(256, numel(data));
%     for idx = 1:rLen
%         r.Value(idx) = data(idx);
%     end
% 
% end
% 
% function data = decomposeBufferPointer(R)
%     data = R.Value;
% end