#include <stdio.h>
#include <Python.h>
#include "../common/AIpython.h"

void Python_init(void)
{
	Py_Initialize();//��Ҫ�ǳ�ʼ��python��������
    PyRun_SimpleString("import sys");//�൱����python�е�import sys��䡣
    PyRun_SimpleString("sys.path.append('../python')");//�ǽ�����·������Ϊ��ǰĿ¼��
}

int BaiduRequest(char *module, char *func, char *arg)
{
	Py_Initialize();//��Ҫ�ǳ�ʼ��python��������
    PyRun_SimpleString("import sys");//�൱����python�е�import sys��䡣
    PyRun_SimpleString("sys.path.append('../python')");//�ǽ�����·������Ϊ��ǰĿ¼��
    PyObject *pModule=NULL;
    PyObject *pFunc=NULL;
    PyObject *pArg = NULL;
    pModule = PyImport_ImportModule(module);//�����õ����ļ�������helloWorld.py��������
    pArg  =  Py_BuildValue("(filePath)", arg);
    pFunc = PyObject_GetAttrString(pModule,func);//����pyton����ģ��helloWorld.py�в���hello������
    PyEval_CallObject(pFunc,pArg);//����helloģ��
    Py_Finalize();//����python�����ͷ���Դ
	return 0;
}



