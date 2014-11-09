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

static PyObject *
crypt_encrypt_buffer( PyObject *self, PyObject *args )
{
    const char *buffer, *key;
    
    if ( !PyArg_ParseTuple( args, "ss", &buffer, &key ) )
        return NULL;

    bp_buffer_t *result;
    bp_size_t result_size;
    result = encrypt_buffer( (bp_buffer_t *)buffer, strlen(buffer), 
                             (bp_buffer_t *)key, strlen(key),
                             &result_size );

    bp_buffer_t *null = result + result_size;
    *null = '\0';

    return Py_BuildValue( "s", result );
}

static PyObject *
crypt_decrypt_buffer( PyObject *self, PyObject *args )
{
    const char *buffer, *key;
    

    if ( !PyArg_ParseTuple( args, "ss", &buffer, &key ) )
        return NULL;

    bp_buffer_t *result;
    bp_size_t result_size;
    result = decrypt_buffer( (bp_buffer_t *)buffer, strlen(buffer), 
                             (bp_buffer_t *)key, strlen(key), 
                             &result_size );

    bp_buffer_t *null = result + result_size;
    *null = '\0';

    return Py_BuildValue( "s", result );
}

static PyMethodDef bpcrypt_funcs[] = {
    { "encrypt_buffer", (PyCFunction)crypt_encrypt_buffer, METH_VARARGS, "encrypt string buffer with key" },
    { "decrypt_buffer", (PyCFunction)crypt_decrypt_buffer, METH_VARARGS, "decrypt string buffer with key" },
    { NULL, NULL, 0, NULL }
};

#if PY_MAJOR_VERSION >= 3
static char bpcrypt_docs[] = " \
     encrypt_buffer( buffer, key ): encrypt some string buffer with key\n \
     decrypt_buffer( buffer, key ): decrypt some encrypted string buffer with key\n";

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

