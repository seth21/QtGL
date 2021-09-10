#ifdef PYTHON_USE

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
// #include "pystate.h"
// #include "pyport.h"
// #include "internal/pycore_pystate.h"

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "mainwindow.h"
#include <QWidget>
#include <QApplication>


typedef struct {
	PyObject_HEAD
	/* Type-specific fields go here. */
	std::vector<QWidget *> windows;
	std::string resourceFolder;
	int started;
} Application;


static void
Application_dealloc(Application *self)
{
	for (std::vector<QWidget *>::iterator it = self->windows.begin(); it !=  self->windows.end(); ++it) {
		if (*it != 0) delete (*it);
	}
	Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
Application_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Application *self;
	self = (Application *) type->tp_alloc(type, 0);

	if (self != NULL) {
	   //TODO temp
	}
	return (PyObject *) self;
}

static int
Application_init(Application *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"resources", "title", NULL};
	const char *resources, *title;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|s", kwlist, &resources, &title)) {	// The title thing will go to window Objects
		return -1;
	}
	self->resourceFolder = resources;
	if (title) {
		// TODO
	}

	return 0;
}

static PyMemberDef Application_members[] = {
	// {"windows", T_OBJECT_EX, offsetof(Application, windows), 0,
	//  "Windows to show upon Application execution"},
	{"started", T_INT, offsetof(Application, started), 0,
	 "Exec has been called"},
	{NULL}  /* Sentinel */
};


static PyObject *
Application_exec(Application *self)
{
	// TODO temp
	// if (self->windows.size() < 1) {
	// 	PyErr_SetString(PyExc_AttributeError, "windows");
	// 	return NULL;
	// }

	// Retrieve executable name to pass to QApplication
	wchar_t *pythonExe = Py_GetPath();
	int len = wcslen(pythonExe);
	char *argv[2] = {new char [len], new char [self->resourceFolder.length()]};
	wcstombs(argv[0], pythonExe, len);
	strcpy(argv[1], self->resourceFolder.c_str());
	int argc = 2;
	QApplication application(argc, argv);
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	// TODO temp:
	self->windows.push_back(new MainWindow());
	for (std::vector<QWidget *>::iterator it = self->windows.begin(); it !=  self->windows.end(); ++it) {
		if (*it != 0) (*it)->show();
	}
	self->started = 1;
	int retVal = application.exec();
	if (&application) {
		delete &application;
	}
	// std::cout << static_cast<QObject *>(self->windows[0])->metaObject()->className();
	return PyLong_FromLong(retVal);
}

static PyMethodDef Application_methods[] = {
	{"exec", (PyCFunction) Application_exec, METH_NOARGS,
	 "Show windows and start Application"
	},
	{NULL}  /* Sentinel */
};


static PyTypeObject ApplicationType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "mytest.Application",
	.tp_basicsize = sizeof(Application),
	.tp_itemsize = 0,
	.tp_dealloc = (destructor) Application_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_doc = "Qt Application",
	.tp_methods = Application_methods,
	.tp_members = Application_members,
	.tp_init = (initproc) Application_init,
	.tp_new = Application_new,
};

static PyModuleDef mytestmodule = {
	PyModuleDef_HEAD_INIT,
	.m_name = "mytest",
	.m_doc = "Test module that creates an Qt Application type.",
	.m_size = -1,
};

PyMODINIT_FUNC
PyInit_mytest(void)
{
	PyObject *m;
	if (PyType_Ready(&ApplicationType) < 0)
		return NULL;

	// std::cout << "PyType ready!";
	m = PyModule_Create(&mytestmodule);
	if (m == NULL)
		return NULL;

	// std::cout << "Module created!";
	Py_INCREF(&ApplicationType);
	if (PyModule_AddObject(m, "Application", (PyObject *) &ApplicationType) < 0) {
		Py_DECREF(&ApplicationType);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}

#endif