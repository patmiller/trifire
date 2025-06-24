// Copyright 2025 - Patrick Miller

#include <Python.h>

#include "bmp.h"
#include "bricks.h"
#include "play.h"
#include "render.h"
#include "state.h"

PyObject* json = NULL;


static PyObject* trifire_init(PyObject* self, PyObject* _) {
  struct State state = DEFAULT_STATE;
  char buffer[1024];
  if (!string_state(&state, buffer, sizeof(buffer))) {
    return PyErr_Format(PyExc_RuntimeError,
                        "Buffer too small %u", sizeof(buffer));
  }

  return PyObject_CallMethod(json, "loads", "s", buffer);
}

static int object_to_state(PyObject* config, struct State* state) {
  // Convert dict to a string via JSON
  PyObject* /*owned*/ s = PyObject_CallMethod(json, "dumps", "O", config);
  if (!s) return 0;

  // Get the UTF8 string value (owned by the unicode object)
  const char* json_str = NULL;
  Py_ssize_t size = 0;
  json_str = PyUnicode_AsUTF8AndSize(s, &size);
  if (!json_str) {
    Py_DECREF(s);
    return 0;
  }

  // Convert that to a state structure
  if (!read_state_string(json_str, state)) {
    Py_DECREF(s);
    PyErr_Format(PyExc_RuntimeError, "invalid state");
    return 0;
  }
  Py_DECREF(s);  // String no longer needed

  return 1;
}

static PyObject* trifire_play(PyObject* self, PyObject* args) {
  PyObject* config /*borrowed*/ = NULL;
  const char* command = NULL;
  if ( !PyArg_ParseTuple(args, "Os", &config, &command) ) {
    return NULL;
  }

  struct State state = DEFAULT_STATE;
  if ( !object_to_state(config, &state) ) {
    return NULL;
  }

  // After all that, just play the game!
  if ( !play(&state, command[0]) ) {
    return PyErr_Format(PyExc_RuntimeError, "invalid play");
  }

  // Convert state back to a string and then that string back
  // do a dict via JSON
  char buffer[1024];
  if ( !string_state(&state, buffer, sizeof(buffer)) ) {
    return PyErr_Format(PyExc_RuntimeError, "invalid state pack");
  }
  return PyObject_CallMethod(json, "loads", "s", buffer);
}

// This takes the state object and renders a BMP of RGB into a
// Python list object
static PyObject* trifire_render(PyObject* self, PyObject* config) {
  // Get a state object from the json dict
  struct State state = DEFAULT_STATE;
  if ( !object_to_state(config, &state) ) {
    return NULL;
  }

  // Make a copy of the brick
  uint32_t rgb[BRICKS_WIDTH*BRICKS_HEIGHT];
  for (unsigned i = 0; i < BRICKS_WIDTH*BRICKS_HEIGHT; ++i) {
    rgb[i] = bricks[i];
  }
  if (!render(rgb, BRICKS_WIDTH, BRICKS_HEIGHT, &state)) {
    return PyErr_Format(PyExc_RuntimeError, "could not render");
  }
  PyObject* pixels /*owned*/ = PyList_New(BRICKS_WIDTH*BRICKS_HEIGHT);
  for (unsigned i = 0; i < BRICKS_WIDTH*BRICKS_HEIGHT; ++i) {
    PyObject* pixel = PyLong_FromLong(rgb[i]);
    if (!pixel) {
      Py_DECREF(pixels);
      return NULL;
    }
    PyList_SET_ITEM(pixels, i, pixel);  // No error checking here
  }

  return pixels;
}

// This takes the state object and renders a BMP of RGB into a
// bytes blob representing the BMP file format
static PyObject* trifire_bmp(PyObject* self, PyObject* config) {
  // Get a state object from the json dict
  struct State state = DEFAULT_STATE;
  if ( !object_to_state(config, &state) ) {
    return NULL;
  }

  // Make a copy of the brick
  uint32_t rgb[BRICKS_WIDTH*BRICKS_HEIGHT];
  for (unsigned i = 0; i < BRICKS_WIDTH*BRICKS_HEIGHT; ++i) {
    rgb[i] = bricks[i];
  }
  if (!render(rgb, BRICKS_WIDTH, BRICKS_HEIGHT, &state)) {
    return PyErr_Format(PyExc_RuntimeError, "could not render");
  }

  // Now, we build a bytes object out
  unsigned n = bmp_size(BRICKS_WIDTH, BRICKS_HEIGHT);
  uint8_t* /*owned*/ bmp = malloc(n);
  pack_bmp(rgb, BRICKS_WIDTH, BRICKS_HEIGHT, bmp);

  PyObject* result = PyBytes_FromStringAndSize((char*)bmp, n);
  free(bmp);
  return result;
}

// Method table
static PyMethodDef MyMethods[] = {
    {"init", trifire_init, METH_NOARGS, "return the state 0 json"},
    {"play", trifire_play, METH_VARARGS,
     "takes a state json and returns a state json and BMP data"},
    {"render", trifire_render, METH_O,
     "takes a state and returns the 640x480 RGB"},
    {"bmp", trifire_bmp, METH_O,
     "takes a state and returns the 640x480 BMP bytes"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    "trifire",       // name of module
    NULL,             // module documentation (optional)
    -1,               // size of per-interpreter state of the module
    MyMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_trifire(void) {
  json = PyImport_ImportModule("json");
  if (!json) return NULL;
  return PyModule_Create(&mymodule);
}
