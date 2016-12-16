#pragma once

namespace FileComponent
{
    public ref class NativeFileHelper sealed
    {
	public:
		static uint64 GetFolderSize(Platform::String^ pathOfFolder);

	public:
		static uint64 GetFileSize(Platform::String^ pathOfFile);
	
	public:
		static int FileExists(Platform::String^ path);

	public:
		static int DirectoryExists(Platform::String^ pathOfFolder);

	public:
		static int RemoveFile(Platform::String^ pathOfFile);

	public:
		static int RemoveDirectory(Platform::String^ pathOfFolder);

	public:
		/** return value：0=fail，1=success，2=fail:uncorrect length */
		static int WriteFile(Platform::String^ pathOfFolder, const Platform::Array<byte>^ data);
	
	public :
		static Platform::Array<byte>^ ReadFile(Platform::String^ pathOfFolder);

	public:
		static Platform::Array<Platform::String^>^ GetSubFiles(Platform::String^ pathOfFolder);

	public:
		static Platform::Array<Platform::String^>^ GetSubDirectories(Platform::String^ pathOfFolder);
/*
	public:
		static int RenameItem(Platform::String^ source, Platform::String^ dest);*/

	public:
		static Platform::Array<Platform::String^>^ GetAllFiles(Platform::String^ pathOfFolder);
	};
}
