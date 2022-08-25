Submodules
==========

Coulomb's Law is pretty simple. Most simulation techniques are much more
complicated than that and involve an intricate interplay of many different
properties.

As an example say we are writing a module which computes the force, |force|,
of a moving charged particle at a position |vec_r|. Assuming the particle
has a mass :math:`m`, its acceleration at |vec_r| is given by |acceleration|,
and that it has a charge :math:`q`, the |force| is given by:

.. math::

   \newcommand{\force}{\vec{F}\left(\vec{r}\right)}
   \newcommand{\acceleration}{\vec{a}\left(\vec{r}\right)}
   \newcommand{\efield}{\vec{E}\left(\vec{r}\right)}

   \force = m\acceleration + q\efield

The important point for this tutorial is that :math:`F\left(\vec{r}\right)` can
be written in terms of the electric field, |e_field|. In practice there's a lot
of different ways to compute electric fields. In many scientific codes
this is handled like:

..


Rather than hard coding the
electric field choices into our force module



Submodules change the class's definition. In the constructor we now need to
declare that our module uses a submodule and that the submodule must satisfy the
property type `ElectricField`. This looks like:

.. literalinclude:: ../../../tests/pluginplay/docs/force.cpp
   :language: c++
   :lines: 22-29

PluginPlay also requires that we specify a tag (we choose the tag
``"electric field"``). The tag ensures that the module can specify and
distinguish between multiple submodules of the same type. When appropriately
named, the tag also aids in readability of the code. The ``run_`` function for
our module looks like:

.. literalinclude:: ../../../tests/pluginplay/docs/force.cpp
   :language: c++
   :lines: 31-43

Of note is the line:

.. literalinclude:: ../../../tests/pluginplay/docs/force.cpp
   :language: c++
   :lines: 37

which says we are running the submodule tagged with ``"electric field"`` as an
``efield_type``. We provide the submodule with the point charge's location and
the list of point charges defining the electric field.

Submodules are integral to PluginPlay so it is worth noting:

- By using submodules we establish data dependencies (*i.e.* here we stated we
  need an electric field, we don't care how it's computed)
- If tomorrow someone writes a new electric field module, it can immediately be
  used with our force module without modifying the force module.
- The submodule that is called can be changed at runtime by the user and our
  module doesn't have to maintain the logic to do so (*i.e.* our force module
  doesn't need to maintain a list of all possible submodules and switch between
  them).
- If the calculation crashes after computing the electric field, but before the
  module completes (pretty unlikely given how few operations remain) memoization
  will avoid the need to recompute the electric field when the calculation is
  restarted. The module does not have to maintain separate checkpoint/restart
  logic!!!!!
