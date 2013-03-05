/* -*- mode: C++; fill-column: 100; -*-
 *
 * This extension module exports the isaac type, which can generate pseudo-random numbers using Bob
 * Jenkins' ISAAC algorithm, described here:
 *
 * http://www.burtleburtle.net/bob/rand/isaacafa.html
 *
 * dmb - June 2012 - Copyright (C) 2012 Arcode Corporation
 */
#include <Python.h>
#include "structmember.h"
#include "rand.h"

typedef struct {
    PyObject_HEAD

    randctx ctx;
} isaac_t;

static PyObject *
isaac_new(PyTypeObject *type, PyObject *args, PyObject *kw)
{
    return type->tp_alloc(type, 0);
}

static int
isaac_init(isaac_t *self, PyObject *args, PyObject *kwds)
{
    int i, pos = 0;
    const char *s;
    Py_ssize_t len;
    ub4 value;

    if (!PyArg_ParseTuple(args, "s#", &s, &len)) {
	PyErr_SetString(PyExc_ValueError, "must provide seed string");
	return -1;
    }	

    for (i = 0; i < RANDSIZ; i++) {
	value = 0;
	if (pos < len)
	    value |= s[pos++];
	if (pos < len)
	    value |= s[pos++] << 8;
	if (pos < len)
	    value |= s[pos++] << 16;
	if (pos < len)
	    value |= s[pos++] << 24;
	self->ctx.randrsl[i] = value;
    }

    /* create initial state */
    randinit(&self->ctx, TRUE);

    return 0;
}

#define REFILL(ctx, R) if (!ctx->randcnt) { isaac(ctx); ctx->randcnt = RANDSIZ; R = ctx->randrsl; } else

static PyObject *
isaac_rand(PyObject *self)
{
    isaac_t *it = (isaac_t *) self;
    randctx *ctx = &it->ctx;
    ub4 *R = ctx->randrsl;
    unsigned long value;
    REFILL(ctx, R);
    value = R[--ctx->randcnt];
    return PyInt_FromSize_t((size_t) value);
}

static PyObject *
isaac_random_string(PyObject *self, PyObject *args)
{
    isaac_t *it = (isaac_t *) self;
    randctx *ctx = &it->ctx;
    ub4 *R = ctx->randrsl;
    ub4 v;
    int i, N = 0;
    unsigned char *allocated, *s;
    PyObject *S;

    if (!PyArg_ParseTuple(args, "i", &N))
        return NULL;

    s = allocated = PyMem_Malloc(N);
    for (i = N; i >= 4; i -= 4) {
	REFILL(ctx, R);
	v = R[--ctx->randcnt];
	*s++ = (unsigned char) ((v >> 24) & 0xFF);
	*s++ = (unsigned char) ((v >> 16) & 0xFF);
	*s++ = (unsigned char) ((v >>  8) & 0xFF);
	*s++ = (unsigned char) v & 0xFF;
    }
    if (i >= 2) {
	REFILL(ctx, R);
	v = R[--ctx->randcnt];
	*s++ = (unsigned char) ((v >> 8) & 0xFF);
	*s++ = (unsigned char) v & 0xFF;
	i -= 2;
    }
    if (i) {
	REFILL(ctx, R);
	v = R[--ctx->randcnt];
	*s++ = (unsigned char) v & 0xFF;
    }

    S = PyString_FromStringAndSize((const char *) allocated, N);
    PyMem_Free(allocated);
    return S;
}

static void
isaac_free(PyObject *self)
{
    Py_TYPE(self)->tp_free(self);
}

static PyMemberDef isaac_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef isaac_methods[] = {
    {"rand", (PyCFunction) isaac_rand, METH_NOARGS, "Generate 32-bit pseudo-random value." },
    {"random_string", (PyCFunction) isaac_random_string, METH_VARARGS, "Generate string of random bytes." },
    {NULL}  /* Sentinel */
};

static PyTypeObject isaacType = {
    PyObject_HEAD_INIT(NULL)
    0,				/*ob_size*/
    "_isaac._isaac",		/*tp_name*/
    sizeof(isaac_t),  		/*tp_basicsize*/
    0,                         /*tp_itemsize*/
    isaac_free,     		/*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,		       	    /*tp_compare*/
    0,                         /*tp_repr*/
    0,			         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,		       	       /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "_isaac objects",		/* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    isaac_methods,  		/* tp_methods */
    isaac_members,  		/* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)isaac_init,      /* tp_init */
    0,                        /* tp_alloc */
    isaac_new,     /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC
init_isaac(void) 
{
    PyObject* m;

    if (PyType_Ready(&isaacType) < 0)
        return;

    m = Py_InitModule3("_isaac", module_methods, "C isaac implementation");

    if (m == NULL)
	return;

    Py_INCREF(&isaacType);
    PyModule_AddObject(m, "_isaac", (PyObject *) &isaacType);
}
