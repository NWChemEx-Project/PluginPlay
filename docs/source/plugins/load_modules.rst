***************************
Loading a Module Collection
***************************

The key piece of a module collection is the ``load_modules`` function. When a
user starts a run with PluginPlay one of the first things they do is initialize
a ``ModuleManager`` instance. The code to do this looks something like:

.. code-block:: c++

   // Initialize ModuleManager
   ModuleManager mm;
   library1::load_modules(mm);
   library2::load_modules(mm);

where ``load_modules(mm)`` calls load the module collections provided by the
various libraries.

Public API
==========

As far as the public API for ``load_modules`` goes, the declaration is simple:

.. literalinclude:: ../../../tests/pluginplay/docs/load_modules.hpp
   :language: c++

In particular note that by forward declaring the ``load_modules`` function it is
not necessary for the user to know the types of your modules and more
importantly any libraries you use to implement them.

.. note::

   If your modules use property types defined elsewhere (such as in centralized
   community resources like NWChemEx-Project/SimDE) and your
   module-specific inputs are standard C++ types (or types found in the
   property types) this is your entire public API!!!

Implementation
==============

The actual implementation of ``load_modules`` typically consists of two parts:
loading the modules and setting the default submodules.

Modules are loaded into the ``ModuleManager`` by calling ``add_module``. For
your convenience the ``ModuleManager`` has an overload of ``add_module`` which
takes a template type parameter that is the type of your module (as in the name
of the class which contains the algorithm) and a module key. The module key is
how users will refer to your module, *e.g.*, when they want to run your module
they will call ``ModuleManager::run_as`` providing your module's key. If your
module requires more detailed set-up other overloads exist that will bind to
existing instances.

Continuing with our electric field examples, adding our modules to the
``ModuleManager`` looks like:

.. literalinclude:: ../../../tests/pluginplay/docs/load_modules.cpp
   :language: c++
   :lines: 1-11,14-16


Once you have added all your modules the next step is to set the default
submodules. In our examples, the ``ClassicalForce`` module needed a submodule
which can compute electric fields. Presently we have already loaded three such
modules; their keys are:

- "Coulomb's Law"
- "Coulomb's Law with screening"
- "Single-precision Coulomb's law"

If by default we want the ``ClassicalForce`` module we loaded under the key
"Force" to use a ``CoulombsLaw`` module we can accomplish this by:

.. literalinclude:: ../../../tests/pluginplay/docs/load_modules.cpp
   :language: c++
   :lines: 13

.. note::

   It is not necessary to set all submodules in the ``load_modules`` function.
   In particular if your module depends on a property that none of your modules
   can compute you would not set that submodule. In this case, it will be the
   responsability of the caller to set the submodule.

The entire implementation of our ``load_modules`` function looks like:

.. literalinclude:: ../../../tests/pluginplay/docs/load_modules.cpp
   :language: c++
