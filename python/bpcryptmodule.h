// -*- C -*-
/* ------------------------------------ */
/* C source header                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (bpcryptmodule)
   Description: 
*/

#ifndef BPCRYPTMODULE_H
#define BPCRYPTMODULE_H


#ifdef __cplusplus
extern "C"{
#endif

#include <Python.h>
#include <libbpcrypt/crypt.h>

static PyObject *encrypt_buffer( PyObject *self, PyObject *args );
static PyObject *decrypt_buffer( PyObject *self, PyObject *args );

/*static char bpcrypt_docs[] = 
    "encrypt_buffer( buffer, key ): encrypt some string buffer with key\n
     decrypt_buffer( buffer, key ): decrypt some encrypted string buffer with key\n";*/

static PyMethodDef bpcrypt_funcs[] = {
    { "encrypt_buffer", (PyCFunction)encrypt_buffer, METH_VARARGS, "encrypt string buffer with key" },
    { NULL, NULL, 0, NULL }
};

#ifdef __cplusplus
}
#endif

#endif

