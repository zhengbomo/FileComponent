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
		/** 写文件，返回值：0=失败，1=成功，2=写入长度不正确 */
		static int WriteFile(Platform::String^ pathOfFolder, const Platform::Array<byte>^ data);

	public :
		static Platform::Array<byte>^ ReadFile(Platform::String^ pathOfFolder);

	public:
		static Platform::Array<Platform::String^>^ GetSubFiles(Platform::String^ pathOfFolder);

	public:
		static Platform::Array<Platform::String^>^ GetSubDirectories(Platform::String^ pathOfFolder);

	public:
		static int RenameFolder(Platform::String^ source, Platform::String^ dest);

	public:
		static int RenameFile(Platform::String^ source, Platform::String^ dest);

	public:
		static Platform::Array<Platform::String^>^ GetAllFiles(Platform::String^ pathOfFolder);
	};
}
