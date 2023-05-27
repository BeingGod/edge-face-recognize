#!/bin/bash

WorkingDir="/data/local/tmp/<project>"
adb shell "mkdir -p ${WorkingDir}"

adb shell "killall lldb-server"
adb push $(find $ANDROID_NDK/ -name lldb-server | grep aarch64) /data/local/tmp/
adb shell "chmod 755 /data/local/tmp/lldb-server"

# vscode setting problemMatcher regexp
echo "lldb-server start"

# 启动server之前需要使用cd命令改变WorkingDir
adb shell "cd ${WorkingDir} && /data/local/tmp/lldb-server p --server --listen unix-abstract://${WorkingDir}/<project>.debug.sock"