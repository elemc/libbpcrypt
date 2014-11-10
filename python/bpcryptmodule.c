// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (bpcryptmodule)
   Description: 
*/

//#include "bpcryptmodule.h"
#include <Python.h>
#include <libbpcrypt/crypt.h>

#if PY_MAJOR_VERSION >= 3
#define OUTPUT_FORMAT "y#"
#else
#define OUTPUT_FORMAT "s#"
#endif

static PyObject *
crypt_encrypt_buffer( PyObject *self, PyObject *args )
{
    char *buffer, *key;
    int buffer_size, key_size;
   
    if ( !PyArg_ParseTuple( args, "z#z#", &buffer, &buffer_size, &key, &key_size ) )
        return NULL;

    bp_buffer_t *result;
    bp_size_t result_size;

    result = encrypt_buffer( (bp_buffer_t *)buffer, buffer_size, (bp_buffer_t *)key, key_size,  &result_size );

    return Py_BuildValue( OUTPUT_FORMAT, (const char *)result, result_size ); //, result_size );
}

static PyObject *
crypt_decrypt_buffer( PyObject *self, PyObject *args )
{
    char *buffer, *key;
    int buffer_size, key_size;
   
    if ( !PyArg_ParseTuple( args, "z#z#", &buffer, &buffer_size, &key, &key_size ) )
        return NULL;

    bp_buffer_t *result;
    bp_size_t result_size;

    result = decrypt_buffer( (bp_buffer_t *)buffer, buffer_size, (bp_buffer_t *)key, key_size,  &result_size );

    return Py_BuildValue( OUTPUT_FORMAT, (const char *)result, result_size ); //, result_size );
}

static PyObject *
misc_BP_to_hex( PyObject *self, PyObject *args )
{
    char *buffer;
    int buffer_size;
   
    if ( !PyArg_ParseTuple( args, "z#", &buffer, &buffer_size ) )
        return NULL;

    bp_size_t result_size;
    bp_buffer_t *result = BP_to_hex( (bp_buffer_t *)buffer, buffer_size, &result_size );

    return Py_BuildValue( "s#", (const char *)result, result_size );
}

static PyMethodDef bpcrypt_funcs[] = {
    { "encrypt_buffer", (PyCFunction)crypt_encrypt_buffer,  METH_VARARGS, "encrypt string buffer with key" },
    { "decrypt_buffer", (PyCFunction)crypt_decrypt_buffer,  METH_VARARGS, "decrypt string buffer with key" },
    { "BP_to_hex",      (PyCFunction)misc_BP_to_hex,        METH_VARARGS, "convert byte array to hex string" }, 
    { NULL, NULL, 0, NULL }
};

#if PY_MAJOR_VERSION >= 3
static char bpcrypt_docs[] = "Python module for libbpcrypt";
/* \
     encrypt_buffer( buffer, key ): encrypt some string buffer with key\n \
     decrypt_buffer( buffer, key ): decrypt some encrypted string buffer with key\n"; */

static struct PyModuleDef bpcrypt_def = {
   PyModuleDef_HEAD_INIT,
   "bpcrypt",   /* name of module */
   bpcrypt_docs, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   bpcrypt_funcs
};
#endif

PyMODINIT_FUNC

#if PY_MAJOR_VERSION >= 3
PyInit_bpcrypt(void)
{
    PyObject *m;
    m = PyModule_Create(&bpcrypt_def);
    if ( m == NULL )
        return NULL;
    return m;
#else
initbpcrypt(void)
{
    PyObject *m;
    m = Py_InitModule3("bpcrypt", bpcrypt_funcs, "Python module for libbpcrypt");    
    if (m == NULL)
        return;
#endif
}

