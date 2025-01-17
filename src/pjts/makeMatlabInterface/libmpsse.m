function [methodinfo,structs,enuminfo,ThunkLibName]=libmpsse
%LIBMPSSE Create structures to define interfaces found in 'all'.

%This function was generated by loadlibrary.m parser version  on Thu Jan 16 21:56:16 2025
%perl options:'all.i -outfile=libmpsse.m -thunkfile=mpsse_thunk_pcwin64.c -header=all.h'
ival={cell(1,0)}; % change 0 to the actual number of functions to preallocate the data.
structs=[];enuminfo=[];fcnNum=1;
fcns=struct('name',ival,'calltype',ival,'LHS',ival,'RHS',ival,'alias',ival,'thunkname', ival);
MfilePath=fileparts(mfilename('fullpath'));
ThunkLibName=fullfile(MfilePath,'mpsse_thunk_pcwin64');
%  void __stdcall Init_libMPSSE ( void ); 
fcns.thunkname{fcnNum}='voidvoidThunk';fcns.name{fcnNum}='Init_libMPSSE'; fcns.calltype{fcnNum}='Thunk'; fcns.LHS{fcnNum}=[]; fcns.RHS{fcnNum}=[];fcnNum=fcnNum+1;
%  void __stdcall Cleanup_libMPSSE ( void ); 
fcns.thunkname{fcnNum}='voidvoidThunk';fcns.name{fcnNum}='Cleanup_libMPSSE'; fcns.calltype{fcnNum}='Thunk'; fcns.LHS{fcnNum}=[]; fcns.RHS{fcnNum}=[];fcnNum=fcnNum+1;
%  FT_STATUS __stdcall Ver_libMPSSE ( LPDWORD libmpsse , LPDWORD libftd2xx ); 
fcns.thunkname{fcnNum}='ulongvoidPtrvoidPtrThunk';fcns.name{fcnNum}='Ver_libMPSSE'; fcns.calltype{fcnNum}='Thunk'; fcns.LHS{fcnNum}='ulong'; fcns.RHS{fcnNum}={'uint32Ptr', 'uint32Ptr'};fcnNum=fcnNum+1;
methodinfo=fcns;