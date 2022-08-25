.. _python_basics:

#################################################
Basics of Creating a PluginPlay::Module in Python
#################################################

.. note::

   PluginPlay allows you to create PluginPlay::Module instances in Python
   and/or in C++. This page details the Python procedure. See :ref:`cpp_basics`
   for the C++ procedure.

.. warning::

   Creating PluginPlay::Module instances in Python is relatively new. These
   instructions detail the current procedure and are subject to change as
   infrastructure and best practices evolve.

Like the C++ tutorial we assume the setup described in :ref:`e_and_m_101`. We
additionally assume that Python bindings were generated for the
``ElectricField``, ``Point``, and ``PointCharge`` classes and that these
bindings live in the ``pluginplay_examples`` Python module (and that the
``pluginplay_examples`` Python module is in your Python path).

******************
Writing the Module
******************

At the moment the only way to write a PluginPlay::Module in Python is by
deriving from ``pluginplay::ModuleBase``, overriding the constructor, and
implementing the ``run_`` method. In Python our Coulomb's Law module looks
like:

.. literalinclude:: ../../../tests/pluginplay/docs/coulombslaw_force.py
   :language: Python
   :lines: 1,2,5,6-34

For the most part the Python implementation is simply a direct translation of
the C++ implementation (with the macro guts exposed).

.. note::

   There's an open
   `issue <https://github.com/NWChemEx-Project/PluginPlay/issues/273>`__
   designed to clean this process up. Even after Issue 273 has been
   resolved the inheritance method will continue to work, it just
   won't be the preferred mechanism.
