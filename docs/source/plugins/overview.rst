************************
About Module Collections
************************

As mentioned in the intro to this chapter, module collections are groups of
modules that are distributed together. In C++ module collections are libraries.
In Python they are Python modules. For the purposes of this page we assume that
our library/module is called ``OurModule``

C++ Set-Up
==========

The atual structure of your source code is arbitary; however, one popular choice
is to follow typical GNU standards. In this case your module collection's source
code will look something like:

.. code-block::

   .
   |-- include/
   |   `-- our_module/
   |       `-- our_module.hpp
   `-- src/
       `-- our_module/
           |-- module1.cpp
           |-- module2.cpp
           |-- modules.hpp
           `-- our_module.cpp

where:

- the ``include/`` directory contains the public API,
- ``our_module.hpp`` includes the ``load_modules`` function (TODO: add link) and
  any other headers needed to use the collection,
- the ``src/`` directory contains the implementation of the collection,
- ``module1.cpp`` and ``module2.cpp`` are the implementation of the modules,
- ``modules.hpp`` contains the declaration of the modules, and
- ``our_module.cpp`` contains the definition of the ``load_modules`` function.

In practice you will also have build system files, readmes, licenses, unit tests
(hopefully), etc, but from the perspective of the pluginplay those are the key parts.


Python Set-Up
=============

TODO: Write Me!!!!
