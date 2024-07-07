# 注意事项

> 使用前需要配置python环境
> 请将.pro文件26-30行地址改为你对应python环境的地址
> 所调用的python文件位于qt生成的.exe 程序所在的文件夹，但python生成的.txt文件位于.exe上一级文件夹
> 由于12306网络限制，运行时请关闭魔法

> 如果Python.h 头文件报错，请将python环境include文件夹object.h

```c
typedef struct{
    const char* name;
    int basicsize;
    int itemsize;
    unsigned int flags;
    PyType_Slot *slots; /* terminated by slot==0. */
} PyType_Spec;
```

改为

```c
#undef slots
typedef struct{
    const char* name;
    int basicsize;
    int itemsize;
    unsigned int flags;
    PyType_Slot *slots; /* terminated by slot==0. */
} PyType_Spec;
#define slots Q_SLOTS
```

并保存