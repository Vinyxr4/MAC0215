//g++ -I/usr/include/python2.7 run_python.cpp -o run_python -lpython2.7

#ifndef PDF_EXTRACTOR_H
#define PDF_EXTRACTOR_H

#include <Python.h>
#include <stdlib.h>
#include <string>

class pdf_extractor {
public:
    pdf_extractor (std::string path);

    // Set enviroment to init pdf_extractor.py use.
    void init ();

    // Closes channel for pdf_extractor.py use.
    void end ();

    // Calls extractor constructor from pdf_extractor, returning
    // the number of pages.
    int extract ();

    // Calls extract_text method from pdf_extractor, returning
    // the page text.
    std::string get_text (int page);

    // Calls extract_bbox method from pdf_extractor, returning
    // an array to be structured as tuples of 4 doubles, beeing
    // lower left and upper right bounds of the text chars.
    double *get_bbox (int page);

private:
    std::string pdf_path;
    PyObject *pModule;
};

#endif
