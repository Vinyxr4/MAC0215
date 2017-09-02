//g++ -I/usr/include/python2.7 run_python.cpp -o run_python -lpython2.7

#include <Python.h>
#include <stdlib.h>
#include <string>

int extract (PyObject *pModule);
std::string extract_text (PyObject *pModule, int page);
double *extract_bbox (PyObject *pModule, int page);

int main (int argc, char *argv[]) {
	PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int num_pages;
	double *bbox;

    setenv("PYTHONPATH",".",1);

    Py_Initialize();
    pName = PyString_FromString("pdf_extractor");

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    num_pages = extract (pModule);

    std::string ans = extract_text (pModule, 0);
    bbox = extract_bbox (pModule, 0);

    Py_DECREF(pModule);
    Py_Finalize();

    free (bbox);

	return 0;
}

int extract (PyObject *pModule) {
    PyObject *pFunc, *pArgs, *pValue;
    int num_pages;

	pFunc = PyObject_GetAttrString (pModule, "extract");

    pArgs = PyTuple_New (1);
    
    pValue = PyString_FromString ("/home/viniciuspd/Desktop/lista_1.pdf");
    PyTuple_SetItem(pArgs, 0, pValue);

    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    num_pages = (int) PyLong_AsLong (pValue);
    Py_DECREF(pValue);

    Py_XDECREF(pFunc);

    return num_pages;
}

std::string extract_text (PyObject *pModule, int page) {
    PyObject *pFunc, *pArgs, *pValue;
    std::string text;

	pFunc = PyObject_GetAttrString (pModule, "extract_text");

    pArgs = PyTuple_New (1);
    
    pValue = PyInt_FromLong (page);
    PyTuple_SetItem(pArgs, 0, pValue);

    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    text = PyString_AsString (pValue);
    
    Py_DECREF(pValue);
    Py_XDECREF(pFunc);

    return text;
}

double *extract_bbox (PyObject *pModule, int page) {
    PyObject *pFunc, *pArgs, *pValue;
    PyObject *ptemp, *objectsRepresentation;
    double *bbox;
    char *a11;

	pFunc = PyObject_GetAttrString (pModule, "extract_positions");

    pArgs = PyTuple_New (1);
    
    pValue = PyInt_FromLong (page);
    PyTuple_SetItem(pArgs, 0, pValue);

    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    bbox = (double*)  malloc (PyList_Size (pValue) * sizeof (double));
    for (int i = 0; i < PyList_Size (pValue); ++i) {
    	ptemp = PyList_GetItem(pValue,i);
        objectsRepresentation = PyObject_Repr (ptemp);
        a11 = PyString_AsString (objectsRepresentation);
        bbox[i] = (double) strtod (a11,NULL);
    }

    Py_DECREF(pValue);
    Py_XDECREF(pFunc);

    return bbox;
}