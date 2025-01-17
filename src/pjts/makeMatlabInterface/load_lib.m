% libmpsse파일이 있으면, 해당 library를 load한다.
if ~libisloaded('libmpsse')
    if exist('libmpsse', 'file') ~= 2
        loadlibrary('libmpsse.dll', 'all.h', 'alias', 'mpsse', 'mfilename', 'libmpsse');
    else
        loadlibrary('libmpsse.dll',@libmpsse);
    end
end
libfunctionsview libmpsse
