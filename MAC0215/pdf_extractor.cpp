#include "pdf_extractor.h"

pdf_extractor::pdf_extractor (std::string path) {
    pdf_path = path;
}

void pdf_extractor::init () {
    PyObject *pName;

	setenv("PYTHONPATH", ".", 1);

    Py_Initialize();
    pName = PyString_FromString("pdf_extractor");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
}

void pdf_extractor::end () {
    Py_DECREF(pModule);
    Py_Finalize();
}

int pdf_extractor::extract () {
    PyObject *pFunc, *pArgs, *pValue;
    int num_pages;

	pFunc = PyObject_GetAttrString (pModule, "extract");

    pArgs = PyTuple_New (1);
    
    pValue = PyString_FromString (pdf_path.c_str ());
    PyTuple_SetItem(pArgs, 0, pValue);

    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    num_pages = (int) PyLong_AsLong (pValue);
    Py_DECREF(pValue);

    Py_XDECREF(pFunc);

    return num_pages;
}

std::string pdf_extractor::get_text (int page) {
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

double *pdf_extractor::get_bbox (int page) {
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
