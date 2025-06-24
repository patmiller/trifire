#include <Python.h>

// The C function to be exposed
static PyObject* foo(PyObject* self, PyObject* args) {
    return PyUnicode_FromString("hello from C");
}

// Method table
static PyMethodDef MyMethods[] = {
    {"foo", foo, METH_NOARGS, "Returns a greeting string"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    "mymodule",       // name of module
    NULL,             // module documentation (optional)
    -1,               // size of per-interpreter state of the module
    MyMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_mymodule(void) {
    return PyModule_Create(&mymodule);
}
