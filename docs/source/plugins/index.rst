#######
Plugins
#######

Typically developers write more than one module. When modules share some common
theme (for example they all relate to computing electric fields) developers will
often choose to distribute/bundle these modules together. We term such
distributions "module collections". The grouping of modules into collections is
purely arbitary and for the developer's convenience. Come runtime, the set of
modules available to the user wil be the union of all module collections that
have been loaded.

The sections in this chapter focus on best practices for maintaining a module
collection.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   overview
   load_modules
   document_modules
