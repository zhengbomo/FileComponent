# FileComponent
FileUtil for UWP using WinAPI

the WinRT platform has not provided the Sync for File Operation, and the default API for file operation is very slow, like shit, here is the new File API build for C++/CX, is fast, can be used as synchronized

## Feature
* Fast
* Sync support
* Thread safe (TODO)

## Nuget
TODO

## Usage
we use absoluted file path for operation, no StorageFile or StorageFolder

### Read file
```cs
var path = Path.Combine(Package.Current.InstalledLocation.Path, "Assets\\logo.png");
var data = NativeFileHelper.ReadFile(path);
```

### Write file
```cs
var path = Path.Combine(Package.Current.InstalledLocation.Path, "Assets\\logo.png");
var data = NativeFileHelper.ReadFile(path);

var destPath = Path.Combine(ApplicationData.Current.LocalFolder.Path, "Assets\\logo.png");

// the directory will be created if is not exists
var result = NativeFileHelper.WriteFile(destPath, data);
if (result == 1) {
    Debug.WriteLine("write success")
} else {
    Debug.WriteLine("write fail")
}
```

### File exists
```cs
var path = Path.Combine(Package.Current.InstalledLocation.Path, "Assets\\logo.png");
var result = NativeFileHelper.FileExists(path);
if (result == 1) {
    Debug.WriteLine("file exists")
} else {
    Debug.WriteLine("file not exists")
}
```
### Delete file
```
var path = Path.Combine(ApplicationData.Current.LocalFolder.Path, "Assets\\logo.png");
var result = NativeFileHelper.RemoveFile(path);
if (result == 1) {
    Debug.WriteLine("file remove success")
} else {
    Debug.WriteLine("file remove fail")
}
```

### other API
* public static ulong GetFolderSize([In] string pathOfFolder);
* public static ulong GetFileSize([In] string pathOfFile);
* public static int DirectoryExists([In] string pathOfFolder);
* public static int RemoveDirectoryW([In] string pathOfFolder);
* public static string[] GetSubFiles([In] string pathOfFolder);
* public static string[] GetSubDirectories([In] string pathOfFolder);
* public static string[] GetAllFiles([In] string pathOfFolder);

## Attention
> the API work fine for localfolder（`ApplicationData.Current.LocalFolder` and `Package.Current.InstalledLocation`）, but is not support for User Folder, like Photo, Video, Music Folder（`KnownFolders.CameraRoll` or `KnownFolders.MusicLibrary`）
