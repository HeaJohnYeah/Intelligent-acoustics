#include <stdio.h>
#include <Python.h>
#include "../common/AIpython.h"

void Python_init(void)
{
	Py_Initialize();//主要是初始化python解释器。
    PyRun_SimpleString("import sys");//相当于在python中的import sys语句。
    PyRun_SimpleString("sys.path.append('../python')");//是将搜索路径设置为当前目录。
}

int BaiduRequest(char *module, char *func, char *arg)
{
	Py_Initialize();//主要是初始化python解释器。
    PyRun_SimpleString("import sys");//相当于在python中的import sys语句。
    PyRun_SimpleString("sys.path.append('../python')");//是将搜索路径设置为当前目录。
    PyObject *pModule=NULL;
    PyObject *pFunc=NULL;
    PyObject *pArg = NULL;
    pModule = PyImport_ImportModule(module);//是利用导入文件函数将helloWorld.py函数导入
    pArg  =  Py_BuildValue("(filePath)", arg);
    pFunc = PyObject_GetAttrString(pModule,func);//是在pyton引用模块helloWorld.py中查找hello函数。
    PyEval_CallObject(pFunc,pArg);//调用hello模块
    Py_Finalize();//清理python环境释放资源
	return 0;
}



