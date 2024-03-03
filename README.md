# Windows Volume Persist

修复windows系统在某些设备上无法持久化保存蓝牙耳机音量的问题，C++17标准。

## 核心原理

使用`核心音频API`实现音量控制

利用COM对象和系统交互，完成音量的获取、设置

[点我查看微软官方文档](https://learn.microsoft.com/zh-cn/windows/win32/coreaudio/endpointvolume-api)

## 实现

程序主体是windows窗口程序，使用`ShowWindow(hwnd, SW_HIDE)`使得窗口隐藏，使用`SetTimer(hwnd, 1, 3000, NULL)`创建每隔3秒触发一次定时器，从而在窗口消息循环里面也能定时执行检测音量信息的业务。

使用`COM`对象`IMMDeviceEnumerator`和`IMMDevice`实现音频设备的枚举和选择，使用`IAudioEndpointVolume`实现音量的获取和设置

1. `deviceEnumerator->GetDefaultAudioEndpoint`获取默认音频设备
2. `endpointVolume->GetMasterVolumeLevelScalar`获取系统音量
3. `endpointVolume->GetMute`获取是否静音
4. `defaultDevice->GetId`获取音频设备的ID
5. `endpointVolume->SetMasterVolumeLevelScalar`设置音量
6. `endpointVolume->SetMute`设置静音状态

使用map维护音频设备ID到音量设置的映射，这样子每一个音频设备都能有独立的音量

当map有更改时，每隔60秒检测一次map的更改状态，然后存入文件

> wcstr的音频ID直接BASE64编码。这样子哪怕ID里面有空格/换行符/Unicode字符，也能简单的用标准库直接存取。

## 效果

程序运行后会在后台运行，根据不同的状态智能选择是否覆写系统目前的音量：

1. 第一次打开程序时，记录当前系统的音量。如果系统音量大于90%，那么就先设置为40%再记录系统的音量。
2. 不是第一次打开程序，但是目前是新设备，那么记录当前系统的音量。如果系统音量大于90%，那么就先设置为40%再记录系统的音量。
3. 如果都不是的话，那么检测当前系统的音量和存储的音量大小相差是否超过1%，超过1%就把系统的音量改为存储的音量。
4. 每隔3秒检测一次音量的变动
   
   如果设备不变：
   
   检测当前的音量设置是否和上一次获取的有变动，如果有变动才更改存储的音量

   如果设备变动：

   如果设备已经被记录过，那么检测当前系统的音量和存储的音量大小相差是否超过1%，超过1%就把系统的音量改为存储的音量。

   如果是新设备，那么记录当前系统的音量。如果系统音量大于90%，那么就先设置为40%再记录系统的音量。
5. 每隔60秒检测一次map的更改状态，如果更改了就存入文件。

   > 这么做是为了缓存短时间内用户的多次更改
6. 具有完整的错误处理，任意一步出现异常都会弹出消息框提示用户并且程序自动退出。
7. 使用条件编译，可以在编译前设置是否编译出带日志输出的版本，方便出现问题时编译带日志的版本排错。