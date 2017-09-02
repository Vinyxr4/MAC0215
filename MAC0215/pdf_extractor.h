//g++ -I/usr/include/python2.7 run_python.cpp -o run_python -lpython2.7

#ifndef PDF_EXTRACTOR_H
#define PDF_EXTRACTOR_H

#include <Python.h>
#include <stdlib.h>
#include <string>

// Set enviroment to init pdf_extractor.py use.
PyObject *init_extraction ();

// Closes channel for pdf_extractor.py use.
void end_extraction (PyObject *pModule);

// Calls extractor constructor from pdf_extractor, returning
// the number of pages.
int extract (PyObject *pModule, std::string pdf_path);

// Calls extract_text method from pdf_extractor, returning
// the page text.
std::string extract_text (PyObject *pModule, int page);

// Calls extract_bbox method from pdf_extractor, returning
// an array to be structured as tuples of 4 doubles, beeing
// lower left and upper right bounds of the text chars.
double *extract_bbox (PyObject *pModule, int page);

#endif
