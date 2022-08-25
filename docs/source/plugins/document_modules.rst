***********************************
Documenting Modules in a Collection
***********************************

This page shows how to document the modules in your module collection using the
``document_modules`` function in the SDE. This page assumes that you can create
a module collection according to :doc:`./overview` and :doc:`load_modules`.

Generate the Documentation
==========================

This section explains the steps necessary to generate documentation for modules
in a given module collection. To cut straight to the point, see the full
documentation program code below in :ref:`example-module-doc-gen`.

Currently, documentation generation is expected to be performed in its own
binary, which we will refer to as ``build_docs`` here. This program must
include both the ``sde::printing::document_modules`` function and the
``load_modules`` function defined for your modules.

.. literalinclude:: ../../../tests/pluginplay/docs/generate_module_documentation.cpp
   :language: c++
   :lines: 1, 3-5

Next, set up a ``sde::ModuleManager`` and load your modules with the
``load_modules`` you wrote.

.. literalinclude:: ../../../tests/pluginplay/docs/generate_module_documentation.cpp
   :language: c++
   :lines: 17-21

The final step is to generate documentation through
``sde::printing::document_modules``, by providing your ``ModuleManager``
instance (``mm`` here) and the ``std::filesystem::path`` to an existing
directory where the documentation should be output (``doc_path`` here). How this
path is generated is a detail left to the developer's discretion. For simplicity
in the example below, the first command line argument is assumed to be a valid
path, although more validation may be necessary.

.. literalinclude:: ../../../tests/pluginplay/docs/generate_module_documentation.cpp
   :language: c++
   :lines: 23-24

.. _example-module-doc-gen:

Example Program
===============

The following code is a minimal example program generating documentation for
the test modules in SDE. Modify the code according to the TODO comments to
generate documentation for your modules.

.. literalinclude:: ../../../tests/pluginplay/docs/generate_module_documentation.cpp
   :language: c++
   :lines: 1-29
