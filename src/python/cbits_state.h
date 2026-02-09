#ifndef CBITS_STATE_H
#define CBITS_STATE_H

#include "cbits_module.h"

typedef struct {
    PyTypeObject *PyBitVectorType;
    PyTypeObject *PyBitVectorIterType;
} cbits_state;

static inline cbits_state *
get_cbits_state(PyObject *module)
{
    return (cbits_state *) PyModule_GetState(module);
}
#define find_cbits_state_by_type(type) \
    (get_cbits_state(PyType_GetModuleByDef(type, &cbits_module)))
#define py_bitvector_check(object, state) \
    PyObject_TypeCheck(object, state->PyBitVectorType)

#endif /* CBITS_STATE_H */
