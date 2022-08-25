from pluginplay import pluginplay
from pluginplay_examples import pluginplay_examples
from cppyy.gbl.std import make_shared, vector
from math import sqrt

ElectricField = pluginplay_examples.ElectricField
Force = pluginplay_examples.Force
Point = pluginplay_examples.Point
PointCharge = pluginplay_examples.PointCharge


class CoulombsLaw(pluginplay.ModuleBase):

    def __init__(self):
        super().__init__(self)
        self.description("Electric Field From Coulomb's Law")
        self.satisfies_property_type[ElectricField]()

    def run_(self, inputs, submods):
        [r, charges] = ElectricField.unwrap_inputs(inputs)
        E = Point([0.0, 0.0, 0.0])
        for charge in charges:
            q = charge.m_charge
            ri = charge.m_r
            ri2 = ri[0]**2 + ri[1]**2 + ri[2]**2
            mag_ri = sqrt(ri2)
            rij = Point(r)
            for i in range(3):
                rij[i] -= charge.m_r[i]
            rij2 = rij[0]**2 + rij[1]**2 + rij[2]**2
            for i in range(3):
                E[i] += (q * ri[i] / (mag_ri * rij2))
        rv = self.results()
        return ElectricField.wrap_results(rv, E)


class ClassicalForce(pluginplay.ModuleBase):

    def __init__(self):
        super().__init__(self)
        self.description("Classical Force Field")
        self.satisfies_property_type[Force]()
        self.add_submodule[ElectricField]("electric field").set_description(
            "Used to compute the electric field of the point charges")

    def run_(self, inputs, submods):
        [q, m, a, charges] = Force.unwrap_inputs(inputs)
        F = Point([0.0, 0.0, 0.0])
        E = submods.at("electric field").run_as[ElectricField](q.m_r, charges)
        for i in range(3):
            F[i] = m * a[i] + q.m_charge * E[0][i]
        rv = self.results()
        return Force.wrap_results(rv, F)


mm = pluginplay.ModuleManager()

mm.add_module("Force", make_shared[ClassicalForce]())
mm.add_module("Coulomb's Law", make_shared[CoulombsLaw]())

# Continued work to de-C++ Python
#mm.add_module[ClassicalForce]("Force")
#mm.add_module[CoulombsLaw]("Coulomb's Law")

mm.change_submod("Force", "electric field", "Coulomb's Law")

r = Point([0.0, 0.0, 0.0])

# Continued work to de-C++ Python
#p1 = PointCharge(1.0, [2.0, 3.0, 3.0])
#p2 = PointCharge(0.5, [-1.0, 4.0, 0.0])
#pvc = [p1, p2]

pvc = vector[PointCharge](
    [PointCharge(1.0, [2.0, 3.0, 3.0]),
     PointCharge(0.5, [-1.0, 4.0, 0.0])])
field = mm.at("Coulomb's Law").run_as[ElectricField](r, pvc)

q = PointCharge(0.1, [1.0, 1.0, 1.0])
m = 3.0
a = Point([-1.0, -1.0, -1.0])
cforce = mm.at("Force").run_as[Force](q, m, a, pvc)
