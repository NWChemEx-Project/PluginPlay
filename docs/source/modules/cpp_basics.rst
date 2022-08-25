.. _cpp_basics:

##############################
Basics of Writing a C++ Module
##############################

.. |acceleration| replace:: :math:`\vec{a}\left(\vec{r}\right)`
.. |force| replace:: :math:`\vec{F}\left(\vec{r}\right)`
.. |vec_r| replace:: :math:`\vec{r}`
.. |e_field| replace:: :math:`\vec{E}\left(\vec{r}\right)`

.. note::

   PluginPlay allows you to write modules in Python and/or in C++. This page
   details the C++ procedure. See :ref:`python_basics` for the Python
   procedure.

In many module development scenarios the API for the module has been
largely decided for you. This is because one will often be writing a
module which satisfies an already existing property type. Here we assume
the setup described in :ref:`e_and_m_101`. In particular we assume the
existence of the ``ElectricField`` property type.

********************
Declaring the Module
********************

The first step in writing a module is to declare it. In C++ this is typically
done in a header file. PluginPlay provides the C++ macro ``DECLARE_MODULE(T)``
for declaring a module of type ``T``. *N.B.* ``T`` here will be the name of
the class implementing the module, it is **NOT** the property type of the
module. Since ``T`` will be a class name it must be a valid C identifier.

Generally speaking modules of the same property type differ by the algorithm
used to compute the property. It thus makes sense to somehow include the
algorithm in the module's name. In our electric field example, all of the
modules will compute electric fields, but they differ in how they do so.
For our first module we will compute the electric field via Coulomb's Law
and we thus choose to name our module ``CoulombsLaw``. We then place the
declaration in a file ``modules.hpp``. The relevant lines are:

.. literalinclude:: ../../../tests/pluginplay/docs/modules.hpp
   :language: c++
   :lines: 1-6,15-16

.. note::

   Most libraries include more than one module, and given that declaring a
   module requires a single line, it is not uncommon to declare multiple modules
   in a single header file.

*******************
Defining the Module
*******************

The next step is to define our ``CoulombsLaw`` module. Typically the definition
is placed in a source file. Module definitions have two parts: the constructor
and the ``run`` member function. The constructor is where all the set-up for
your module occurs; it is also where you can set meta-data associated with your
module. The ``run`` member function is where your module does the work.

Defining the Constructor
========================

To aid in defining the constructor, PluginPlay provides the macro
``MODULE_CTOR(T)``, where ``T`` is the name of the module. At a minimum the
constructor must set the property type(s) that the module satisfies. It is
also good practice to provide a short description about the algorithm.

For our ``CoulombsLaw`` module a basic constructor looks like:

.. literalinclude:: ../../../tests/pluginplay/docs/coulombs_law.cpp
   :language: c++
   :lines: 7-23

One particularly nice feature of PluginPlay is that it can auto-generate
reStructuredText documentation for modules by scraping the meta-data and fields
of the module. The description we provided will serve as a preface to the
resulting documentation. Some of the other meta-data that module developers
can set in the constructor includes: a list of citations, the author of
the module, and the version. A full list of meta-data is
available (TODO: Add link).


.. note::

   PluginPlay automatically documents input parameters and submodules so there
   is no need to include these in your description.

.. hint::

   The property type will be needed in a few places so it is a good idea to set
   a typedef, this way if you need to change the property type for any reason
   you only need to change it in the typedef.

.. hint::

   The raw string literal introduced in C++11 denoted by
   ``auto str = R"(...)";`` is a great way to write a lengthy description
   without having to use escapes or other workarounds for line endings.


Defining the Run Member
=======================

The ``run`` member of the module takes a set of inputs and a set of submodules,
and returns the property (or properties). Like the module constructor,
PluginPlay provides the macro ``MODULE_RUN(T)``, where ``T`` is the name of the
module to aid in defining the module's ``run`` member function. By convention
(and hidden in the definition of the macro) the names of the two arguments are:
``inputs`` and ``submods``. Presently we will ignore the ``submods`` argument.

For defining our ``CoulombsLaw::run`` member the relevant PluginPlay bits of
code are:

.. literalinclude:: ../../../tests/pluginplay/docs/coulombs_law.cpp
   :language: c++
   :lines: 25-29,49-51

In practice PluginPlay requires every module to have the same API. Each
property type knows how to go from PluginPlay's API to the conceptual API
defined by that property type. In turn, the first step of a module will
usually be of the form:

.. code-block::

   const auto& [input0, ...] = property_type::unwrap_inputs(inputs);

where ``...`` contains identifiers for the 1-st, 2-nd, etc. inputs (presently
``r`` and ``charges``) and ``property_type`` is the name of the class
defining the property type the module satisfies (here ``ElectricField``).
Similarly, the last line will usually be:

.. code-block::

   return property_type::wrap_results(rv, ...);

where ``...`` are the results of the module (here ``E``) and ``property_type``
is again the name of the class defining the property type the module
satisfies. Between these two lines our electric field module behaves
(conceptually) like it has the API:

.. code-block::

   Point run(const Point& r, const std::vector<PointCharge>& charges);

For completeness, the full definition of the ``run`` member (including the
source code for actually computing the electric field) is:

.. literalinclude:: ../../../tests/pluginplay/docs/coulombs_law.cpp
   :language: c++
   :lines: 25-51
